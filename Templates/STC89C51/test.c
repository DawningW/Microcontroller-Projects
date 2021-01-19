#include <STC89C5xRC.h>

void delay()
{
    unsigned char i, j, k;
    i = 5;
    j = 52;
    k = 195;
    do
    {
        do
        {
            while (--k);
        } while (--j);
    } while (--i);
}

void main()
{
    while (1)
    {
        delay();
    }
}
