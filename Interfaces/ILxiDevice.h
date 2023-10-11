#pragma once

class ILxiDevice
{
public:
    virtual bool begin() = 0;

    virtual bool connect() = 0;

    virtual bool loop() = 0;

    //virtual ~ILxiDevice() = 0;
};
