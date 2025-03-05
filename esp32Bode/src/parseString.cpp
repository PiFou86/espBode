#include "parseString.h"

/* Function parses number from the msg string expects natural a number. */
uint32_t parseNumber(char *msg)
{
    uint32_t number = 0;
    while(0<=(msg[0]-'0') && (msg[0]-'0')<=9)
    {
        number*=10;
        number+=msg[0]-'0';
        msg++;
    }
    return number;
}

/* Similar to parseNumber, but handles also decimal '.' and '-' sign.0
Return value is multiplied *1000 to include the decimal part:
    123.345 -> 123456
    -1.2    ->  -1200 */
int32_t parseDecimal(char *msg)
{
    uint8_t dot = 0;
    int32_t multiplier = 1000;
    int32_t number = 0;

    if(msg[0] == '-')
    {
        multiplier*=-1;
        msg++;
    }

    while((0<=(msg[0]-'0') && (msg[0]-'0')<=9) || msg[0] == '.')
    {
        if(msg[0] == '.')
        {
            dot = 1;
            msg++;
            continue;
        }
        if(dot) multiplier/=10;
        number*=10;
        number+=msg[0]-'0';
        msg++;
    }
    return number*multiplier;
}
