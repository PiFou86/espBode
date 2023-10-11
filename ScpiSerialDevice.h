#pragma once

#include "Interfaces/IScpiDevice.h"
#include <string> //std::string
#include <stdint.h> //uint8_t

class HardwareSerial;

class ScpiSerialDevice : public IScpiDevice
{
public:
    ScpiSerialDevice(HardwareSerial* serial);

    virtual const char* lastScpiCommand();
    virtual const char* lastScpiReply();
    virtual bool sendScpiCommand(const char* cmdPattern);

protected:
    bool sendScpiCommand(const char* cmdPattern, uint32_t param1);
    bool sendScpiCommand(const char* cmdPattern, uint32_t param1, uint32_t param2);
    bool writeCommandToSerial(char* data, uint8_t len);

protected:
    HardwareSerial* _serial = nullptr;
    std::string _lastCommand;
    std::string _lastReply;
};
