#pragma once

#include "AwgDevice.h"

class AwgFY6900 : public AwgDevice
{
public:
    enum FY6900WaveType
    {
        FY6900WaveType_Sine          = 0,
        FY6900WaveType_Square        = 1,
        FY6900WaveType_Rectangle     = 2,
        FY6900WaveType_Trapezoid     = 3,
        FY6900WaveType_CMOS          = 4,
        FY6900WaveType_AdjPulse      = 5,
        FY6900WaveType_DC            = 6,
        FY6900WaveType_Triangle      = 7,
        FY6900WaveType_PosRamp       = 8,
        FY6900WaveType_NegRamp       = 9,
        FY6900WaveType_StairTrng     = 10,
        FY6900WaveType_PosStair      = 11,
        FY6900WaveType_NegStair      = 12,
        FY6900WaveType_PosExp        = 13,
        FY6900WaveType_NegExp        = 14,
        FY6900WaveType_PosFallExp    = 15,
        FY6900WaveType_NegFallExp    = 16,
        FY6900WaveType_PosLog        = 17,
        FY6900WaveType_NegLog        = 18,
        FY6900WaveType_PosFallLog    = 19,
        FY6900WaveType_NegFallLog    = 20,
        FY6900WaveType_PosFullSin    = 21,
        FY6900WaveType_NegFullSin    = 22,
        FY6900WaveType_PosHalfSin    = 23,
        FY6900WaveType_NegHalfSin    = 24,
        FY6900WaveType_Lorentz       = 25,
        FY6900WaveType_Multitone     = 26,
        FY6900WaveType_Noise         = 27,
        FY6900WaveType_ECG           = 28,
        FY6900WaveType_Trapezoid2    = 29,
        FY6900WaveType_SincPulse     = 30,
        FY6900WaveType_Impulse       = 31,
        FY6900WaveType_AWGN          = 32,
        FY6900WaveType_AM            = 33,
        FY6900WaveType_FM            = 34,
        FY6900WaveType_Chirp         = 35,
        FY6900WaveType_Impulse2      = 36,
        
        FY6900WaveType__LAST_,
        FY6900WaveType_UNDEFINED = 255
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

protected:
    FY6900WaveType MapWaveType(WaveType waveType);
};