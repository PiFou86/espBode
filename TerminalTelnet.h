#pragma once
#include "TerminalBase.h"

class ESPTelnet;

class TerminalTelnet : public TerminalBase
{
public:
    TerminalTelnet(ESPTelnet* telnet);
    void begin();
    void loop();

    // reads next character - returns 0 if there is no data available
    virtual char read();

    // writes message
    virtual void write(const std::string& message);

protected:
    ESPTelnet* _telnet = nullptr;
};