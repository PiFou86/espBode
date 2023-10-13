#pragma once

#include "Interfaces/ILxiDevice.h"

class LxiDeviceConfig;
class IScpiDevice;
class WiFiServer;
class EspNetwork;
#include <ESP8266WiFi.h> //class WiFiServer, WifiClient

class LxiScpiWifiDevice : public ILxiDevice
{
public:
    LxiScpiWifiDevice(LxiDeviceConfig *lxiConfig, IScpiDevice *scpiDevice);
    virtual ~LxiScpiWifiDevice();

    virtual bool connect();
    virtual bool disconnect();

    virtual bool begin();
    virtual bool loop();

protected:
    LxiDeviceConfig* _lxiConfig;
    IScpiDevice* _scpiDevice;
    WiFiServer* _rpcServer;
    WiFiServer* _lxiServer;
    EspNetwork* _lxiHandler;
    WiFiClient _lxiClient;
};

