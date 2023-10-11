#include "AwgSerialDeviceBase.h"
#include <HardwareSerial.h> //class HardwareSerial
#include <string.h>

AwgSerialDeviceBase::AwgSerialDeviceBase(HardwareSerial* serial)
    : ScpiSerialDevice(serial)
{
    // define defaults
    _defaults.ch1Output = 0;  //off (0=off 1=on)
    _defaults.ch2Output = 0;  //off
    _defaults.ch1Wave = WaveType_Sine;
    _defaults.ch2Wave = WaveType_Sine;
    _defaults.ch1Freq = 1000; //1000Hz (in Hz)
    _defaults.ch2Freq = 1000; //1000Hz
    _defaults.ch1Phase = 0;   //0deg (in 0.1deg => 125 = 12.5deg)
    _defaults.ch2Phase = 0;   //0deg
    _defaults.ch1Ampl = 1000; //1000mV (in mV)
    _defaults.ch2Ampl = 1000; //1000mV
    _defaults.ch1Offset = 0;  //0mV (in mv)
    _defaults.ch2Offset = 0;  //0mV

    // init state with undefined values
    _state.ch1Output = 0;
    _state.ch2Output = 0;
    _state.ch1Wave = WaveType_UNDEFINED;
    _state.ch2Wave = WaveType_UNDEFINED;
    _state.ch1Freq = 0;
    _state.ch2Freq = 0;
    _state.ch1Phase = 0;
    _state.ch2Phase = 0;
    _state.ch1Ampl = 0;
    _state.ch2Ampl = 0;
    _state.ch1Offset = 0;
    _state.ch2Offset = 0;
}

AwgSerialDeviceBase::AwgDeviceState AwgSerialDeviceBase::getDeviceState()
{
    return _state;
}

AwgSerialDeviceBase::AwgDeviceState AwgSerialDeviceBase::getDeviceDefaults()
{
    return _defaults;
}

bool AwgSerialDeviceBase::initDevice(AwgDeviceState settings)
{
    bool ok = true;
    ok &= setCh1Output(0);
    ok &= setCh2Output(0);
    ok &= setCh1WaveType(settings.ch1Wave);
    ok &= setCh2WaveType(settings.ch2Wave);
    ok &= setCh1Freq(settings.ch1Freq);
    ok &= setCh2Freq(settings.ch2Freq);
    ok &= setCh1Phase(settings.ch1Phase);
    ok &= setCh2Phase(settings.ch2Phase);
    ok &= setCh1Ampl(settings.ch1Ampl);
    ok &= setCh2Ampl(settings.ch2Ampl);
    ok &= setCh1Offset(settings.ch1Offset);
    ok &= setCh2Offset(settings.ch2Offset);
    ok &= setCh1Output(settings.ch1Output);
    ok &= setCh2Output(settings.ch2Output);
    return ok;
}

bool AwgSerialDeviceBase::setCh1WaveType(WaveType waveType)
{
    if (sendCh1WaveTypeCommand(waveType))
    {
        _state.ch1Wave = waveType;
        return true;
    }
    return false;
}

bool AwgSerialDeviceBase::setCh2WaveType(WaveType waveType)
{
    if (sendCh2WaveTypeCommand(waveType))
    {
        _state.ch2Wave = waveType;
        return true;
    }
    return false;
}

bool AwgSerialDeviceBase::setCh1Output(uint32_t output_OnOff)
{
    if (sendCh1OutputCommand(output_OnOff))
    {
        _state.ch1Output = output_OnOff;
        return true;
    }
    return false;
}

bool AwgSerialDeviceBase::setCh2Output(uint32_t output_OnOff)
{
    if (sendCh2OutputCommand(output_OnOff))
    {
        _state.ch2Output = output_OnOff;
        return true;
    }
    return false;
}

bool AwgSerialDeviceBase::setCh1Freq(uint32_t frequency_Hz)
{
    if (sendCh1FrequencyCommand(frequency_Hz))
    {
        _state.ch1Freq = frequency_Hz;
        return true;
    }
    return false;
}

bool AwgSerialDeviceBase::setCh2Freq(uint32_t frequency_Hz)
{
    if (sendCh2FrequencyCommand(frequency_Hz))
    {
        _state.ch2Freq = frequency_Hz;
        return true;
    }
    return false;
}

bool AwgSerialDeviceBase::setCh1Ampl(uint32_t ampl_mV)
{
    if (sendCh1AmplitudeCommand(ampl_mV))
    {
        _state.ch1Ampl = ampl_mV;
        return true;
    }
    return false;
}

bool AwgSerialDeviceBase::setCh2Ampl(uint32_t ampl_mV)
{
    if (sendCh2AmplitudeCommand(ampl_mV))
    {
        _state.ch2Ampl = ampl_mV;
        return true;
    }
    return false;
}

bool AwgSerialDeviceBase::setCh1Phase(uint32_t phase_deziDegree)
{
    if (sendCh1PhaseCommand(phase_deziDegree))
    {
        _state.ch1Phase = phase_deziDegree;
        return true;
    }
    return false;
}

bool AwgSerialDeviceBase::setCh2Phase(uint32_t phase_deziDegree)
{
    if (sendCh2PhaseCommand(phase_deziDegree))
    {
        _state.ch2Phase = phase_deziDegree;
        return true;
    }
    return false;
}

bool AwgSerialDeviceBase::setCh1Offset(int32_t offset_mV)
{
    if (sendCh1OffsetCommand(offset_mV))
    {
        _state.ch1Offset = offset_mV;
        return true;
    }
    return false;
}

bool AwgSerialDeviceBase::setCh2Offset(int32_t offset_mV)
{
    if (sendCh2OffsetCommand(offset_mV))
    {
        _state.ch2Offset = offset_mV;
        return true;
    }
    return false;
}
