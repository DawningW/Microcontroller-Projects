#include <STC8G.h>

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

void main()
{
    while (1)
    {
        delayms(500);
    }
    return 0;
}
