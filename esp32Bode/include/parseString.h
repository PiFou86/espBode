#pragma once

#include <Arduino.h>

/* Function parses number from the msg string expects natural a number. */
uint32_t parseNumber(char *msg);

/* Similar to parseNumber, but handles also decimal '.' and '-' sign.0
Return value is multiplied *1000 to include the decimal part:
    123.345 -> 123456
    -1.2    ->  -1200 */
int32_t parseDecimal(char *msg);