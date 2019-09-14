#include "1602A.h"
uchar code bell[] ={0x01,0x1b,0x1d,0x19,0x1d,0x1b,0x01,0x00};
void main()
{
    uchar i;
    lcd_init();

   	lcd_PrPos(0,16);
   	lcd_PrString("abc");

   	for(i = 0; i < 16; i++)
   	{
      	lcd_wcmd(0x18);
    	delay(200);
   	}
	lcd_wcmd(0x02);    //重置光标
	lcd_wcmd(0x01);    //清屏

    lcd_wCGRAM(0x01,bell); //写入自定义字符

	lcd_PrPos(1,5);
//	lcd_wdat(0x01);    //显示自定义字符
	lcd_wdat(0x39);

	lcd_PrPos(0,5);
	for(i=0;i<5;i++)
	{
		lcd_wdat(0x30+i);
	}
	while(1);
}
