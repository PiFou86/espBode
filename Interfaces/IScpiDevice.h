#pragma once

class IScpiDevice
{
public:
    virtual bool sendScpiCommand(const char* cmdPattern) = 0;
    virtual const char* lastScpiCommand() = 0;
    virtual const char* lastScpiCommandReply() = 0;
};
