#pragma once

#include "AwgDevice.h"

enum FY6800WaveType
{
    FY6800WaveType_Sine          = 0,
    FY6800WaveType_Square        = 1,
    FY6800WaveType_CMOS          = 2,
    FY6800WaveType_AdjPulse      = 3,
    FY6800WaveType_DC            = 4,
    FY6800WaveType_Triangle      = 5,
    FY6800WaveType_PosRamp       = 6,
    FY6800WaveType_NegRamp       = 7,
    FY6800WaveType_StairTrng     = 8,
    FY6800WaveType_PosStair      = 9,
    FY6800WaveType_NegStair      = 10,
    FY6800WaveType_PosExp        = 11,
    FY6800WaveType_NegExp        = 12,
    FY6800WaveType_PosFallExp    = 13,
    FY6800WaveType_NegFallExp    = 14,
    FY6800WaveType_PosLog        = 15,
    FY6800WaveType_NegLog        = 16,
    FY6800WaveType_PosFallLog    = 17,
    FY6800WaveType_NegFallLog    = 18,
    FY6800WaveType_PosFullSin    = 19,
    FY6800WaveType_NegFullSin    = 20,
    FY6800WaveType_PosHalfSin    = 21,
    FY6800WaveType_NegHalfSin    = 22,
    FY6800WaveType_Lorentz       = 23,
    FY6800WaveType_Multitone     = 24,
    FY6800WaveType_Noise         = 25,
    FY6800WaveType_ECG           = 26,
    FY6800WaveType_Trapezoid     = 27,
    FY6800WaveType_SincPulse     = 28,
    FY6800WaveType_Impulse       = 29,
    FY6800WaveType_AWGN          = 30,
    FY6800WaveType_AM            = 31,
    FY6800WaveType_FM            = 32,
    FY6800WaveType_Chirp         = 33,
    FY6800WaveType_UNDEFINED = 255
};

#include "AwgFY6900.h"

class AwgFY6800 : public AwgFY6900
{
public: 
    AwgFY6800(HardwareSerial *serial) : AwgFY6900(serial) {} 
};