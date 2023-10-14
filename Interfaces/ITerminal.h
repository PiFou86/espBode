#pragma once

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
