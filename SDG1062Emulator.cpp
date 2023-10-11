#include "SDG1062Emulator.h"
#include "Interfaces/IAwgDevice.h" //class IAwgDevice
#include "Interfaces/LxiDeviceConfig.h" //class LxiDeviceConfig
#include <string.h> //strncmp,...
#include <string> //std::string

SDG1062Emulator::SDG1062Emulator(IAwgDevice* targetAwgDevice)
{
    _awgDevice = targetAwgDevice;
    
    // init Lxi Device Configuration:
    _lxiConfig = new LxiDeviceConfig();
    _lxiConfig->deviceID = "IDN-SGLT-PRI SDG1062X\n";
    _lxiConfig->rpcServerPort = 111;
    _lxiConfig->lxiServerPort = 703;
}

LxiDeviceConfig* SDG1062Emulator::lxiConfig()
{
    return _lxiConfig;
}

const char* SDG1062Emulator::lastScpiCommand()
{
    return _lastCommand.c_str();
}

const char* SDG1062Emulator::lastScpiReply()
{
    return _lastReply.c_str();
}

bool SDG1062Emulator::sendScpiCommand(const char* cmdPattern)
{
    _lastCommand = cmdPattern;
    _lastReply.clear();
    
    // execute SCPI command
    return handleWriteMsg(_lastCommand.c_str(), _lastCommand.size());
}


//-----------------------------------------------------------------------------------
// from esp_parser.cpp

uint32_t SDG1062Emulator::parseNumber(const char* msg)
{
    uint32_t number = 0;
    while (0 <= (msg[0] - '0') && (msg[0] - '0') <= 9)
    {
        number *= 10;
        number += msg[0] - '0';
        msg++;
    }
    return number;
}

/* Similar to parseNumber, but handles also decimal '.' and '-' sign.0
Return value is multiplied *1000 to include the decimal part:
    123.345 -> 123456
    -1.2    ->  -1200 */
int32_t SDG1062Emulator::parseDecimal(const char* msg)
{
    uint8_t dot = 0;
    int32_t multiplier = 1000;
    int32_t number = 0;

    if (msg[0] == '-')
    {
        multiplier *= -1;
        msg++;
    }

    while ((0 <= (msg[0] - '0') && (msg[0] - '0') <= 9) || msg[0] == '.')
    {
        if (msg[0] == '.')
        {
            dot = 1;
            msg++;
            continue;
        }
        if (dot) multiplier /= 10;
        number *= 10;
        number += msg[0] - '0';
        msg++;
    }
    return number * multiplier;
}

bool SDG1062Emulator::handleWriteMsg(const char* msg, uint8_t len)
{
    uint8_t selectedChannel = 1;
    bool ok = true;
    while (len > 0)
    {
        /* ID request message, we preapare answer in _readBuffer */
        if (0 == strncmp(msg, "IDN-SGLT-PRI?", 13))
        {
            _lastReply = _lxiConfig->deviceID;
            return true;
        }

        if (0 == strncmp(msg, "C1:", 3))
        {
            selectedChannel = 1;
            msg += 3;
            len -= 3;
        }

        if (0 == strncmp(msg, "C2:", 3))
        {
            selectedChannel = 2;
            msg += 3;
            len -= 3;
        }

        if (0 == strncmp(msg, "BSWV WVTP,", 10))
        {
            msg += 10;
            len -= 10;

            if (0 == strncmp(msg, "SINE,", 5))
            {
                msg += 5;
                len -= 5;
                ok &= selectedChannel == 1
                    ? _awgDevice->setCh1WaveType(IAwgDevice::WaveType_Sine)
                    : _awgDevice->setCh2WaveType(IAwgDevice::WaveType_Sine);
            }
        }

        if (0 == strncmp(msg, "PHSE,", 5))
        {
            msg += 5;
            len -= 5;
            ok &= selectedChannel == 1
                ? _awgDevice->setCh1Phase(parseNumber(msg))
                : _awgDevice->setCh2Phase(parseNumber(msg));
        }

        if (0 == strncmp(msg, "FRQ,", 4))
        {
            msg += 4;
            len -= 4;
            ok &= selectedChannel == 1
                ? _awgDevice->setCh1Freq(parseNumber(msg))
                : _awgDevice->setCh2Freq(parseNumber(msg));
        }

        if (0 == strncmp(msg, "AMP,", 4))
        {
            msg += 4;
            len -= 4;
            ok &= selectedChannel == 1
                ? _awgDevice->setCh1Ampl(parseDecimal(msg))
                : _awgDevice->setCh2Ampl(parseDecimal(msg));
        }

        if (0 == strncmp(msg, "OFST,", 5))
        {
            msg += 5;
            len -= 5;
            ok &= selectedChannel == 1
                ? _awgDevice->setCh1Offset(parseDecimal(msg))
                : _awgDevice->setCh2Offset(parseDecimal(msg));
        }

        if (0 == strncmp(msg, "OUTP ON", 7))
        {
            msg += 7;
            len -= 7;
            ok &= selectedChannel == 1
                ? _awgDevice->setCh1Output(1)
                : _awgDevice->setCh2Output(1);
        }

        if (0 == strncmp(msg, "OUTP OFF", 8))
        {
            msg += 8;
            len -= 8;
            ok &= selectedChannel == 1
                ? _awgDevice->setCh1Output(0)
                : _awgDevice->setCh2Output(0);
        }

        /* Unknown string, skip one character */
        msg++;
        len--;
    }
    return ok;
}
