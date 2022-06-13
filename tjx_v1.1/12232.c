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

BYTE lcd_read(bit type, bit side)
{
    BYTE content;
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

BYTE lcd_read_state(bit side)
{
	return lcd_read(0, side);
}

static bool lcd_busy(bit side)
{
	return (bool) (lcd_read_state(side) & 0x80);
}

BYTE lcd_read_dat(bit side)
{
	while (lcd_busy(side));
	return lcd_read(1, side);
}

void lcd_write(bit type, bit side, BYTE content)
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

void lcd_write_cmd(bit side, BYTE cmd)
{
    while (lcd_busy(side));
	lcd_write(0, side, cmd);
}

void lcd_write_dat(bit side, BYTE dat)
{
	while (lcd_busy(side));
	lcd_write(1, side, dat);
}

// TODO 待重写
void wrlattice(BYTE data1, BYTE data2)
{
    BYTE i,j;
    for (j = 0; j < 4; j++)
    {
        lcd_write_cmd_all(LCD_CMD_PAGE + j);
        lcd_write_cmd_all(LCD_CMD_LINE);
        lcd_write_cmd_all(LCD_CMD_ROW);
        for (i = 0; i < 61; i += 2)
        {
            lcd_write_dat(1, data1);
            lcd_write_dat(2, data2);
        }
    }
}

void disp_char(BYTE code *eng)
{ 
    BYTE i, j, k;
    for (k = 0; k < 7; k++)
    {
        for(j=0;j<4;j++)
        {
            lcd_write_cmd(0, LCD_CMD_PAGE + j);
            lcd_write_cmd(0, LCD_CMD_LINE);
            lcd_write_cmd(0, LCD_CMD_ROW + 5 + k * 8);
            for (i = 0; i < 8; i++)
            lcd_write_dat(0,eng[k * 32 + j * 8 + i]);
        }
    }
    for (k = 7; k < 14; k++)
    {
        for (j = 0; j < 4; j++)
        {
            lcd_write_cmd(1, LCD_CMD_PAGE + j);
            lcd_write_cmd(1, LCD_CMD_LINE);
            lcd_write_cmd(1, LCD_CMD_ROW + k * 8 - 56);
            for (i = 0; i < 8; i++)
            lcd_write_dat(1,eng[k * 32 + j * 8 + i]);
        }
    }
}

void disp_chinese(BYTE code *chn)
{
    BYTE i, j, k;
    for (k = 0; k < 3; k++)
    {
        for (j = 0; j < 4; j++)
        {
            lcd_write_cmd(0, LCD_CMD_PAGE + j);
            lcd_write_cmd(0, LCD_CMD_LINE);
            lcd_write_cmd(0, LCD_CMD_ROW + 12 + k * 16);
            for (i = 0; i < 16; i++)
            lcd_write_dat(0, chn[k * 64 + j * 16 + i]);
        }
    }
    for (k = 3; k < 6; k++)
    {
        for (j = 0; j < 4; j++)
        {
            lcd_write_cmd(1, LCD_CMD_PAGE + j);
            lcd_write_cmd(1, LCD_CMD_LINE);
            lcd_write_cmd(1, LCD_CMD_ROW + k * 16 - 48);
            for (i = 0; i < 16; i++)
            lcd_write_dat(1, chn[k * 64 + j * 16 + i]);
        }
    }
} 

void disp_pattern(BYTE code *img)
{
    BYTE i, j;
    for (j = 0; j < 4; j++)
    {
        lcd_write_cmd_all(LCD_CMD_PAGE + j);
        lcd_write_cmd_all(LCD_CMD_LINE);
        lcd_write_cmd_all(LCD_CMD_ROW);
        for (i = 0; i < 122; i++)
        {
            if (i < 61)
            {
                lcd_write_cmd(0, LCD_CMD_ROW + i);
                lcd_write_dat(0,img[j * 122 + i]);
            }
            else
            {
                lcd_write_cmd(1, LCD_CMD_ROW + i - 61);
                lcd_write_dat(1,img[j * 122 + i]);
            }
        }
    }
} 
