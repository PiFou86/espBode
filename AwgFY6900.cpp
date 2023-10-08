#include "AwgFy6900.h"

AwgFY6900::FY6900WaveType AwgFY6900::MapWaveType(AwgDevice::WaveType waveType)
{
    switch (waveType)
    {
    case AwgDevice::WaveType_Sine:
        return WaveType_Sine;
    case AwgDevice::WaveType_Square:
        return WaveType_Square;
    case AwgDevice::WaveType_Pulse:
        return WaveType_Impulse; //??
    case AwgDevice::WaveType_Triangle:
        return WaveType_Triangle;
    case AwgDevice::WaveType_DC:
        return WaveType_DC;
    case AwgDevice::WaveType_Noise:
        return WaveType_Noise;
    case AwgDevice::WaveType_Lorentz:
        return WaveType_Lorentz;
    case AwgDevice::WaveType_PosStair:
        return WaveType_PosStair;
    case AwgDevice::WaveType_NegStair:
        return WaveType_NegStair;
    case AwgDevice::WaveType_ExpFall:
        return WaveType_PosExp; //??
    case AwgDevice::WaveType_Partialsine:
        return WaveType_PosHalfSin; //??
    case AwgDevice::WaveType_CMOS:
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

/* 
>>> before the refactoring of send commands:
Executable segment sizes:
IROM   : 251124          - code in flash         (default or ICACHE_FLASH_ATTR) 
IRAM   : 27324   / 32768 - code in IRAM          (ICACHE_RAM_ATTR, ISRs...) 
DATA   : 1252  )         - initialized variables (global, static) in RAM/HEAP 
RODATA : 1372  ) / 81920 - constants             (global, static) in RAM/HEAP 
BSS    : 25248 )         - zeroed variables      (global, static) in RAM/HEAP 
Sketch uses 281072 bytes (29%) of program storage space. Maximum is 958448 bytes.
Global variables use 27872 bytes (34%) of dynamic memory, leaving 54048 bytes for local variables. Maximum is 81920 bytes.

>>> after the refactoring of send commands:
Executable segment sizes:
IROM   : 250900          - code in flash         (default or ICACHE_FLASH_ATTR) 
IRAM   : 27324   / 32768 - code in IRAM          (ICACHE_RAM_ATTR, ISRs...) 
DATA   : 1252  )         - initialized variables (global, static) in RAM/HEAP 
RODATA : 1256  ) / 81920 - constants             (global, static) in RAM/HEAP 
BSS    : 25240 )         - zeroed variables      (global, static) in RAM/HEAP 
Sketch uses 280732 bytes (29%) of program storage space. Maximum is 958448 bytes.
Global variables use 27748 bytes (33%) of dynamic memory, leaving 54172 bytes for local variables. Maximum is 81920 bytes.

Executable segment sizes:
IROM   : 250900          - code in flash         (default or ICACHE_FLASH_ATTR) 
IRAM   : 27324   / 32768 - code in IRAM          (ICACHE_RAM_ATTR, ISRs...) 
DATA   : 1252  )         - initialized variables (global, static) in RAM/HEAP 
RODATA : 1232  ) / 81920 - constants             (global, static) in RAM/HEAP 
BSS    : 25248 )         - zeroed variables      (global, static) in RAM/HEAP 
Sketch uses 280708 bytes (29%) of program storage space. Maximum is 958448 bytes.
Global variables use 27732 bytes (33%) of dynamic memory, leaving 54188 bytes for local variables. Maximum is 81920 bytes.

Using library ESP8266WiFi at version 1.0 in folder: C:\Users\Steffen\AppData\Local\Arduino15\packages\esp8266\hardware\esp8266\2.7.4\libraries\ESP8266WiFi 
Using library ESP Telnet at version 2.0.0 in folder: C:\Users\Steffen\OneDrive\Documents\Arduino\libraries\ESP_Telnet 
Using library ESP8266WebServer at version 1.0 in folder: C:\Users\Steffen\AppData\Local\Arduino15\packages\esp8266\hardware\esp8266\2.7.4\libraries\ESP8266WebServer
*/

