#include <STC12C5410AD.H>

void delay()
{
    int i, j;

    for (i=0; i<1000; i++)
    for (j=0; j<500; j++);
}
void main()
{
    P10 = 0;
    delay();
    P10 = 1;
    P11 = 0;
    delay();
    P11 = 1;
    P12 = 0;
    delay();
    P12 = 1;
    P13 = 0;
    delay();
    P13 = 1;
    P14 = 0;
    delay();
    P14 = 1;
    P15 = 0;
    delay();
    P15 = 1;
    P16 = 0;
    delay();
    P16 = 1;
    P17 = 0;
    delay();
    P17 = 1;

    P20 = 0;
    delay();
    P20 = 1;
    P21 = 0;
    delay();
    P21 = 1;
    P22 = 0;
    delay();
    P22 = 1;
    P23 = 0;
    delay();
    P23 = 1;
    P24 = 0;
    delay();
    P24 = 1;
    P25 = 0;
    delay();
    P25 = 1;
    P26 = 0;
    delay();
    P26 = 1;
    P27 = 0;
    delay();
    P27 = 1;

    P30 = 0;
    delay();
    P30 = 1;
    P31 = 0;
    delay();
    P31 = 1;
    P32 = 0;
    delay();
    P32 = 1;
    P33 = 0;
    delay();
    P33 = 1;
    P34 = 0;
    delay();
    P34 = 1;
    P35 = 0;
    delay();
    P35 = 1;
    P36 = 0;
    delay();
    P36 = 1;
    P37 = 0;
    delay();
    P37 = 1;

    while (1)
    {
        P1 = 0x00;
        delay();
        P1 = 0xff;

			  P2 = 0x00;
        delay();
        P2 = 0xff;
			
        P3 = 0x00;
        delay();
        P3 = 0xff;
    }
}