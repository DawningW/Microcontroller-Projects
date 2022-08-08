#include "system.h"

// TODO delay 需要重写以适配不同频率

void delay(uint16_t ms)
{
    uint8_t i;
    while (ms-- != 0)
        for (i = 0; i < 91; i++);
}

void sleep(uint16_t s)
{
    uint8_t i, j, k;
    while (s-- != 0)
    {
        i = 8, j = 1, k = 243;
        _nop_();
        do
        {
            do
            {
                while (--k);
            }
            while (--j);
        }
        while (--i);
    }
}
