#include "AWGProxy/Devices/AWGProxyFY6900.h"

#include "esp_config.h"

#define EWaveType_Sine         0

bool AWGProxyFY6900::writeCommandToSerial(char* data, uint8_t len)
{
    uint32_t timeout = 0;

    // debug output
    String dataStr(data);
    DEBUG("[\n" + dataStr + "]");

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
        DEBUG("Invalid response for command");
    }
    return ok;
}

bool AWGProxyFY6900::sendCommand(const char* cmdPattern, uint32_t param1, uint32_t param2)
{
    size_t cmdLen = snprintf(NULL, 0, cmdPattern, param1, param2);
    char* command = (char*) malloc(cmdLen+1); //reserve 1 additional byte for snpritf 0 termination character
    snprintf(command, cmdLen+1, cmdPattern, param1, param2);
    bool ok = writeCommandToSerial(command, cmdLen);
    free(command);
    return ok;
}

bool AWGProxyFY6900::sendCommand(const char* cmdPattern, uint32_t param1)
{
    // this kind of cmdPatterns must not contain exactly 1 place holder!
    // todo: add a check (search for exactly 1 '%')
    return sendCommand(cmdPattern, param1, 0);
}

bool AWGProxyFY6900::sendCommand(const char* cmdPattern)
{
    // this kind of cmdPatterns must not contain place holders!
    // todo: add a check (search for unexpected '%')
    return sendCommand(cmdPattern, 0, 0);
}

bool AWGProxyFY6900::setCh1Wave(uint8_t wave)
{
    _gDeviceState.ch1Wave = wave;
    return sendCommand("WMW%02u\n", wave);
}

bool AWGProxyFY6900::setCh2Wave(uint8_t wave)
{
    _gDeviceState.ch2Wave = wave;
    return sendCommand("WFW%02u\n", wave);
}

bool AWGProxyFY6900::setCh1Output(uint32_t output)
{
    _gDeviceState.ch1Output = output;
    return sendCommand(output ? "WMN1\n" : "WMN0\n");
}

bool AWGProxyFY6900::setCh2Output(uint32_t output)
{
    _gDeviceState.ch2Output = output;
    return sendCommand(output ? "WFN1\n" : "WFN0\n");
}

/* Set frequency in Hz */
bool AWGProxyFY6900::setCh1Freq(uint32_t frequency)
{
    _gDeviceState.ch1Freq = frequency;
    // todo: check if the FY6900-100Mhz version has a frquency digit more and if so if it is backward compatible with the 14 digit commands 
    return sendCommand("WMF%08u000000\n", frequency);
}

/* Set frequency in Hz */
bool AWGProxyFY6900::setCh2Freq(uint32_t frequency)
{
    _gDeviceState.ch2Freq = frequency;
    // todo: check if the FY6900-100Mhz version has a frequency digit more and if so if it is backward compatible with the 14 digit commands 
    return sendCommand("WFF%08u000000\n", frequency);
}

/* Ampl is in mV: 12.345V = 12345 */
bool AWGProxyFY6900::setCh1Ampl(uint32_t ampl)
{
    _gDeviceState.ch1Ampl = ampl;
    return sendCommand("WMA%02u.%03u\n", ampl/1000, ampl%1000);
}

bool AWGProxyFY6900::setCh2Ampl(uint32_t ampl)
{
    _gDeviceState.ch2Ampl = ampl;
    return sendCommand("WFA%02u.%03u\n", ampl/1000, ampl%1000);
}

/* Phase is in 0.1deg: 12.5deg = 125 */
bool AWGProxyFY6900::setCh1Phase(uint32_t phase)
{
    _gDeviceState.ch1Phase = phase;
    return sendCommand("WMP%03u.%03u\n", phase/1000, (phase%1000)/100);
}

bool AWGProxyFY6900::setCh2Phase(uint32_t phase)
{
    _gDeviceState.ch2Phase = phase;
    return sendCommand("WFP%03u.%03u\n", phase/1000, (phase%1000)/100);
}

bool AWGProxyFY6900::setCh1Offset(int32_t offset)
{
    _gDeviceState.ch1Offset = offset;
    return (offset>=0)
      ? sendCommand("WMO%02u.%02u\n", offset/1000, offset%1000)
      : sendCommand("WMO-%02u.%02u\n", -offset/1000, -offset%1000);
}

bool AWGProxyFY6900::setCh2Offset(int32_t offset)
{
    _gDeviceState.ch2Offset = offset;
    return (offset>=0)
      ? sendCommand("WFO%02u.%02u\n", offset/1000, offset%1000)
      : sendCommand("WFO-%02u.%02u\n", -offset/1000, -offset%1000);
}

bool AWGProxyFY6900::begin(void)
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

AWGProxyFY6900::~AWGProxyFY6900() {}