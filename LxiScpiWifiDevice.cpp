/* Specify DEBUG output target by defining DEBUG_TO_SERIAL or DEBUG_TO_TELNET (or NONE) */
//#define DEBUG_TO_SERIAL
#define DEBUG_TO_TELNET
// define PRINT macros
#ifndef PRINT_TO_SERIAL
    #define PRINT_TO_SERIAL(TEXT)   Serial.println(TEXT);
#endif
#ifndef PRINT_TO_TELNET
    #define PRINT_TO_TELNET(TEXT)   telnet.println(TEXT);
#endif
// define DEBUG output macro
#ifndef DEBUG
  #ifdef DEBUG_TO_SERIAL
    #define DEBUG(TEXT)         Serial.println(TEXT);
  #endif
  #ifdef DEBUG_TO_TELNET
    #include "ESPTelnet.h"
    extern ESPTelnet telnet;
    #define DEBUG(TEXT)         telnet.println(TEXT);
  #endif
#endif
#ifndef DEBUG
  #define DEBUG(TEXT)
#endif


#include "LxiScpiWifiDevice.h"
#include "EspNetwork.h" //class EspNetwork
#include "Interfaces/IScpiDevice.h" //class IScpiDevice
#include "Interfaces/LxiDeviceConfig.h" //class LxiDeviceConfig
//#include <ESP8266WiFi.h> //class WiFiServer

LxiScpiWifiDevice::LxiScpiWifiDevice(LxiDeviceConfig *lxiConfig, IScpiDevice *scpiDevice)
{
    _lxiConfig = lxiConfig;
    _scpiDevice = scpiDevice;

    _rpcServer = new WiFiServer(_lxiConfig->rpcServerPort);
    _lxiServer = new WiFiServer(_lxiConfig->lxiServerPort); 
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

    _lxiClient.setTimeout(1000);
    do
    {
        _lxiClient = _lxiServer->accept();
    }
    while(!_lxiClient);
    _lxiClient.setTimeout(1000);
    DEBUG("LXI connection established");

    _lxiHandler = new EspNetwork(&_lxiClient, _lxiConfig, _scpiDevice);
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
    if (_lxiClient)
    {
        DEBUG("LxiScpiWifiDevice::disconnect() - delete _lxiClient.stop()");
        _lxiClient.stop();
    }
    DEBUG("LxiScpiWifiDevice::disconnect() - end");
    return true;
}

bool LxiScpiWifiDevice::loop()
{
    //DEBUG("LxiScpiWifiDevice::loop() - start");
    if (!_lxiClient || _lxiHandler == nullptr)
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
