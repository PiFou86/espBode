#pragma once

#include "AwgFY6900.h"

class AwgFY6800 : public AwgFY6900
{
public:  
    enum FY6800WaveType
    {
        WaveType_Sine          = 0,
        WaveType_Square        = 1,
        WaveType_CMOS          = 2,
        WaveType_AdjPulse      = 3,
        WaveType_DC            = 4,
        WaveType_Triangle      = 5,
        WaveType_PosRamp       = 6,
        WaveType_NegRamp       = 7,
        WaveType_StairTrng     = 8,
        WaveType_PosStair      = 9,
        WaveType_NegStair      = 10,
        WaveType_PosExp        = 11,
        WaveType_NegExp        = 12,
        WaveType_PosFallExp    = 13,
        WaveType_NegFallExp    = 14,
        WaveType_PosLog        = 15,
        WaveType_NegLog        = 16,
        WaveType_PosFallLog    = 17,
        WaveType_NegFallLog    = 18,
        WaveType_PosFullSin    = 19,
        WaveType_NegFullSin    = 20,
        WaveType_PosHalfSin    = 21,
        WaveType_NegHalfSin    = 22,
        WaveType_Lorentz       = 23,
        WaveType_Multitone     = 24,
        WaveType_Noise         = 25,
        WaveType_ECG           = 26,
        WaveType_Trapezoid     = 27,
        WaveType_SincPulse     = 28,
        WaveType_Impulse       = 29,
        WaveType_AWGN          = 30,
        WaveType_AM            = 31,
        WaveType_FM            = 32,
        WaveType_Chirp         = 33,

        WaveType__LAST_,
        WaveType_UNDEFINED = 255
    };

public: 
    AwgFY6800(HardwareSerial *serial) : AwgFY6900(serial) {}

protected:
    FY6900WaveType MapWaveType(WaveType waveType);    
};