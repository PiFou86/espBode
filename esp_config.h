#pragma once
#ifndef _ESP_CONFIG_H_
#define _ESP_CONFIG_H_

#include <stdint.h>

/* AWG device config */
class ConfigAwgDevice
{
public:
    enum ConfigAwgType
    {
        UNDEFINED = 0,
        FY6800 = 1,
        FY6900 = 2,
        JDS2800 = 3,
        _LAST_
    };

    enum ConfigAwgType deviceType;

    ConfigAwgDevice()
    {
        /* Select the target AWG from: FY6900, FY6800 or JDS2800 */
        deviceType = ConfigAwgType::FY6900;
    }
};


/* WiFi config */
class ConfigWifi
{
public:  
    enum ConfigWifiMode
    {
        WIFI_MODE_AP,
        WIFI_MODE_CLIENT,
    };

    enum ConfigWifiMode wifiMode;
    const char* wifiSsid;
    const char* wifiPsk;
    bool staticIp;
    const char* staticIpAdr;
    const char* staticIpMask;
    const char* staticIpGateway;

    ConfigWifi()
    {
      /* Select Wifi configuration
          WIFI_MODE_AP      : creates new network that oscilloscope can connect to
          WIFI_MODE_CLIENT  : joins existing network 
          staticIp = false  : uses DHCP (make the router assigne always same IP) 
          staticIp = true   : uses static IP definiton (specificaly for AP mode) */
      wifiMode = ConfigWifiMode::WIFI_MODE_CLIENT;
      staticIp = false;
      staticIpAdr = "192.168.1.6";
      staticIpMask = "255.255.255.0";
      staticIpGateway = "192.168.1.1";
    }
};

class ConfigEspBode
{
public:
    ConfigWifi       wifiConfig;
    ConfigAwgDevice  awgConfig;
};

#endif /* _ESP_CONFIG_H_ */
