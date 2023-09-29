#include "esp_config.h"
#include <string.h>

#if AWG == FY6900
#warning Compiling for FY6900

#include "esp_fy6900.h"

volatile SDeviceState gDeviceState;

bool writeCommandToSerial(char* data, uint8_t len)
{
    uint32_t timeout = 0;

    // some debug output 
    // todo: clarify where does this telnet output goes to? might this disturb the FY6900?)
    telnet.println("[");
    telnet.print(data);
    telnet.println("]");

    // write command
    Serial.write((uint8_t*)data, len);
    
    // wait for device acknowledge response
    while(!Serial.available())
    {
        delay(1);
        if(timeout++>1000) return false;
    }
    bool ok = false;
    ok = (Serial.read() == 0x0a); // 0x0a == \n

    if(!ok){
      telnet.println("Invalid response for command");      
    }
    return ok;
}

bool sendCommand(const char* cmdPattern, uint32_t param1, uint32_t param2)
{
    size_t cmdLen = snprintf(NULL, 0, cmdPattern, param1, param2);
    char* command = (char*) malloc(cmdLen+1); //reserve 1 additional byte for snpritf 0 termination character
    snprintf(command, cmdLen+1, cmdPattern, param1, param2);
    bool ok = writeCommandToSerial(command, cmdLen);
    free(command);
    return ok;
}

bool sendCommand(const char* cmdPattern, uint32_t param1)
{
    // this kind of cmdPatterns must not contain exactly 1 place holder!
    // todo: add a check (search for exactly 1 '%')
    return sendCommand(cmdPattern, param1, 0);
}

bool sendCommand(const char* cmdPattern)
{
    // this kind of cmdPatterns must not contain place holders!
    // todo: add a check (search for unexpected '%')
    return sendCommand(cmdPattern, 0, 0);
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
*/

bool setCh1Wave(EWaveType wave)
{
    gDeviceState.ch1Wave = wave;
    return sendCommand("WMW%02u\n", wave);
}

bool setCh2Wave(EWaveType wave)
{
    gDeviceState.ch2Wave = wave;
    return sendCommand("WFW%02u\n", wave);
}

bool setCh1Output(uint32_t output)
{
    gDeviceState.ch1Output = output;
    return sendCommand(output ? "WMN1\n" : "WMN0\n");
}

bool setCh2Output(uint32_t output)
{
    gDeviceState.ch2Output = output;
    return sendCommand(output ? "WFN1\n" : "WFN0\n");
}

/* Set frequency in Hz */
bool setCh1Freq(uint32_t frequency)
{
    gDeviceState.ch1Freq = frequency;
    // todo: check if the FY6900-100Mhz version has a frquency digit more and if so if it is backward compatible with the 14 digit commands 
    return sendCommand("WMF%08u000000\n", frequency);
}

/* Set frequency in Hz */
bool setCh2Freq(uint32_t frequency)
{
    gDeviceState.ch2Freq = frequency;
    // todo: check if the FY6900-100Mhz version has a frequency digit more and if so if it is backward compatible with the 14 digit commands 
    return sendCommand("WFF%08u000000\n", frequency);
}

/* Ampl is in mV: 12.345V = 12345 */
bool setCh1Ampl(uint32_t ampl)
{
    gDeviceState.ch1Ampl = ampl;
    return sendCommand("WMA%02u.%03u\n", ampl/1000, ampl%1000);
}

bool setCh2Ampl(uint32_t ampl)
{
    gDeviceState.ch2Ampl = ampl;
    return sendCommand("WFA%02u.%03u\n", ampl/1000, ampl%1000);
}

/* Phase is in 0.1deg: 12.5deg = 125 */
bool setCh1Phase(uint32_t phase)
{
    gDeviceState.ch1Phase = phase;
    return sendCommand("WMP%03u.%03u\n", phase/1000, (phase%1000)/100);
}

bool setCh2Phase(uint32_t phase)
{
    gDeviceState.ch2Phase = phase;
    return sendCommand("WFP%03u.%03u\n", phase/1000, (phase%1000)/100);
}

bool setCh1Offset(int32_t offset)
{
    gDeviceState.ch1Offset = offset;
    return (offset>=0)
      ? sendCommand("WMO%02u.%02u\n", offset/1000, offset%1000)
      : sendCommand("WMO-%02u.%02u\n", -offset/1000, -offset%1000);
}

bool setCh2Offset(int32_t offset)
{
    gDeviceState.ch2Offset = offset;
    return (offset>=0)
      ? sendCommand("WFO%02u.%02u\n", offset/1000, offset%1000)
      : sendCommand("WFO-%02u.%02u\n", -offset/1000, -offset%1000);
}

bool initDevice(void)
{
    Serial.write((uint8_t*)"\n", 1);

    return
         setCh1Output(0)
      && setCh1Wave(EWaveType_Sine)
      && setCh1Freq(1000)
      && setCh1Ampl(1000)
      && setCh1Offset(0)

      && setCh2Output(0)
      && setCh2Wave(EWaveType_Sine)
      && setCh2Freq(1000)
      && setCh2Ampl(1000)
      && setCh2Offset(0);
}

#endif
