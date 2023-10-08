#pragma once

#include "Interfaces/IAwgDevice.h"
#include "ScpiSerialDevice.h"
#include <stdint.h> //uint8_t, uint32_t

class AwgSerialDeviceBase : public IAwgDevice, public ScpiSerialDevice
{
public:
    AwgSerialDeviceBase(HardwareSerial* serial);

    virtual AwgDeviceState getDeviceState();
    virtual AwgDeviceState getDeviceDefaults();
    virtual bool initDevice(AwgDeviceState settings);

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
    virtual bool sendCh1WaveTypeCommand(WaveType waveType) = 0;
    virtual bool sendCh2WaveTypeCommand(WaveType waveType) = 0;
    virtual bool sendCh1OutputCommand(uint32_t output_OnOff) = 0;
    virtual bool sendCh2OutputCommand(uint32_t output_OnOff) = 0;
    virtual bool sendCh1FrequencyCommand(uint32_t frequency_Hz) = 0;
    virtual bool sendCh2FrequencyCommand(uint32_t frequency_Hz) = 0;
    virtual bool sendCh1AmplitudeCommand(uint32_t ampl_mV) = 0;
    virtual bool sendCh2AmplitudeCommand(uint32_t ampl_mV) = 0;
    virtual bool sendCh1PhaseCommand(uint32_t phase_deziDegree) = 0;
    virtual bool sendCh2PhaseCommand(uint32_t phase_deziDegree) = 0;
    virtual bool sendCh1OffsetCommand(int32_t offset_mV) = 0;
    virtual bool sendCh2OffsetCommand(int32_t offset_mV) = 0;

protected:
    AwgDeviceState _defaults;
    AwgDeviceState _state;
};
