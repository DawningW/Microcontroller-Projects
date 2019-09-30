#include "1602A.h"
uchar code bell[] = {0x01,0x1b,0x1d,0x19,0x1d,0x1b,0x01,0x00};

void main()
{
	lcd_init();

	lcd_set_address(0, 0);
	delay(5);
	lcd_print("abc");
	delay(5);
	lcd_disp(1, 0, '!');
	delay(5);
	
	while (1);
}
