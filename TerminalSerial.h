#pragma once

#include "TerminalBase.h"

class HardwareSerial;

class TerminalSerial : public TerminalBase
{
public:
    TerminalSerial(HardwareSerial* serial);

    // reads next character - returns 0 if there is no data available
    virtual char read();

    // writes message
    virtual void write(const std::string& message);

protected:
    HardwareSerial* _serial = nullptr;
};
