/* WiFi credentials */
#define WIFI_SSID   "wlan_ssid"
#define WIFI_PSK    "wlan_key"

#include "Terminal.h"

#include "ESPTelnet.h"

ESPTelnet telnet; // global telnet instance, that provides telnet based communication once the WiFI network connection is established (then used for user communication)
HardwareSerial *g_serial; // reference to Serial port that is used initially for serial console output but lateron just for communication with connected AWG device
Terminal *g_terminal;

#define TERMINAL_PRINT(text)   TERMINAL_SAFE_PRINT(g_terminal, text)
#define TERMINAL_PRINTLN(text) TERMINAL_SAFE_PRINTLN(g_terminal, text)
#define TERMINAL_PRINTF(...)   TERMINAL_SAFE_PRINTF(g_terminal, __VA_ARGS__)

#include "esp_config.h"
#include "LxiScpiWifiDevice.h"

// include the available concrete Awg device implementations
#include "AwgFY6800.h"
#include "AwgFY6900.h"
#include "AwgJDS2800.h"
#include "SDG1062Emulator.h"

ConfigEspBode *g_espConfig; // the concrete configuration instance (WiF connction, concrete AWG type) -> shall be persisted onto ESP-Filesystem and shall be ecitable via Terminal menu)
IAwgDevice *g_awgDevice;  // reference (generic AWG Device and also a generic SCPI Device) to the concrete AWG device implementation connected to serial port
SDG1062Emulator *g_sdgEmulator; //reference to the emulator that translated the LXI network commands for emulated SDG1062-Device into SCPI-AWG-commands)
//IScpiDevice *g_sdgEmulator;
ILxiDevice *g_lxiDevice; // holds reference to LXI (network) device connection


void setup() {

    pinMode(LED_BUILTIN, OUTPUT);

    // init serial interface
    g_serial = &Serial;
    g_serial->begin(115200);
    // workaround to get g_serial->print() working (synching esp with console)
    for (int i=0; i<6; i++) { g_serial->println("-"); delay(250); }

    // wrap serial and telnet into termianl 
    g_terminal = new Terminal(g_serial, &telnet);
    g_terminal->useSerial();

    TERMINAL_PRINTLN("\n----- ESP info -----");
    // inform about ESP flash capacity
    uint32_t realSize = ESP.getFlashChipRealSize();
    uint32_t ideSize = ESP.getFlashChipSize();
    FlashMode_t ideMode = ESP.getFlashChipMode();
    TERMINAL_PRINTF("Flash real id:   %08X\n", ESP.getFlashChipId());
    TERMINAL_PRINTF("Flash real size: %u bytes\n", realSize);
    TERMINAL_PRINTF("Flash ide  size: %u bytes\n", ideSize);
    TERMINAL_PRINTF("Flash ide speed: %u Hz\n", ESP.getFlashChipSpeed());
    TERMINAL_PRINTF("Flash ide mode:  %s\n", (ideMode == FM_QIO ? "QIO" : ideMode == FM_QOUT ? "QOUT" : ideMode == FM_DIO  ? "DIO" : ideMode == FM_DOUT ? "DOUT" : "UNKNOWN"));
    if (ideSize != realSize) {
      TERMINAL_PRINTLN("Flash chip configuration wrong!");
    } else {
      TERMINAL_PRINTLN("Flash chip configuration ok.");
    }


    TERMINAL_PRINTLN("\n----- Loading config -----");
    // todo: load espConfig from Filesystem and/or define interactively via seial interface...
    g_espConfig = new ConfigEspBode();
    /* Select the target AWG device */
    g_espConfig->awgConfig.deviceType = ConfigAwgDevice::FY6900;
    /* Select Wifi configuration
        WIFI_MODE_AP      : creates new network that oscilloscope can connect to
        WIFI_MODE_CLIENT  : joins existing network 
        staticIp = false  : uses DHCP (make the router assigne always same IP) 
        staticIp = true   : uses static IP definiton (specificaly for AP mode) */
    g_espConfig->wifiConfig.wifiMode = ConfigWifi::WIFI_MODE_CLIENT;
    g_espConfig->wifiConfig.wifiSsid = WIFI_SSID;
    g_espConfig->wifiConfig.wifiPsk = WIFI_PSK;
    g_espConfig->wifiConfig.staticIp = false;
    g_espConfig->wifiConfig.staticIpAdr = "192.168.1.6";
    g_espConfig->wifiConfig.staticIpMask = "255.255.255.0";
    g_espConfig->wifiConfig.staticIpGateway = "192.168.1.1";


    TERMINAL_PRINTLN("\n----- Enter setup -----");


    TERMINAL_PRINTLN("\n----- Connecting WiFi -----");
    // configuring and connecting to WiFi
    if (g_espConfig->wifiConfig.staticIp) {
        IPAddress ip, mask, gateway;
        ip.fromString(g_espConfig->wifiConfig.staticIpAdr);
        mask.fromString(g_espConfig->wifiConfig.staticIpMask);
        gateway.fromString(g_espConfig->wifiConfig.staticIpGateway);
        WiFi.config(ip, gateway, mask);
    }
    if (g_espConfig->wifiConfig.wifiMode == ConfigWifi::WIFI_MODE_CLIENT) {
        WiFi.mode(WiFiMode_t::WIFI_STA);
        WiFi.begin(g_espConfig->wifiConfig.wifiSsid, g_espConfig->wifiConfig.wifiPsk);
    } else if (g_espConfig->wifiConfig.wifiMode == ConfigWifi::WIFI_MODE_AP) {
        WiFi.softAP(g_espConfig->wifiConfig.wifiSsid, g_espConfig->wifiConfig.wifiPsk);
    } else {
      //#error PLEASE SELECT WIFI_MODE_AP OR WIFI_MODE_CLIENT!
    }
    // We start by connecting to a WiFi network
    TERMINAL_PRINTF("connecting to '%s'  ", WIFI_SSID);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        TERMINAL_PRINT(".");
        digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    }
    TERMINAL_PRINTF("\nWiFi connected (IP address %s)\n", WiFi.localIP().toString().c_str());


    TERMINAL_PRINTLN("\n----- Connecting to UART AWG device -----");
    // create AwgDevice
    if (g_espConfig->awgConfig.deviceType == ConfigAwgDevice::FY6800) {
      g_awgDevice = new AwgFY6800(g_serial);
    } else if (g_espConfig->awgConfig.deviceType == ConfigAwgDevice::FY6900) {
      g_awgDevice = new AwgFY6900(g_serial);
    } else if (g_espConfig->awgConfig.deviceType == ConfigAwgDevice::JDS2800) {
      //g_awgDevice = new AwgDS2800(g_serial);
    }


    TERMINAL_PRINTLN("\n----- Starting Siglent AWG LXI Network emulation -----");
    g_sdgEmulator = new SDG1062Emulator(g_awgDevice);
    g_lxiDevice = new LxiScpiWifiDevice(g_sdgEmulator->lxiConfig(), g_sdgEmulator);

    // network initialization
    g_lxiDevice->begin();

    // now that WiFi is connected we can init the telnet and replace the terminal implementation
    telnet.begin();
    g_terminal->useTelnet();
}


void loop() {
    while(1)
    {
        telnet.loop();
        //g_terminal->loop();

        TERMINAL_PRINT(".");
        if (!g_lxiDevice->loop())
        {
            TERMINAL_PRINTLN("RESTARTING");
            return;
        }

        // Lets give the user some hardware feedback
        digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    }        
}
