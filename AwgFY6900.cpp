#include "AwgFy6900.h"

FY6900WaveType AwgFY6900::MapWaveType(AwgWaveType waveType)
{
    switch (waveType)
    {
    case AwgWaveType::AwgWaveType_Sine:
        return FY6900WaveType::FY6900WaveType_Sine;
    case AwgWaveType::AwgWaveType_Square:
        return FY6900WaveType::FY6900WaveType_Square;
    case AwgWaveType::AwgWaveType_Pulse:
        return FY6900WaveType::FY6900WaveType_Impulse; //??
    case AwgWaveType::AwgWaveType_Triangle:
        return FY6900WaveType::FY6900WaveType_Triangle;
    case AwgWaveType::AwgWaveType_DC:
        return FY6900WaveType::FY6900WaveType_DC;
    case AwgWaveType::AwgWaveType_Noise:
        return FY6900WaveType::FY6900WaveType_Noise;
    case AwgWaveType::AwgWaveType_Lorentz:
        return FY6900WaveType::FY6900WaveType_Lorentz;
    case AwgWaveType::AwgWaveType_PosStair:
        return FY6900WaveType::FY6900WaveType_PosStair;
    case AwgWaveType::AwgWaveType_NegStair:
        return FY6900WaveType::FY6900WaveType_NegStair;
    case AwgWaveType::AwgWaveType_ExpFall:
        return FY6900WaveType::FY6900WaveType_PosExp; //??
    case AwgWaveType::AwgWaveType_Partialsine:
        return FY6900WaveType::FY6900WaveType_PosHalfSin; //??
    case AwgWaveType::AwgWaveType_CMOS:
        return FY6900WaveType::FY6900WaveType_CMOS;
    default:
      break;
    }
    return FY6900WaveType::FY6900WaveType_UNDEFINED;
}

bool AwgFY6900::sendCh1WaveTypeCommand(AwgWaveType waveType)
{
    return sendDeviceCommand("WMW%02u\n", MapWaveType(waveType));
}

bool AwgFY6900::sendCh2WaveTypeCommand(AwgWaveType waveType)
{
    return sendDeviceCommand("WFW%02u\n", MapWaveType(waveType));
}

bool AwgFY6900::sendCh1OutputCommand(uint32_t output_OnOff)
{
    return sendDeviceCommand(output_OnOff ? "WMN1\n" : "WMN0\n");
}

bool AwgFY6900::sendCh2OutputCommand(uint32_t output_OnOff)
{
    return sendDeviceCommand(output_OnOff ? "WFN1\n" : "WFN0\n");
}

bool AwgFY6900::sendCh1FrequencyCommand(uint32_t frequency_Hz)
{
    // todo: check if the FY6900-100Mhz version has a frquency digit more and if so if it is backward compatible with the 14 digit commands 
    return sendDeviceCommand("WMF%08u000000\n", frequency_Hz);
}

bool AwgFY6900::sendCh2FrequencyCommand(uint32_t frequency_Hz)
{
    // todo: check if the FY6900-100Mhz version has a frequency digit more and if so if it is backward compatible with the 14 digit commands 
    return sendDeviceCommand("WFF%08u000000\n", frequency_Hz);
}

bool AwgFY6900::sendCh1AmplitudeCommand(uint32_t ampl_mV)
{
    return sendDeviceCommand("WMA%02u.%03u\n", ampl_mV / 1000, ampl_mV % 1000);
}

bool AwgFY6900::sendCh2AmplitudeCommand(uint32_t ampl_mV)
{
    return sendDeviceCommand("WFA%02u.%03u\n", ampl_mV / 1000, ampl_mV % 1000);
}

bool AwgFY6900::sendCh1PhaseCommand(uint32_t phase_deziDegree)
{
    return sendDeviceCommand("WMP%03u.%03u\n", phase_deziDegree / 1000, (phase_deziDegree % 1000) / 100);
}

bool AwgFY6900::sendCh2PhaseCommand(uint32_t phase_deziDegree)
{
    return sendDeviceCommand("WFP%03u.%03u\n", phase_deziDegree / 1000, (phase_deziDegree % 1000) / 100);
}

bool AwgFY6900::sendCh1OffsetCommand(int32_t offset_mV)
{
    return (offset_mV >= 0)
        ? sendDeviceCommand("WMO%02u.%02u\n", offset_mV / 1000, offset_mV % 1000)
        : sendDeviceCommand("WMO-%02u.%02u\n", -offset_mV / 1000, -offset_mV % 1000);
}

bool AwgFY6900::sendCh2OffsetCommand(int32_t offset_mV)
{
    return (offset_mV >= 0)
        ? sendDeviceCommand("WFO%02u.%02u\n", offset_mV / 1000, offset_mV % 1000)
        : sendDeviceCommand("WFO-%02u.%02u\n", -offset_mV / 1000, -offset_mV % 1000);
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

