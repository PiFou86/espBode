#pragma once

#include <Arduino.h>

class AWGProxyBase {
    public:
    virtual void setCh1Wave(uint8_t wave) = 0;
    virtual void setCh2Wave(uint8_t wave) = 0;
    
    /* 0 - off; 1 - on */
    virtual void setCh1Output(uint32_t output) = 0;
    virtual void setCh2Output(uint32_t output) = 0;
    
    /* Set frequency in Hz */
    virtual void setCh1Freq(uint32_t frequency) = 0;
    virtual void setCh2Freq(uint32_t frequency) = 0;
    
    /* Ampl is in mV: 12.345V = 12345 */
    virtual void setCh1Ampl(uint32_t ampl) = 0;
    virtual void setCh2Ampl(uint32_t ampl) = 0;
    
    /* Phase is in 0.1deg: 12.5deg = 125 */
    virtual void setCh1Phase(uint32_t phase) = 0;
    virtual void setCh2Phase(uint32_t phase) = 0;
    
    /* Offset is in mV: 12.345V = 12345 */
    virtual void setCh1Offset(int32_t offset) = 0;
    virtual void setCh2Offset(int32_t offset) = 0;
    
    /* Can be used to set some default parameters */
    virtual void begin(void) = 0;

    virtual ~AWGProxyBase() {}
};
