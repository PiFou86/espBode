#ifndef _ESP_CONFIG_H_
#define _ESP_CONFIG_H_

#define FY6800 1
#define FY6900 2
#define JDS2800 3

/* Specify DEBUG output target by defining DEBUG_TO_SERIAL or DEBUG_TO_TELNET (or NONE) */
//#define DEBUG_TO_SERIAL
#define DEBUG_TO_TELNET

/* Select the target AWG from: FY6900, FY6800 or JDS2800 */
#define AWG FY6900

/* Select either AP or CLIENT mode:
    - AP - creates new network that oscilloscope can connect to
    - CLIENT - joins existing network
    */
//#define WIFI_MODE_AP
#define WIFI_MODE_CLIENT

/* WiFi credentials */
#define WIFI_SSID             "wlan_ssid"
#define WIFI_PSK              "wlan_key"

/* Comment this for DHCP. However you'll need to obtain IP somehow. */
#define STATIC_IP

/* Static ip configuration */
#ifdef STATIC_IP
  #define ESP_IP              192,168,1,6
  #define ESP_MASK            255,255,255,0
  #define ESP_GW              192,168,1,1
#endif

#define ID                  "IDN-SGLT-PRI SDG1062X\n"

#define RPC_PORT            (111)
#define LXI_PORT            (703)

#define PORTMAP             (0x000186A0)
#define VXI_11_CORE         (0x000607AF)

#define PORTMAP_GETPORT     (0x00000003)

#define RPC_SINGLE_FRAG     (0x80000000)
#define RPC_REPLY           (0x00000001)

#define VXI_11_CREATE_LINK  (10)
#define VXI_11_DESTROY_LINK (23)
#define VXI_11_DEV_WRITE    (11)
#define VXI_11_DEV_READ     (12)

#define RX_BUFF_SIZE        (128)

// define DEBUG macro
#ifndef DEBUG
  #ifdef DEBUG_TO_SERIAL
    #define DEBUG(TEXT)         Serial.println(TEXT);
  #endif
#endif
#ifndef DEBUG
  #ifdef DEBUG_TO_TELNET
    #include "ESPTelnet.h"
    extern ESPTelnet telnet;
    #define DEBUG(TEXT)         telnet.println(TEXT);
  #endif
#endif
#ifndef DEBUG
  #define DEBUG(TEXT)
#endif

#endif /* _ESP_CONFIG_H_ */
