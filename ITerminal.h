#pragma once

#define TERMINAL_SAFE_PRINT(ref,text)   if(ref){(ref)->write(text);}
#define TERMINAL_SAFE_PRINTLN(ref,text) if(ref){(ref)->writeLine(text);}
#define TERMINAL_SAFE_PRINTF(ref,...)   if(ref){int sz=std::snprintf(nullptr,0,__VA_ARGS__);std::vector<char>buf(sz+1);std::sprintf(buf.data(),__VA_ARGS__);(ref)->write(buf.data());}

//#define TERMINAL_PRINT(text)   TERMINAL_SAFE_PRINT(g_terminal, text)
//#define TERMINAL_PRINTLN(text) TERMINAL_SAFE_PRINTLN(g_terminal, text)
//#define TERMINAL_PRINTF(...)   TERMINAL_SAFE_PRINTF(g_terminal, __VA_ARGS__)

#include <string>

class ITerminal
{
public:
    // reads next character - returns 0 if there is no data available
    virtual char read() = 0;
    // writes message
    virtual void write(const std::string& message) = 0;

    // reads a char from input - blocks until a key was read
    virtual char readKey() = 0;
    // reads a line from input - blocks until a key was read
    virtual std::string readLine() = 0;

    virtual void writeKey(char key) = 0;
    virtual void writeLine(const std::string& message) = 0;
    virtual void writeLine() = 0;
};
