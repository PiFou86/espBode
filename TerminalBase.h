#pragma once
#include "Interfaces/ITerminal.h"

class TerminalBase : public ITerminal
{
public:
    virtual void writeKey(char key);
    virtual void writeLine(const std::string& message);
    virtual void writeLine();

    virtual char readKey();
    virtual std::string readLine();
};