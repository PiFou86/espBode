#pragma once

#include "ITerminal.h"

class TerminalSerial;
class TerminalTelnet;
class HardwareSerial;
class ESPTelnet;

class Terminal : public ITerminal
{
public:
    Terminal(HardwareSerial* serial, ESPTelnet* telnet);
    void begin();
    void loop();
    void useSerial();
    void useTelnet();

    virtual char read();
    virtual void write(const std::string& message);
    virtual char readKey();
    virtual std::string readLine();
    virtual void writeKey(char key);
    virtual void writeLine(const std::string& message);
    virtual void writeLine();
    
private:
    ITerminal* _terminal;
    TerminalSerial* _terminalSerial;
    TerminalTelnet* _terminalTelnet;
};
