#include "AwgFy6800.h"

/*overwrite*/ AwgFY6900::FY6900WaveType AwgFY6800::MapWaveType(IAwgDevice::WaveType waveType)
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
