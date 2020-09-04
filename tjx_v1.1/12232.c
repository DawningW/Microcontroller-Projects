#include "12232.h"
#include "timer.h"

void lcd_init()
{
    delay(15);
    LCD_RESET = 1;
    _nop_();
    LCD_RESET = 0;
    delay(5);
    lcd_write_cmd_all(LCD_CMD_RESET);
    delay(5);
    lcd_write_cmd_all(LCD_CMD_MODE); // 正常驱动
    delay(5);
    lcd_write_cmd_all(LCD_CMD_DUTY | 0x1); // 1/32DUTY
    delay(5);
    lcd_write_cmd_all(LCD_CMD_DIRECTION); // 反向
    delay(5);
    lcd_write_cmd_all(LCD_CMD_END); // 关闭“读-修改-写”模式
    delay(5);
    lcd_write_cmd_all(LCD_CMD_DISPLAY | 0x1); // 打开显示
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

void lcd_write_dat(bit side, byte dat)
{
	while (lcd_busy(side));
	lcd_write(1, dat);
}

// TODO 待移除
void wrlattice(byte data1, byte data2)
{
    byte i,j;
    for (j = 0; j < 4; j++)
    {
        lcd_write_cmd_all(LCD_CMD_LINE);
        lcd_write_cmd_all(LCD_CMD_PAGE + j);
        lcd_write_cmd_all(LCD_CMD_ROW);
        for (i = 0; i < 61; i += 2)
        {
            lcd_write_dat(1, data1);
            lcd_write_dat(2, data2);
        }
    }
}
