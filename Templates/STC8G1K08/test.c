#include <intrins.h>
#include "STC8G.h"

#define uint8 unsigned char
#define uint16 unsigned short
#define uint32 unsigned int

// 灯的数据接在了单片机哪个引脚上
#define LED_DATA P31
// 灯的个数
#define LED_COUNT 13

// 毫秒延时 晶振@11.0592MHz
void delayms(uint16 ms)
{
    uint8 i, j;
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

// 向LED数据脚发送一个字节的数据
void sendByte(uint8 byte)
{
    char i;
    for (i = 8; i > 0; --i)
    {
        LED_DATA = 1;
        _nop_();
        byte <<= 1; // 最高位送入CY
        LED_DATA = CY;
        _nop_();
        LED_DATA = 0;
        _nop_();
    }
}

// 向LED数据脚发送一个灯珠的颜色
void sendColor(uint8 red, uint8 green, uint8 blue)
{
    sendByte(green);
    sendByte(red);
    sendByte(blue);
}

// 向LED数据脚发送复位指令
void sendReset()
{
    LED_DATA = 0;
    delayms(1);
}

// 程序入口
int main()
{
    uint8 i, light = 0;
    while (1)
    {
        // 跑马灯
        for (i = 0; i < LED_COUNT; ++i)
        {
            if (light == i) sendColor(63, 63, 63);
            else sendColor(0, 0, 0);
        }
        if (++light >= LED_COUNT) light = 0;
        sendReset();
        delayms(500);
    }
    return 0;
}
