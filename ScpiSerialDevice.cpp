#include "ScpiSerialDevice.h"
#include <HardwareSerial.h> //class HardwareSerial

ScpiSerialDevice::ScpiSerialDevice(HardwareSerial* serial)
{
    _serial = serial;
}

bool ScpiSerialDevice::writeCommandToSerial(char* data, uint8_t len)
{
    uint32_t timeout = 0;

    _lastCommand = data;
    _lastReply.clear();
    //DEBUG("[\n" + _lastCommand + "]");

    // sanity check (data should be 0 terminated string)
    if (len != _lastCommand.length()) {
        //DEBUG("invalid data or len");
        return 0;
    }

    // write command
    _serial->write((uint8_t*)data, len);

    // wait for device acknowledge response
    while (!_serial->available())
    {
        delay(1);
        if (timeout++ > 1000) return false;
    }
    
    bool ok = false;
    ok = (_serial->read() == 0x0a); // 0x0a == \n

    //if (!ok) {
    //    DEBUG("invalid response for command");
    //}
    return ok;
}

const char* ScpiSerialDevice::lastScpiCommand()
{
    return _lastCommand.c_str();
}

const char* ScpiSerialDevice::lastScpiReply()
{
    return _lastReply.c_str();
}

bool ScpiSerialDevice::sendScpiCommand(const char* cmdPattern, uint32_t param1, uint32_t param2)
{
    // this kind of cmdPatterns can contain up to 2 place holders
    // todo: add a check (ensure not more than 2 '%')
    size_t cmdLen = snprintf(NULL, 0, cmdPattern, param1, param2);
    char* command = (char*)malloc(cmdLen + 1); //reserve 1 additional byte for snpritf 0 termination character
    snprintf(command, cmdLen + 1, cmdPattern, param1, param2);
    bool ok = writeCommandToSerial(command, cmdLen);
    free(command);
    return ok;
}

bool ScpiSerialDevice::sendScpiCommand(const char* cmdPattern, uint32_t param1)
{
    // this kind of cmdPatterns must contain exactly 1 place holder!
    // todo: add a check (ensure exactly 1 '%')
    return sendScpiCommand(cmdPattern, param1, 0);
}

bool ScpiSerialDevice::sendScpiCommand(const char* cmdPattern)
{
    // this kind of cmdPatterns must not contain any place holders!
    // todo: add a check (ensure none '%')
    return sendScpiCommand(cmdPattern, 0, 0);
}