#include "TerminalSerial.h"
#include <HardwareSerial.h> //class HardwareSerial
#include <stdint.h>

TerminalSerial::TerminalSerial(HardwareSerial* serial)
{
    _serial = serial;
}

void TerminalSerial::write(const std::string& message)
{
    uint8_t* data = (uint8_t*) message.c_str();

    uint8_t len = message.length();
    if (len == 0)
    {
        return;
    }

    // write
    _serial->write(data, len);
}

char TerminalSerial::read()
{
    // wait 100ms for device data
    uint32_t timeout = 0;
    while (!_serial->available())
    {
        delay(10);
        timeout += 10;
        if (timeout > 100)
        {
            return 0;
        }
    }
    return _serial->read();
}
