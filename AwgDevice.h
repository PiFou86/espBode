#pragma once

#include <HardwareSerial.h>
#include <string> //std::string
#include <stdint.h> //uint8_t

enum AwgWaveType
{
    AwgWaveType_UNDEFINED,

    AwgWaveType_Sine,
    AwgWaveType_Square,
    AwgWaveType_Pulse,
    AwgWaveType_Triangle,
    AwgWaveType_DC,
    AwgWaveType_Noise,

    AwgWaveType_Lorentz,
    AwgWaveType_PosStair,
    AwgWaveType_NegStair,
    AwgWaveType_ExpFall,
    AwgWaveType_Partialsine,
    AwgWaveType_CMOS,
    //AwgWaveType_Halfwave,
    //AwgWaveType_Fullwave,
    //AwgWaveType_IndexRise,
    //AwgWaveType_ForSonic,
    //AwgWaveType_SieckPulse,
};

typedef struct
{
    uint8_t     ch1Output;
    uint8_t     ch2Output;
    AwgWaveType ch1Wave;
    AwgWaveType ch2Wave;
    uint32_t    ch1Freq;
    uint32_t    ch2Freq;
    uint32_t    ch1Ampl;
    uint32_t    ch2Ampl;
    uint32_t    ch1Phase;
    uint32_t    ch2Phase;
    uint32_t    ch1Offset;
    uint32_t    ch2Offset;

} AwgDeviceState;


class AwgDevice
{
public:
    AwgDevice(HardwareSerial* serial);

    bool sendDeviceCommand(const char* cmdPattern);
    const char* lastDeviceCommand();
    const char* lastDeviceCommandReply();

    AwgDeviceState getDeviceDefaults();
    bool initDevice(AwgDeviceState settings);

    virtual bool setCh1WaveType(AwgWaveType waveType);
    virtual bool setCh2WaveType(AwgWaveType waveType);
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
    virtual bool sendCh1WaveTypeCommand(AwgWaveType waveType);
    virtual bool sendCh2WaveTypeCommand(AwgWaveType waveType);
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

    bool writeCommandToSerial(char* data, uint8_t len);
    bool sendDeviceCommand(const char* cmdPattern, uint32_t param1, uint32_t param2);
    bool sendDeviceCommand(const char* cmdPattern, uint32_t param1);

protected:
    HardwareSerial* _serial = nullptr;
    String _lastCommand;
    String _lastReply;
    AwgDeviceState _defaults;
    AwgDeviceState _state;
};
