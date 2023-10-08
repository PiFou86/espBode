#pragma once

#include "ScpiSerialDevice.h"
#include <stdint.h> //uint8_t, uint32_t


class AwgDevice : protected ScpiSerialDevice
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
    AwgDevice(HardwareSerial* serial);

    AwgDeviceState getDeviceDefaults();
    bool initDevice(AwgDeviceState settings);

    virtual bool setCh1WaveType(WaveType waveType);
    virtual bool setCh2WaveType(WaveType waveType);
    /* 0 - off; 1 - on */
    virtual bool setCh1Output(uint32_t output_OnOff);
    virtual bool setCh2Output(uint32_t output_OnOff);
    /* Set frequency in Hz */
    virtual bool setCh1Freq(uint32_t frequency_Hz);
    virtual bool setCh2Freq(uint32_t frequency_Hz);
    /* Ampl is in mV: 12.345V = 12345 */
    virtual bool setCh1Ampl(uint32_t ampl_mV);
    virtual bool setCh2Ampl(uint32_t ampl_mV);
    /* Phase is in 0.1deg: 12.5deg = 125 */
    virtual bool setCh1Phase(uint32_t phase_deziDegree);
    virtual bool setCh2Phase(uint32_t phase_deziDegree);
    /* Offset is in mV: 12.345V = 12345 */
    virtual bool setCh1Offset(int32_t offset_mV);
    virtual bool setCh2Offset(int32_t offset_mV);

protected:
    virtual bool sendCh1WaveTypeCommand(WaveType waveType);
    virtual bool sendCh2WaveTypeCommand(WaveType waveType);
    virtual bool sendCh1OutputCommand(uint32_t output_OnOff);
    virtual bool sendCh2OutputCommand(uint32_t output_OnOff);
    virtual bool sendCh1FrequencyCommand(uint32_t frequency_Hz);
    virtual bool sendCh2FrequencyCommand(uint32_t frequency_Hz);
    virtual bool sendCh1AmplitudeCommand(uint32_t ampl_mV);
    virtual bool sendCh2AmplitudeCommand(uint32_t ampl_mV);
    virtual bool sendCh1PhaseCommand(uint32_t phase_deziDegree);
    virtual bool sendCh2PhaseCommand(uint32_t phase_deziDegree);
    virtual bool sendCh1OffsetCommand(int32_t offset_mV);
    virtual bool sendCh2OffsetCommand(int32_t offset_mV);

protected:
    AwgDeviceState _defaults;
    AwgDeviceState _state;
};
