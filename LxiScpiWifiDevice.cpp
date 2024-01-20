#include "LxiScpiWifiDevice.h"

#include <ESP8266WiFi.h> //class WiFiServer, WifiClient
#include "EspNetwork.h" //class EspNetwork

#include "Interfaces/IScpiDevice.h" //class IScpiDevice
#include "Interfaces/LxiDeviceConfig.h" //class LxiDeviceConfig
#include "Interfaces/ITerminal.h"
#define DEBUG(text)  if (_terminal) { _terminal->writeLine(text); }

LxiScpiWifiDevice::LxiScpiWifiDevice(LxiDeviceConfig *lxiConfig, IScpiDevice *scpiDevice, ITerminal *terminal)
{
    _lxiConfig = lxiConfig;
    _scpiDevice = scpiDevice;
    _terminal = terminal;

    _rpcServer = new WiFiServer(_lxiConfig->rpcServerPort);
    _lxiServer = new WiFiServer(_lxiConfig->lxiServerPort); 
    _lxiClient = new WiFiClient();
}

LxiScpiWifiDevice::~LxiScpiWifiDevice()
{
    disconnect();
    if (_rpcServer != nullptr)
    {
        delete _rpcServer;
        _rpcServer = nullptr;
    }
    if (_lxiServer != nullptr)
    {
        delete _lxiServer;
        _lxiServer = nullptr;
    }
    if (_lxiClient != nullptr)
    {
        delete _lxiClient;
        _lxiClient = nullptr;
    }
}


bool LxiScpiWifiDevice::begin()
{
    _rpcServer->begin();
    _lxiServer->begin();
    return true;
}

bool LxiScpiWifiDevice::connect()
{
    DEBUG("LxiScpiWifiDevice::connect() - start");
    WiFiClient rpcClient;
    do
    {
        rpcClient = _rpcServer->accept();
        if (!rpcClient) 
        {
            delay(1000);
            DEBUG(".");
        }
    }
    while(!rpcClient);
    DEBUG("RPC connection established");

    EspNetwork *rpcHandler = new EspNetwork(&rpcClient, _lxiConfig, _scpiDevice);
    auto rpcHandlePacketReturn = rpcHandler->handlePacket();
    delete rpcHandler;
    rpcClient.stop();

    if(rpcHandlePacketReturn != 0)
    {
        disconnect();
        DEBUG("LxiScpiWifiDevice::connect() - rpcHandler->handlePacket() failed");
        return false;
    }

    do
    {
        (*_lxiClient) = _lxiServer->accept();
    }
    while(!(*_lxiClient));
    _lxiClient->setTimeout(1000);
    DEBUG("LXI connection established");

    _lxiHandler = new EspNetwork(_lxiClient, _lxiConfig, _scpiDevice);
    DEBUG("LxiScpiWifiDevice::connect() - end");
    return true;
}

bool LxiScpiWifiDevice::disconnect()
{
    DEBUG("LxiScpiWifiDevice::disconnect() - start");
    if (_lxiHandler != nullptr)
    {
        DEBUG("LxiScpiWifiDevice::disconnect() - delete _lxiHandler");
        delete _lxiHandler;
        _lxiHandler = nullptr;
    }
    if (_lxiClient != nullptr)
    {
        DEBUG("LxiScpiWifiDevice::disconnect() - _lxiClient->stop()");
        _lxiClient->stop();
    }
    DEBUG("LxiScpiWifiDevice::disconnect() - end");
    return true;
}

bool LxiScpiWifiDevice::loop()
{
    //DEBUG("LxiScpiWifiDevice::loop() - start");
    if (_lxiClient == nullptr || _lxiHandler == nullptr)
    {
        if (!connect())
        {
            DEBUG("LxiScpiWifiDevice::loop() - connect() failed");
            disconnect();
            return false;
        }
    }

    if(0 != _lxiHandler->handlePacket())
    {
        DEBUG("LxiScpiWifiDevice::loop() - handlePacket() failed");
        disconnect();
        return false;
    }

    //DEBUG("LxiScpiWifiDevice::loop() - end");
    return true;
}
