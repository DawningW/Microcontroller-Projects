#include "12232.h"
#include "font.h"
#include "delay.h"

static uint16_t find_chinese(const char *s)
{
    uint16_t i;
    for (i = 0; font_zh_index[i] != '\0'; i += 3)
    {
        if (font_zh_index[i] == s[0] && font_zh_index[i + 1] == s[1] && font_zh_index[i + 2] == s[2])
            return i / 3;
    }
    return 0;
}

static bool lcd_busy(lcd_side_t side)
{
    return (bool) (lcd_read_state(side) & 0x80);
}

void lcd_init()
{
    // 使用 68 系列 MPU 接口
    LCD_RESET = 1;
    _nop_();
    LCD_RESET = 0;
    _nop_();
    LCD_RESET = 1;
    lcd_write_cmd(ALL, LCD_CMD_RESET);
    lcd_write_cmd(ALL, LCD_CMD_MODE); // 正常驱动
    lcd_write_cmd(ALL, LCD_CMD_DUTY | 0x1); // 1/32DUTY
    lcd_write_cmd(ALL, LCD_CMD_DIRECTION); // 反向
    lcd_write_cmd(ALL, LCD_CMD_END); // 关闭"读-修改-写"模式
    lcd_write_cmd(ALL, LCD_CMD_DISPLAY | 0x1); // 打开显示
    lcd_clear();
}

BYTE lcd_read(lcd_side_t side, bit type)
{
    BYTE content = 0x00;
    LCD_A0 = type;
    LCD_RW = 1;
    if (side & LEFT)
    {
        LCD_E1 = 1;
        content |= LCD_DB;
        LCD_E1 = 0;
    }
    if (side & RIGHT)
    {
        LCD_E2 = 1;
        content |= LCD_DB;
        LCD_E2 = 0;
    }
    return content;
}

BYTE lcd_read_state(lcd_side_t side)
{
    return lcd_read(side, 0);
}

BYTE lcd_read_dat(lcd_side_t side)
{
    while (lcd_busy(side));
    return lcd_read(side, 1);
}

void lcd_write(lcd_side_t side, bit type, BYTE content)
{
    LCD_A0 = type;
    LCD_RW = 0;
    LCD_DB = content;
    if (side & LEFT)
    {
        LCD_E1 = 1;
        _nop_();
        LCD_E1 = 0;
    }
    if (side & RIGHT)
    {
        LCD_E2 = 1;
        _nop_();
        LCD_E2 = 0;
    }
}

void lcd_write_cmd(lcd_side_t side, BYTE cmd)
{
    while (lcd_busy(side));
    lcd_write(side, 0, cmd);
}

void lcd_write_dat(lcd_side_t side, BYTE dat)
{
    while (lcd_busy(side));
    lcd_write(side, 1, dat);
}

void lcd_clear()
{
    BYTE i, j;
    lcd_write_cmd(ALL, LCD_CMD_LINE);
    for (i = 0; i < 4; i++)
    {
        lcd_write_cmd(ALL, LCD_CMD_PAGE + i);
        lcd_write_cmd(ALL, LCD_CMD_COLUMN);
        for (j = 0; j < 61; j++)
        {
            lcd_write_dat(ALL, 0x00);
        }
    }
}

void lcd_draw_glyph(BYTE x, BYTE y, BYTE *glyph, BYTE w, BYTE h)
{
    BYTE i, j;
    // 在左屏还是右屏
    lcd_side_t side = x < 61 ? LEFT : RIGHT;
    if (side == RIGHT) x -= 61;
    // 开始绘制
    for (i = 0; i < h; i++)
    {
        lcd_write_cmd(side, LCD_CMD_PAGE + ((y + i) & 0x3));
        lcd_write_cmd(side, LCD_CMD_COLUMN + x);
        for (j = 0; j < w; j++)
        {
            if (x + j < 61)
            {
                lcd_write_dat(side, glyph[i * w + j]);
            }
            else if (side == LEFT)
            {
                if (x + j == 61)
                {
                    // 从左屏跨右屏
                    lcd_write_cmd(RIGHT, LCD_CMD_PAGE + ((y + i) & 0x3));
                    lcd_write_cmd(RIGHT, LCD_CMD_COLUMN);
                }
                lcd_write_dat(RIGHT, glyph[i * w + j]);
            }
        }
    }
}

void lcd_draw_str(BYTE x, BYTE y, const char *str)
{
    while (*str != '\0')
    {
        if ((BYTE) *str < 0x80) // 这里需要显式类型转换
        {
            lcd_draw_glyph(x, y, font_en_8x16[*str - 32], 8, 2);
            x += 8;
            str += 1;
        }
        else
        {
            lcd_draw_glyph(x, y, font_zh_16x16[find_chinese(str)], 16, 2);
            x += 16;
            str += 3;
        }
        if (x >= 122)
        {
            break;
        }
    }
}
