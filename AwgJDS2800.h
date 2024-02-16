#pragma once

#include "AwgSerialDeviceBase.h"

enum JDS2800WaveType
{
    JDS2800WaveType_Sine          = 0,
    JDS2800WaveType_Square        = 1,
	  JDS2800WaveType_Pulse         = 2,
    JDS2800WaveType_Triangle      = 3,
    JDS2800WaveType_Partialsine   = 4,
    JDS2800WaveType_CMOS          = 5,
    JDS2800WaveType_DC            = 6,
    JDS2800WaveType_Halfwave      = 7,
    JDS2800WaveType_Fullwave      = 8,
    JDS2800WaveType_PosStair      = 9,
    JDS2800WaveType_NegStair      = 10,
    JDS2800WaveType_Noise         = 11,
    JDS2800WaveType_IndexRise     = 12,
    JDS2800WaveType_ExpFall       = 13,
    JDS2800WaveType_ForSonic      = 14,
    JDS2800WaveType_SieckPulse    = 15,
    JDS2800WaveType_Lorentz       = 16,
};

