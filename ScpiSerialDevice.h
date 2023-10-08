#pragma once

#include <HardwareSerial.h>
#include <string> //std::string
#include <stdint.h> //uint8_t

class ScpiSerialDevice
{
public:
    ScpiSerialDevice(HardwareSerial* serial);

    bool sendScpiCommand(const char* cmdPattern);
    bool sendScpiCommand(const char* cmdPattern, uint32_t param1);
    bool sendScpiCommand(const char* cmdPattern, uint32_t param1, uint32_t param2);

    const char* lastScpiCommand();
    const char* lastScpiCommandReply();

protected:
    bool writeCommandToSerial(char* data, uint8_t len);

protected:
    HardwareSerial* _serial = nullptr;
    String _lastCommand;
    String _lastReply;
};
