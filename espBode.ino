#include <ESP8266WiFi.h>
#include "esp_network.h"
#include "esp_config.h"

#include "ESPTelnet.h"

/*
#if AWG == FY6800
  #include "esp_fy6800.h"
#elif AWG == FY6900
  #include "esp_fy6900.h"
#elif AWG == JDS2800
  #include "esp_jds2800.h"
#else
  #error "Please select an AWG in esp_config.h"
#endif
*/

WiFiServer *rpc_server;
WiFiServer *lxi_server;
ESPTelnet telnet;

void setup() {

    pinMode(LED_BUILTIN, OUTPUT);

    Serial.begin(115200);

    // workaround to get Serial.print() working (synching esp with console)
    for (int i=0; i<5; i++) { Serial.println("-"); delay(250); }

    // inform about ESP flash capacity
    uint32_t realSize = ESP.getFlashChipRealSize();
    uint32_t ideSize = ESP.getFlashChipSize();
    FlashMode_t ideMode = ESP.getFlashChipMode();
    Serial.printf("Flash real id:   %08X\n", ESP.getFlashChipId());
    Serial.printf("Flash real size: %u bytes\n", realSize);
    Serial.printf("Flash ide  size: %u bytes\n", ideSize);
    Serial.printf("Flash ide speed: %u Hz\n", ESP.getFlashChipSpeed());
    Serial.printf("Flash ide mode:  %s\n", (ideMode == FM_QIO ? "QIO" : ideMode == FM_QOUT ? "QOUT" : ideMode == FM_DIO  ? "DIO" : ideMode == FM_DOUT ? "DOUT" : "UNKNOWN"));
    if (ideSize != realSize) {
        Serial.println("Flash Chip configuration wrong!\n");
    } else {
        Serial.println("Flash Chip configuration ok.\n");
    }

    // configuring and connection to WiFi
#if defined(STATIC_IP)
    IPAddress ip(ESP_IP);
    IPAddress mask(ESP_MASK);
    IPAddress gateway(ESP_GW);
    WiFi.config(ip, gateway, mask);
#endif

#if defined(WIFI_MODE_CLIENT)
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PSK);
#elif defined(WIFI_MODE_AP)
    WiFi.softAP(WIFI_SSID, WIFI_PSK);
#else
    #error PLEASE SELECT WIFI_MODE_AP OR WIFI_MODE_CLIENT!
#endif

    // We start by connecting to a WiFi network
    Serial.printf("Connecting to WiFi '%s'  ", WIFI_SSID);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
        digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    }
    Serial.printf("\nWiFi connected (IP address %s)\n", WiFi.localIP().toString().c_str());

    // network initialization
    rpc_server = new WiFiServer(RPC_PORT);
    lxi_server = new WiFiServer(LXI_PORT); 

    telnet.begin();
    rpc_server->begin();
    lxi_server->begin();
}

void loop() {

    WiFiClient rpc_client;
    do
    {
        rpc_client = rpc_server->accept();
    }
    while(!rpc_client);
    DEBUG("RPC connection established");
    handlePacket(rpc_client);
    rpc_client.stop();

    WiFiClient lxi_client;
    //lxi_client.setTimeout(1000);
    do
    {
        lxi_client = lxi_server->accept();
    }
    while(!lxi_client);
    lxi_client.setTimeout(1000);
    DEBUG("LXI connection established");

    while(1)
    {
        telnet.loop();        
        if(0 != handlePacket(lxi_client))
        {
            lxi_client.stop();
            DEBUG("RESTARTING");
            return;
        }else{
          // Lets give the user some feedback
          digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
        }
    }        
}
