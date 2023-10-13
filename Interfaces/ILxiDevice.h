#pragma once

class ILxiDevice
{
public:
    virtual ~ILxiDevice() {};

    //virtual bool connect() = 0;
    //virtual bool disconnect() = 0;

    virtual bool begin() = 0;
    virtual bool loop() = 0;
};
