#include <intrins.h>
#include "STC8G.h"

#define byte unsigned char

#define LED_DATA P31
#define LED_COUNT 13

// @11.0592MHz
void delayus(byte us)
{
    while (us--)
    {
        _nop_();
        _nop_();
    }
}

void delayms(byte ms)
{
    byte i, j;
    while (ms--)
    {
        i = 15;
        j = 90;
        do
        {
            while (--j);
        } while (--i);
    }
}

void sendByte(byte b)
{
    char i;
    for (i = 7; i >= 0; --i)
    {
        LED_DATA = 1;
        _nop_(); _nop_(); _nop_(); _nop_();
        LED_DATA = (b >> i) & 0x01;
        _nop_(); _nop_(); _nop_(); _nop_();
        LED_DATA = 0;
    }
}

void sendColor(byte red, byte green, byte blue)
{
    sendByte(green);
    sendByte(red);
    sendByte(blue);
}

int main()
{
    byte i, light = 0;
    while (1)
    {
        for (i = 0; i < LED_COUNT; ++i)
        {
            if (light == i) sendColor(0, 255, 0);
            else sendColor(0, 0, 0);
        }
        if (++light >= LED_COUNT) light = 0;
        delayms(1);
    }
    return 0;
}
