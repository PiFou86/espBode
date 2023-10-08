#include "AwgFy6900.h"

AwgFY6900::FY6900WaveType AwgFY6900::MapWaveType(IAwgDevice::WaveType waveType)
{
    switch (waveType)
    {
    case IAwgDevice::WaveType_Sine:
        return WaveType_Sine;
    case IAwgDevice::WaveType_Square:
        return WaveType_Square;
    case IAwgDevice::WaveType_Pulse:
        return WaveType_Impulse; //??
    case IAwgDevice::WaveType_Triangle:
        return WaveType_Triangle;
    case IAwgDevice::WaveType_DC:
        return WaveType_DC;
    case IAwgDevice::WaveType_Noise:
        return WaveType_Noise;
    case IAwgDevice::WaveType_Lorentz:
        return WaveType_Lorentz;
    case IAwgDevice::WaveType_PosStair:
        return WaveType_PosStair;
    case IAwgDevice::WaveType_NegStair:
        return WaveType_NegStair;
    case IAwgDevice::WaveType_ExpFall:
        return WaveType_PosExp; //??
    case IAwgDevice::WaveType_Partialsine:
        return WaveType_PosHalfSin; //??
    case IAwgDevice::WaveType_CMOS:
        return WaveType_CMOS;
    default:
      break;
    }
    return WaveType_UNDEFINED;
}

bool AwgFY6900::sendCh1WaveTypeCommand(WaveType waveType)
{
    return sendScpiCommand("WMW%02u\n", MapWaveType(waveType));
}

bool AwgFY6900::sendCh2WaveTypeCommand(WaveType waveType)
{
    return sendScpiCommand("WFW%02u\n", MapWaveType(waveType));
}

bool AwgFY6900::sendCh1OutputCommand(uint32_t output_OnOff)
{
    return sendScpiCommand(output_OnOff ? "WMN1\n" : "WMN0\n");
}

bool AwgFY6900::sendCh2OutputCommand(uint32_t output_OnOff)
{
    return sendScpiCommand(output_OnOff ? "WFN1\n" : "WFN0\n");
}

bool AwgFY6900::sendCh1FrequencyCommand(uint32_t frequency_Hz)
{
    // todo: check if the FY6900-100Mhz version has a frquency digit more and if so if it is backward compatible with the 14 digit commands 
    return sendScpiCommand("WMF%08u000000\n", frequency_Hz);
}

bool AwgFY6900::sendCh2FrequencyCommand(uint32_t frequency_Hz)
{
    // todo: check if the FY6900-100Mhz version has a frequency digit more and if so if it is backward compatible with the 14 digit commands 
    return sendScpiCommand("WFF%08u000000\n", frequency_Hz);
}

bool AwgFY6900::sendCh1AmplitudeCommand(uint32_t ampl_mV)
{
    return sendScpiCommand("WMA%02u.%03u\n", ampl_mV / 1000, ampl_mV % 1000);
}

bool AwgFY6900::sendCh2AmplitudeCommand(uint32_t ampl_mV)
{
    return sendScpiCommand("WFA%02u.%03u\n", ampl_mV / 1000, ampl_mV % 1000);
}

bool AwgFY6900::sendCh1PhaseCommand(uint32_t phase_deziDegree)
{
    return sendScpiCommand("WMP%03u.%03u\n", phase_deziDegree / 1000, (phase_deziDegree % 1000) / 100);
}

bool AwgFY6900::sendCh2PhaseCommand(uint32_t phase_deziDegree)
{
    return sendScpiCommand("WFP%03u.%03u\n", phase_deziDegree / 1000, (phase_deziDegree % 1000) / 100);
}

bool AwgFY6900::sendCh1OffsetCommand(int32_t offset_mV)
{
    return (offset_mV >= 0)
        ? sendScpiCommand("WMO%02u.%02u\n", offset_mV / 1000, offset_mV % 1000)
        : sendScpiCommand("WMO-%02u.%02u\n", -offset_mV / 1000, -offset_mV % 1000);
}

bool AwgFY6900::sendCh2OffsetCommand(int32_t offset_mV)
{
    return (offset_mV >= 0)
        ? sendScpiCommand("WFO%02u.%02u\n", offset_mV / 1000, offset_mV % 1000)
        : sendScpiCommand("WFO-%02u.%02u\n", -offset_mV / 1000, -offset_mV % 1000);
}
