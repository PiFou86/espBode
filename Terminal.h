#pragma once

#include "ITerminal"
//#include "TerminalSerial.h"


#define TERMINAL_PRINT(text)    ESPTerminal.write(text);
#define TERMINAL_PRINTLN(text)  ESPTerminal.writeLine(text);}
#define TERMINAL_PRINTF(...)    {int sz=std::snprintf(nullptr,0,__VA_ARGS__);std::vector<char>buf(sz+1);std::sprintf(buf.data(),__VA_ARGS__);ESPTerminal.write(buf.data());}

class Terminal : public ITerminal
{
public:
  begin();
  loop();
  useSerial();
  useTelnet();
private:
  ITerminal* _terminal;
};
