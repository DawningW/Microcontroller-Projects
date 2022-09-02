#include "delay.h"

void delay(uint16_t us)
{
    while (us--)
        _nop_();
}

void delay_ms(uint16_t ms) // @22.1184MHz
{
    uint8_t i, j;
    while (ms-- != 0)
    {
        i = 22;
        j = 128;
        do
        {
            while (--j);
        }
        while (--i);
    }
}
