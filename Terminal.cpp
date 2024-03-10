#include "Terminal.h"

#include "TerminalSerial.h"
#include "TerminalTelnet.h"

#include <HardwareSerial.h> //class HardwareSerial
#include <EspTelnet.h> //class ESPTelnet
   
Terminal::Terminal(HardwareSerial* serial, ESPTelnet* telnet)
{
    _terminalSerial = new TerminalSerial(serial);
    _terminalTelnet = new TerminalTelnet(telnet);
    _terminal = _terminalSerial; 
}

void Terminal::begin()
{
    //_terminalSerial->begin();
    //_terminalTelnet->begin();
}

void Terminal::loop()
{
    //_terminalSerial->loop();
    //_terminalTelnet->loop();
}

void Terminal::useSerial()
{
    _terminal = _terminalSerial; 
}

void Terminal::useTelnet()
{
    _terminal = _terminalTelnet; 
}

char Terminal::read()
{
    if (_terminal != nullptr)
        return _terminal->read();
    return 0;
}

void Terminal::write(const std::string& message)
{
    if (_terminal != nullptr)
        _terminal->read(); 
}

char Terminal::readKey()
{
    if (_terminal != nullptr)
        return _terminal->readKey();
    return 0;
}

std::string Terminal::readLine()
{
    if (_terminal != nullptr)
        return _terminal->readLine();
    return "";
}

void Terminal::writeKey(char key)
{
    if (_terminal != nullptr)
        _terminal->writeKey(key);
}

void Terminal::writeLine(const std::string& message)
{
    if (_terminal != nullptr)
        _terminal->writeLine(message);
}

void Terminal::writeLine()
{
    if (_terminal != nullptr)
        _terminal->writeLine();
}
