#ifndef _LCD_h_
#define _LCD_h_

#include <STC89C5xRC.H>
#include <intrins.h>

// 基本数据类型
#define uchar unsigned char
#define uint unsigned int
	
// 选择引脚
// 若采用四线传输方式,MCU口的高四位和LCD数据口的高四位相连,传输数据时先发送高四位,再发送低四位数据
// #define LCD_4DB // 4数据线
#define LCD_8DB // 8数据线
sfr LCD_DB = 0x80;
sbit LCD_RS = P2^7;
sbit LCD_RW = P2^6;
sbit LCD_E = P2^5;

// 命令字
// 控制指令
#define LCD_CMD_CLR 0x01 // 清屏
#define LCD_CMD_RST 0x02 // 光标复位
// 输入模式设置: 写入新数据后光标或字符的移动
// 0 0 0 0 0 1 I/D S
// I/D 0=光标左移 1=光标右移
// S 0=字符不移动 1=字符右移
#define LCD_CMD_ENTRY_MODE 0x04
// 显示开/关控制: 控制显示器开/关, 光标显示/关闭以及光标是否闪烁
// 0 0 0 0 1 D C B
// D 0=显示关 1=显示开
// C 0=光标关闭 1=光标显示
// B 0=光标闪烁 1=光标不闪烁
#define LCD_CMD_DISPLAY_CONTROL 0x08
// 光标或字符移位: 使光标移位或使整个屏幕移位
// 0 0 0 1 S/C R/L * *
// S/C 0=光标(改变AC,左AC--,右AC++) 1=屏幕(不改变AC,光标会跟随字符移动)
// R/L 0=左移 1=右移
#define LCD_CMD_SHIFT 0x10
// 功能设置指令
// 0 0 1 DL N F * *
// DL 0=4位数据总线 1=8位数据总线
// N 0=显示单行 1=显示双行
// F 0=5×7点阵 1=5×10点阵
#define LCD_CMD_4DB 0x28 // 4位总线, 双行, 5x7点阵
#define LCD_CMD_8DB 0x38 // 8位总线, 双行, 5x7点阵

// 设置数据总线
#ifdef LCD_4DB
#define LCD_MODE LCD_CMD_4DB
#endif
#ifdef LCD_8DB
#define LCD_MODE LCD_CMD_8DB
#endif

/* 控制线说明
 * 状态字读操作：输入: RS=低、RW=高、E=高, 输出:读出状态字,
 * 数据读出操作：输入: RS=高、RW=高、E=高, 输出:读出为数据,
 * 指令写入操作：输入: RS=低、RW=低、E=下降沿, 输出:无,
 * 数据写入操作：输入: RS=高、RW=低、E=下降沿, 输出:无.
 */

extern void init_lcd();
extern bit lcd_busy();
#define lcd_write_cmd(cmd) lcd_write_cmd_busy(cmd, 1)
extern void lcd_write_cmd_busy(uchar cmd, bit check_busy);
extern void lcd_set_address(bit row, uchar col);
extern void lcd_write_dat(uchar dat);
extern uchar lcd_read_dat();
extern void lcd_write_cgram(uchar pos, uchar *arr);

extern void lcd_print(uchar *str);
extern void lcd_disp(bit row, uchar col, uchar ch);

#endif
