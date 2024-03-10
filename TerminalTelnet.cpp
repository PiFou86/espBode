#include "TerminalTelnet.h"

#include <EspTelnet.h> //class ESPTelnet

//#define TELNET_PRINT(text)    ESPTerminal.write(text);
//#define TELNET_PRINTLN(text)  ESPTerminal.writeLine(text);}
//#define TELNET_PRINTF(...)    {int sz=std::snprintf(nullptr,0,__VA_ARGS__);std::vector<char>buf(sz+1);std::sprintf(buf.data(),__VA_ARGS__);ESPTerminal.write(buf.data());}

TerminalTelnet::TerminalTelnet(ESPTelnet* telnet)
{
    _telnet = telnet;
    //_telnet->onInputReceived(CallbackFunction f)
}

void TerminalTelnet::begin()
{
    //_telnet->begin(port: 23, checkConnection: true);
    _telnet->begin(23, true);
}

void TerminalTelnet::loop()
{
    _telnet->loop();
}

// reads next character - returns 0 if there is no data available
char TerminalTelnet::read()
{
    // todo: implement
    return 0;
}

// writes message
void TerminalTelnet::write(const std::string& message)
{
    uint8_t* data = (uint8_t*) message.c_str();

    uint8_t len = message.length();
    if (len == 0)
    {
        return;
    }

    // write
    _telnet->write(data, len);  
}
