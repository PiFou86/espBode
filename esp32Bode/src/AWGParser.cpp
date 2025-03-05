#include "AWGParser.h"

#include <WiFi.h>

#include "esp_config.h" 

#include "parseString.h"

#define EWaveType_Sine        0

void AWGParser::handleWriteMsg(char *msg, uint8_t len)
{
    uint8_t selectedChannel = 1;
    while(len>0)
    {
        /* ID request message, we preapare answer in gReadBuffer */
        if(0 == strncmp(msg, "IDN-SGLT-PRI?", 13))
        {
            if(_gReadBuffer != NULL) free((void*)_gReadBuffer); /* Prevent memory leaks */
            _gReadBuffer = (char*)malloc(strlen(ID)+1);
            strcpy((char*)_gReadBuffer, ID);
            break;
        }

        if(0 == strncmp(msg, "C1:", 3))
        {
            selectedChannel = 1;
            msg+=3;
            len-=3;
        }

        if(0 == strncmp(msg, "C2:", 3))
        {
            selectedChannel = 2;
            msg+=3;
            len-=3;
        }

        if(0 == strncmp(msg, "BSWV WVTP,", 10))
        {
            msg+=10;
            len-=10;

            if(0 == strncmp(msg, "SINE,",5))
            {
                msg+=5;
                len-=5;
                selectedChannel==1?_awgProxy->setCh1Wave(EWaveType_Sine):_awgProxy->setCh2Wave(EWaveType_Sine);
            }
        }

        if(0 == strncmp(msg, "PHSE,", 5))
        {
            msg+=5;
            len-=5;
            selectedChannel==1?_awgProxy->setCh1Phase(parseNumber(msg)):_awgProxy->setCh2Phase(parseNumber(msg));
        }

        if(0 == strncmp(msg, "FRQ,", 4))
        {
            msg+=4;
            len-=4;
            selectedChannel==1?_awgProxy->setCh1Freq(parseNumber(msg)):_awgProxy->setCh2Freq(parseNumber(msg));
        }

        if(0 == strncmp(msg, "AMP,", 4))
        {
            msg+=4;
            len-=4;
            selectedChannel==1?_awgProxy->setCh1Ampl(parseDecimal(msg)):_awgProxy->setCh2Ampl(parseDecimal(msg));
        }

        if(0 == strncmp(msg, "OFST,", 5))
        {
            msg+=5;
            len-=5;
            selectedChannel==1?_awgProxy->setCh1Offset(parseDecimal(msg)):_awgProxy->setCh2Offset(parseDecimal(msg));
        }

        if(0 == strncmp(msg, "OUTP ON", 7))
        {
            msg+=7;
            len-=7;
            selectedChannel==1?_awgProxy->setCh1Output(1):_awgProxy->setCh2Output(1);
        }

        if(0 == strncmp(msg, "OUTP OFF", 8))
        {
            msg+=8;
            len-=8;
            selectedChannel==1?_awgProxy->setCh1Output(0):_awgProxy->setCh2Output(0);
        }

        /* Unknown string, skip one character */
        msg++;
        len--;
    }
}
