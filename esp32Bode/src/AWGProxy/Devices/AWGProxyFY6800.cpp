#include "AWGProxy/Devices/AWGProxyFY6800.h"

#define EWaveType_Sine         0
void AWGProxyFY6800::write(char* data, uint8_t len)
{
    uint32_t timeout = 0;
    Serial.write((uint8_t*)data, len);
    while(!Serial.available())
    {
        delay(1);
        if(timeout++>1000) return;
    }
    (void)Serial.read();
}

bool AWGProxyFY6800::setCh1Wave(uint8_t wave)
{
    char command[] = "WMW00\n";
    snprintf(command, 7, "WMW%02u\n", wave);
    _gDeviceState.ch1Wave = wave;
    write(command, 6);

    return true;
}

bool AWGProxyFY6800::setCh2Wave(uint8_t wave)
{
    char command[] = "WFW00\n";
    snprintf(command, 7, "WFW%02u\n", wave);
    _gDeviceState.ch2Wave = wave;
    write(command, 6);

    return true;
}

bool AWGProxyFY6800::setCh1Output(uint32_t output)
{
    if(output)
    {
        _gDeviceState.ch1Output = 1;
        write((char*)"WMN1\n", 5);
    }
    else
    {
        _gDeviceState.ch1Output = 0;
        write((char*)"WMN0\n", 5);
    }

    return true;
}

bool AWGProxyFY6800::setCh2Output(uint32_t output)
{
    if(output)
    {
        _gDeviceState.ch2Output = 1;
        write((char*)"WFN1\n", 5);
    }
    else
    {
        _gDeviceState.ch2Output = 0;
        write((char*)"WFN0\n", 5);
    }

    return true;
}

/* Set frequency in Hz */
bool AWGProxyFY6800::setCh1Freq(uint32_t frequency)
{
    char command[] = "WMF00000000000000\n";
    snprintf(command, 19, "WMF%08lu000000\n", frequency);
    _gDeviceState.ch1Freq = frequency;
    write(command, 18);

    return true;
}

/* Set frequency in Hz */
bool AWGProxyFY6800::setCh2Freq(uint32_t frequency)
{
    char command[] = "WFF00000000000000\n";
    snprintf(command, 19, "WFF%08lu000000\n", frequency);
    _gDeviceState.ch2Freq = frequency;
    write(command, 18);

    return true;
}

/* Ampl is in mV: 12.345V = 12345 */
bool AWGProxyFY6800::setCh1Ampl(uint32_t ampl)
{
    char command[] = "WMA00.000\n";
    snprintf(command, 11, "WMA%02u.%03u\n", ampl/1000, ampl%1000);
    _gDeviceState.ch1Ampl = ampl;
    write(command, 10);

    return true;
}

bool AWGProxyFY6800::setCh2Ampl(uint32_t ampl)
{
    char command[] = "WFA00.000\n";
    snprintf(command, 11, "WFA%02u.%03u\n", ampl/1000, ampl%1000);
    _gDeviceState.ch2Ampl = ampl;
    write(command, 10);

    return true;
}

/* Phase is in 0.1deg: 12.5deg = 125 */
bool AWGProxyFY6800::setCh1Phase(uint32_t phase)
{
    char command[] = "WMP00.000\n";
    snprintf(command, 11, "WMP%02u.%01u\n", phase/1000, (phase%1000)/100);
    _gDeviceState.ch1Phase = phase;
    write(command, 10);

    return true;
}

bool AWGProxyFY6800::setCh2Phase(uint32_t phase)
{
    char command[] = "WFP00.000\n";
    snprintf(command, 11, "WFP%02u.%01u\n", phase/1000, (phase%1000)/100);
    _gDeviceState.ch2Phase = phase;
    write(command, 10);

    return true;
}

bool AWGProxyFY6800::setCh1Offset(int32_t offset)
{
    char command[] = "WMO00.000\n";
    _gDeviceState.ch1Offset = offset;
    if(offset>=0)
    {
        snprintf(command, 11, "WMO%02u.%03u\n", offset/1000, offset%1000);
        write(command, 10);
    }
    else
    {
        snprintf(command, 12, "WMO-%02u.%03u\n", -offset/1000, -offset%1000);
        write(command, 11);
    }

    return true;
}

bool AWGProxyFY6800::setCh2Offset(int32_t offset)
{
    char command[] = "WFO00.000\n";
    _gDeviceState.ch2Offset = offset;
    if(offset>=0)
    {
        snprintf(command, 11, "WFO%02u.%03u\n", offset/1000, offset%1000);
        write(command, 10);
    }
    else
    {
       snprintf(command, 12, "WFO-%02u.%03u\n", -offset/1000, -offset%1000);
       write(command, 11);
   }

   return true;
}

bool AWGProxyFY6800::begin(void)
{
    Serial.write((uint8_t*)"\n", 1);

    setCh1Output(0);
    setCh1Wave(EWaveType_Sine);
    setCh1Freq(1000);
    setCh1Ampl(1000);
    setCh1Offset(0);

    setCh2Output(0);
    setCh2Wave(EWaveType_Sine);
    setCh2Freq(1000);
    setCh2Ampl(1000);
    setCh2Offset(0);

    return true;
}

AWGProxyFY6800::~AWGProxyFY6800() {}