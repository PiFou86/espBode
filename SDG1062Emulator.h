#pragma once

#include "Interfaces/IScpiDevice.h"
#include <string> //std::string

class IAwgDevice;
class LxiDeviceConfig;

class SDG1062Emulator : public IScpiDevice
{
public:
    SDG1062Emulator(IAwgDevice *targetAwgDevice);

    LxiDeviceConfig* lxiConfig();

    virtual const char* lastScpiCommand();
    virtual const char* lastScpiReply();
    
    virtual bool sendScpiCommand(const char* cmdPattern);

protected:
    // from esp_parser.h
    static int32_t parseDecimal(const char* msg);
    static uint32_t parseNumber(const char* msg);
    bool handleWriteMsg(const char* msg, uint8_t len);

protected:
    IAwgDevice* _awgDevice = nullptr;
    LxiDeviceConfig* _lxiConfig;
    std::string _lastCommand;
    std::string _lastReply;
};
