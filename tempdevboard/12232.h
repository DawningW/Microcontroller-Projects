#ifndef __LCD_12232_H__
#define __LCD_12232_H__

#include "includes.h"

// 选择引脚
#define LCD_DB P0
#define LCD_RESET P46
#define LCD_E1 P45
#define LCD_E2 P44
#define LCD_RW P27
#define LCD_A0 P26

// 控制指令
#define LCD_CMD_RESET 0xE2 // 复位
// 显示开关指令
// 1 0 1 0 1 1 1 D
// D 0=显示关 1=显示开
// 在显示关闭的状态下选择静态驱动模式，那么内部电路将处于安全模式
// 不改变显示 RAM(DD RAM) 中的内容,也不影响内部状态
#define LCD_CMD_DISPLAY 0xAE
// 开/关静态驱动模式设置
// 1 0 1 0 0 1 0 D
// D 0=正常驱动 1=打开静态显示。
// 在打开静态显示时执行关闭显示指令，内部电路将被置为安全模式
#define LCD_CMD_MODE 0xA4
// DUTY(占空比)选择
// 1 0 1 0 1 0 0 D
// D 0=1/16DUTY 1=1/32DUTY
#define LCD_CMD_DUTY 0xA8
// 设置显示方向
// 1 0 1 0 0 0 0 D
// D 0=反向 1=正向
// 设置 DDRAM 中的列地址与段驱动输出的对应关系
#define LCD_CMD_DIRECTION 0xA0
// 设置显示起始行
// 1 1 0 x x x x x
// x: 显示起始行(0~31)
#define LCD_CMD_LINE 0xC0
// 页地址设置
// 1 0 1 1 1 0 x x
// x: 页地址(0~3)
#define LCD_CMD_PAGE 0xB8
// 列地址设置
// 0 x x x x x x x
// x: 列地址(0~60)
#define LCD_CMD_COLUMN 0x00
// "读-修改-写"模式设置
// 1 1 1 0 0 0 0 0
// 执行该指令以后，每执行一次写数据指令列地址自动加 1；但执行读数据指令时列地址不会改变。这个状态一直持续到执行“END”指令。
// 注意：在“读-修改-写”模式下，除列地址设置指令之外，其他指令照常执行。
#define LCD_CMD_ADD 0xE0
// END 指令
// 1 1 1 0 1 1 1 0
// 关闭“读-修改-写”模式，并把列地址指针恢复到打开“读-修改-写”模式前的位置。
#define LCD_CMD_END 0xEE

typedef enum
{
	LEFT = 0x1,
	RIGHT = 0x2,
    ALL = LEFT | RIGHT
} lcd_side_t;

void lcd_init();
BYTE lcd_read(lcd_side_t side, bit type);
BYTE lcd_read_state(lcd_side_t side); // 不检查忙
BYTE lcd_read_dat(lcd_side_t side);
void lcd_write(lcd_side_t side, bit type, BYTE content);
void lcd_write_cmd(lcd_side_t side, BYTE cmd);
void lcd_write_dat(lcd_side_t side, BYTE dat);
void lcd_clear();
// 绘制一个字形, y和h的单位为8像素
void lcd_draw_glyph(BYTE x, BYTE y, BYTE *glyph, BYTE w, BYTE h);
// 字符串编码为UTF-8, 不支持'\\n'和'\\r'等控制字符
void lcd_draw_str(BYTE x, BYTE y, const char *str);

#endif
