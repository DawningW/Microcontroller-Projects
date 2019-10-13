/*------------------------------------------------------------------*/
/* --- STC MCU Limited ---------------------------------------------*/
/* --- STC89-90xx Series MCU IO Demo -------------------------------*/
/* --- Mobile: (86)13922805190 -------------------------------------*/
/* --- Fax: 86-0513-55012956,55012947,55012969 ---------------------*/
/* --- Tel: 86-0513-55012928,55012929,55012966----------------------*/
/* --- Web: www.STCMCU.com -----------------------------------------*/
/* --- Web: www.GXWMCU.com -----------------------------------------*/
/* If you want to use the program or the program referenced in the  */
/* article, please specify in which data and procedures from STC    */
/*------------------------------------------------------------------*/

#include <STC89C5xRC.H>

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
	  P00 = 0;
    delay();
    P00 = 1;
    P01 = 0;
    delay();
    P01 = 1;
    P02 = 0;
    delay();
    P02 = 1;
    P03 = 0;
    delay();
    P03 = 1;
    P04 = 0;
    delay();
    P04 = 1;
    P05 = 0;
    delay();
    P05 = 1;
    P06 = 0;
    delay();
    P06 = 1;
    P07 = 0;
    delay();
    P07 = 1;
	
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
		
		P40 = 0;
    delay();
    P40 = 1;
    P41 = 0;
    delay();
    P41 = 1;
    P42 = 0;
    delay();
    P42 = 1;
    P43 = 0;
    delay();
    P43 = 1;

    while (1)
    {
			  P0 = 0x00;
        delay();
        P0 = 0xff;
			
        P1 = 0x00;
        delay();
        P1 = 0xff;
			
			  P2 = 0x00;
        delay();
        P2 = 0xff;

        P3 = 0x00;
        delay();
        P3 = 0xff;
			
			  P4 = 0x00;
        delay();
        P4 = 0xff;
    }
}

