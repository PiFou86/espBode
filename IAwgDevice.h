#pragma once

#include <stdint.h> //uint8_t, uint32_t

class IAwgDevice
{
public:
    enum WaveType
    {
        WaveType_UNDEFINED,

        WaveType_Sine,
        WaveType_Square,
        WaveType_Pulse,
        WaveType_Triangle,
        WaveType_DC,
        WaveType_Noise,

        WaveType_Lorentz,
        WaveType_PosStair,
        WaveType_NegStair,
        WaveType_ExpFall,
        WaveType_Partialsine,
        WaveType_CMOS,
        //WaveType_Halfwave,
        //WaveType_Fullwave,
        //WaveType_IndexRise,
        //WaveType_ForSonic,
        //WaveType_SieckPulse,
    };

    struct AwgDeviceState
    {
        uint8_t   ch1Output;
        uint8_t   ch2Output;
        WaveType  ch1Wave;
        WaveType  ch2Wave;
        uint32_t  ch1Freq;
        uint32_t  ch2Freq;
        uint32_t  ch1Ampl;
        uint32_t  ch2Ampl;
        uint32_t  ch1Phase;
        uint32_t  ch2Phase;
        uint32_t  ch1Offset;
        uint32_t  ch2Offset;
    };

public:
    virtual AwgDeviceState getDeviceState() = 0;
    virtual AwgDeviceState getDeviceDefaults() = 0;
    virtual bool initDevice(AwgDeviceState settings) = 0;

    virtual bool setCh1WaveType(WaveType waveType) = 0;
    virtual bool setCh2WaveType(WaveType waveType) = 0;
    /* 0 - off; 1 - on */
    virtual bool setCh1Output(uint32_t output_OnOff) = 0;
    virtual bool setCh2Output(uint32_t output_OnOff) = 0;
    /* Set frequency in Hz */
    virtual bool setCh1Freq(uint32_t frequency_Hz) = 0;
    virtual bool setCh2Freq(uint32_t frequency_Hz) = 0;
    /* Ampl is in mV: 12.345V = 12345 */
    virtual bool setCh1Ampl(uint32_t ampl_mV) = 0;
    virtual bool setCh2Ampl(uint32_t ampl_mV) = 0;
    /* Phase is in 0.1deg: 12.5deg = 125 */
    virtual bool setCh1Phase(uint32_t phase_deziDegree) = 0;
    virtual bool setCh2Phase(uint32_t phase_deziDegree) = 0;
    /* Offset is in mV: 12.345V = 12345 */
    virtual bool setCh1Offset(int32_t offset_mV) = 0;
    virtual bool setCh2Offset(int32_t offset_mV) = 0;
};
