#pragma once

#include "Interfaces/ILxiDevice.h"

class LxiDeviceConfig;
class ITerminal;
class IScpiDevice;
class WiFiServer;
class WiFiClient;
class EspNetwork;

class LxiScpiWifiDevice : public ILxiDevice
{
public:
    LxiScpiWifiDevice(LxiDeviceConfig *lxiConfig, IScpiDevice *scpiDevice, ITerminal *terminal = nullptr);
    virtual ~LxiScpiWifiDevice();

    virtual bool connect();
    virtual bool disconnect();

    virtual bool begin();
    virtual bool loop();

protected:
    ITerminal* _terminal;
    LxiDeviceConfig* _lxiConfig;
    IScpiDevice* _scpiDevice;
    WiFiServer* _rpcServer;
    WiFiServer* _lxiServer;
    WiFiClient* _lxiClient;
    EspNetwork* _lxiHandler;
};
