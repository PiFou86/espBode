#include "AwgFy6800.h"

/*overwrite*/ AwgFY6900::FY6900WaveType AwgFY6800::MapWaveType(AwgDevice::WaveType waveType)
{
    auto awgWaveType = AwgFY6900::MapWaveType(waveType);
    if ((awgWaveType == AwgFY6900::WaveType_UNDEFINED) || ((int)awgWaveType >= (int)AwgFY6800::WaveType__LAST_))
    {
        // print Error:
        setCh1Output(0);
        setCh2Output(0);
        return FY6900WaveType::WaveType_UNDEFINED;
    }
    return awgWaveType;
}

/*
FY6800WaveType AwgFY6800::MapWaveType(AwgWaveType waveType)
{
    switch (waveType)
    {
    case AwgWaveType::AwgWaveType_Sine:
        return FY6800WaveType::WaveType_Sine;
    case AwgWaveType::AwgWaveType_Square:
        return FY6800WaveType::WaveType_Square;
    case AwgWaveType::AwgWaveType_Pulse:
        return FY6800WaveType::WaveType_Impulse; //??
    case AwgWaveType::AwgWaveType_Triangle:
        return FY6800WaveType::WaveType_Triangle;
    case AwgWaveType::AwgWaveType_DC:
        return FY6800WaveType::WaveType_DC;
    case AwgWaveType::AwgWaveType_Noise:
        return FY6800WaveType::WaveType_Noise;
    case AwgWaveType::AwgWaveType_Lorentz:
        return FY6800WaveType::WaveType_Lorentz;
    case AwgWaveType::AwgWaveType_PosStair:
        return FY6800WaveType::WaveType_PosStair;
    case AwgWaveType::AwgWaveType_NegStair:
        return FY6800WaveType::WaveType_NegStair;
    case AwgWaveType::AwgWaveType_ExpFall:
        return FY6800WaveType::WaveType_PosExp; //??
    case AwgWaveType::AwgWaveType_Partialsine:
        return FY6800WaveType::WaveType_PosHalfSin; //??
    case AwgWaveType::AwgWaveType_CMOS:
        return FY6800WaveType::WaveType_CMOS;
    default:
      break;
    }
    return FY6800WaveType::WaveType_UNDEFINED;
}

bool AwgFY6800::sendCh1WaveTypeCommand(AwgWaveType waveType)
{
    return sendDeviceCommand("WMW%02u\n", MapWaveType(waveType));
}

bool AwgFY6800::sendCh2WaveTypeCommand(AwgWaveType waveType)
{
    return sendDeviceCommand("WFW%02u\n", MapWaveType(waveType));
}

bool AwgFY6800::sendCh1OutputCommand(uint32_t output_OnOff)
{
    return sendDeviceCommand(output_OnOff ? "WMN1\n" : "WMN0\n");
}

bool AwgFY6800::sendCh2OutputCommand(uint32_t output_OnOff)
{
    return sendDeviceCommand(output_OnOff ? "WFN1\n" : "WFN0\n");
}

bool AwgFY6800::sendCh1FrequencyCommand(uint32_t frequency_Hz)
{
    // todo: check if the FY6900-100Mhz version has a frquency digit more and if so if it is backward compatible with the 14 digit commands 
    return sendDeviceCommand("WMF%08u000000\n", frequency_Hz);
}

bool AwgFY6800::sendCh2FrequencyCommand(uint32_t frequency_Hz)
{
    // todo: check if the FY6900-100Mhz version has a frequency digit more and if so if it is backward compatible with the 14 digit commands 
    return sendDeviceCommand("WFF%08u000000\n", frequency_Hz);
}

bool AwgFY6800::sendCh1AmplitudeCommand(uint32_t ampl_mV)
{
    return sendDeviceCommand("WMA%02u.%03u\n", ampl_mV / 1000, ampl_mV % 1000);
}

bool AwgFY6800::sendCh2AmplitudeCommand(uint32_t ampl_mV)
{
    return sendDeviceCommand("WFA%02u.%03u\n", ampl_mV / 1000, ampl_mV % 1000);
}

bool AwgFY6800::sendCh1PhaseCommand(uint32_t phase_deziDegree)
{
    return sendDeviceCommand("WMP%03u.%03u\n", phase_deziDegree / 1000, (phase_deziDegree % 1000) / 100);
}

bool AwgFY6800::sendCh2PhaseCommand(uint32_t phase_deziDegree)
{
    return sendDeviceCommand("WFP%03u.%03u\n", phase_deziDegree / 1000, (phase_deziDegree % 1000) / 100);
}

bool AwgFY6800::sendCh1OffsetCommand(int32_t offset_mV)
{
    return (offset_mV >= 0)
        ? sendDeviceCommand("WMO%02u.%02u\n", offset_mV / 1000, offset_mV % 1000)
        : sendDeviceCommand("WMO-%02u.%02u\n", -offset_mV / 1000, -offset_mV % 1000);
}

bool AwgFY6800::sendCh2OffsetCommand(int32_t offset_mV)
{
    return (offset_mV >= 0)
        ? sendDeviceCommand("WFO%02u.%02u\n", offset_mV / 1000, offset_mV % 1000)
        : sendDeviceCommand("WFO-%02u.%02u\n", -offset_mV / 1000, -offset_mV % 1000);
}
*/