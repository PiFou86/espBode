#include "TerminalBase.h"
#include <ESP8266WiFi.h> //delay()

void TerminalBase::writeKey(char key)
{
    char str[2];
    str[0] = key;
    str[1] = 0;
    std::string message = str;
    write(message);
}

void TerminalBase::writeLine(const std::string& message)
{
    std::string line = message + "\n";
    write(line);
}

void TerminalBase::writeLine()
{
    writeKey('\n');
}

char TerminalBase::readKey()
{
    char key = read();
    while (key == 0)
    {
        delay(500);
        key = read();
    }
    return key;
}

std::string TerminalBase::readLine()
{
    std::string line;
    char key = readKey();
    while (key != '\n')
    {
        line += key;
        key = readKey();
    }
    return line;
}
