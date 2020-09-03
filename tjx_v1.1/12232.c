#include "12232.h"
#include "timer.h"

sfr LCD_DB = 0x80;
sbit LCD_RESET = P1^0;
sbit LCD_E1 = P1^1;
sbit LCD_E2 = P1^2;
sbit LCD_RW = P1^3;
sbit LCD_A0 = P1^4;

void lcd_init()
{
    delay(15);
    LCD_RESET = 1;
    _nop_();
    LCD_RESET = 0;
    delay(5);
    lcd_write_cmd_all(0xe2);
    delay(5);
    lcd_write_cmd_all(0xa4);
    delay(5);
    lcd_write_cmd_all(0xa9);
    delay(5);
    lcd_write_cmd_all(0xa0);
    delay(5);
    lcd_write_cmd_all(0xee);
    delay(5);
    lcd_write_cmd_all(0xaf); 
    delay(5);
}

byte lcd_read(bit type, bit side)
{
    byte content;
    LCD_A0 = type;
    LCD_RW = 1;
    if (side == 0)
    {
        LCD_E1 = 0;
        content = LCD_DB;
        LCD_E1 = 1;
    }
    else
    {
        LCD_E2 = 0;
        content = LCD_DB;
        LCD_E2 = 1;
    }
    return content;
}

byte lcd_read_state(bit side)
{
	return lcd_read(0, side);
}

byte lcd_read_dat(bit side)
{
	while (lcd_busy(side));
	return lcd_read(1, side);
}

static bit lcd_busy(bit side)
{
	return (bit) (lcd_read_state(side) & 0x80);
}

void lcd_write(bit type, bit side, byte content)
{
    LCD_A0 = type;
    LCD_RW = 0;
    LCD_DB = content;
    if (side == 0)
    {
        LCD_E1 = 0;
        _nop_();
        LCD_E1 = 1;
    }
    else
    {
        LCD_E2 = 0;
        _nop_();
        LCD_E2 = 1;
    }
}

void lcd_write_cmd(bit side, byte cmd)
{
    while (lcd_busy(side));
	lcd_write(0, side, cmd);
}

#define lcd_write_cmd_all(cmd) lcd_write_cmd(0, cmd);lcd_write_cmd(1, cmd); 

void lcd_write_dat(bit side, byte dat)
{
	while (lcd_busy(side));
	lcd_write(1, dat);
}
