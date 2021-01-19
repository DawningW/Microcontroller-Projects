#include <STC12C5410AD.h>

void delay()
{
    int i, j;
    for (i=0; i<1000; i++)
    for (j=0; j<500; j++);
}

void main()
{
    while (1)
    {
        delay();
    }
}
