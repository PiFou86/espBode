#pragma once

#include "AwgDevice.h"

class AwgFY6900 : public AwgDevice
{
public:
    enum FY6900WaveType
    {
        WaveType_Sine          = 0,
        WaveType_Square        = 1,
        WaveType_Rectangle     = 2,
        WaveType_Trapezoid     = 3,
        WaveType_CMOS          = 4,
        WaveType_AdjPulse      = 5,
        WaveType_DC            = 6,
        WaveType_Triangle      = 7,
        WaveType_PosRamp       = 8,
        WaveType_NegRamp       = 9,
        WaveType_StairTrng     = 10,
        WaveType_PosStair      = 11,
        WaveType_NegStair      = 12,
        WaveType_PosExp        = 13,
        WaveType_NegExp        = 14,
        WaveType_PosFallExp    = 15,
        WaveType_NegFallExp    = 16,
        WaveType_PosLog        = 17,
        WaveType_NegLog        = 18,
        WaveType_PosFallLog    = 19,
        WaveType_NegFallLog    = 20,
        WaveType_PosFullSin    = 21,
        WaveType_NegFullSin    = 22,
        WaveType_PosHalfSin    = 23,
        WaveType_NegHalfSin    = 24,
        WaveType_Lorentz       = 25,
        WaveType_Multitone     = 26,
        WaveType_Noise         = 27,
        WaveType_ECG           = 28,
        WaveType_Trapezoid2    = 29,
        WaveType_SincPulse     = 30,
        WaveType_Impulse       = 31,
        WaveType_AWGN          = 32,
        WaveType_AM            = 33,
        WaveType_FM            = 34,
        WaveType_Chirp         = 35,
        WaveType_Impulse2      = 36,
        
        WaveType__LAST_,
        WaveType_UNDEFINED = 255
    };

public: 
    AwgFY6900(HardwareSerial *serial) : AwgDevice(serial) {}
    
protected:
    /* WaveType is in AwgDevice::WaveType (needs to be mapped of actual device wave type IDs) */
    virtual bool sendCh1WaveTypeCommand(WaveType waveType);
    virtual bool sendCh2WaveTypeCommand(WaveType waveType);
    /* Output is boolean:  1="on" 0="off" */
    virtual bool sendCh1OutputCommand(uint32_t output_OnOff);
    virtual bool sendCh2OutputCommand(uint32_t output_OnOff);
    /* Frequency is in Hz: 12.345kHz = 12345 */
    virtual bool sendCh1FrequencyCommand(uint32_t frequency_Hz);
    virtual bool sendCh2FrequencyCommand(uint32_t frequency_Hz);
    /* Amplitude is in mV: 12.345V = 12345 */
    virtual bool sendCh1AmplitudeCommand(uint32_t ampl_mV);
    virtual bool sendCh2AmplitudeCommand(uint32_t ampl_mV);
    /* Phase is in 0.1deg: 12.5deg = 125 */
    virtual bool sendCh1PhaseCommand(uint32_t phase_deziDegree);
    virtual bool sendCh2PhaseCommand(uint32_t phase_deziDegree);
    /* Offset is in mV:    12.345V = 12345 */
    virtual bool sendCh1OffsetCommand(int32_t offset_mV);
    virtual bool sendCh2OffsetCommand(int32_t offset_mV);

    FY6900WaveType MapWaveType(WaveType waveType);
};