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

bool LxiScpiWifiDevice::begin()
{
    _rpcServer->begin();
    _lxiServer->begin();
    return true;
}

bool LxiScpiWifiDevice::connect()
{
    WiFiClient rpcClient;
    do
    {
        rpcClient = _rpcServer->accept();
    }
    while(!rpcClient);
    DEBUG("RPC connection established");

    EspNetwork *rpcHandler = new EspNetwork(&rpcClient, _lxiConfig, _scpiDevice);
    rpcHandler->handlePacket();
    delete(rpcHandler);
    rpcClient.stop();

    _lxiClient.setTimeout(1000);
    do
    {
        _lxiClient = _lxiServer->accept();
    }
    while(!_lxiClient);
    _lxiClient.setTimeout(1000);
    DEBUG("LXI connection established");

    _lxiHandler = new EspNetwork(&_lxiClient, _lxiConfig, _scpiDevice);
    return true;
}

bool LxiScpiWifiDevice::loop()
{
    if(0 != _lxiHandler->handlePacket())
    {
        delete(_lxiHandler);
        _lxiClient.stop();
        DEBUG("RESTARTING");
        return false;
    }
    return true;
}
