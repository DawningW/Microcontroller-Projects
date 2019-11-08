#ifndef _LCD_h_
#define _LCD_h_

#include <intrins.h>
#include "system.h"
#include "registers.h"

#define MAX_DISPLAY_CHAR 16 // 0x8F - 0x80 + 1
#define MAX_STORE_CHAR 39 // 0xC0 - 0x80
	
// 选择引脚
#define LCD_8BIT_DB // 是否使用8位数据线
// 若采用四线传输方式,MCU口的高四位和LCD数据口的高四位相连,传输数据时先发送高四位,再发送低四位数据
sfr LCD_DB = 0x80;
sbit LCD_RS = P2^7;
sbit LCD_RW = P2^6;
sbit LCD_E = P2^5;

// 控制指令
#define LCD_CMD_CLR 0x01 // 清屏
#define LCD_CMD_RST 0x02 // 光标复位
// 输入模式设置: 写入新数据后光标或字符的移动
// 0 0 0 0 0 1 I/D S
// I/D 0=光标左移 1=光标右移
// S 0=字符不移动 1=字符右移
#define LCD_CMD_ENTRY 0x04
// 显示开/关控制: 控制显示器开/关, 光标显示/关闭以及光标是否闪烁
// 0 0 0 0 1 D C B
// D 0=显示关 1=显示开
// C 0=光标关闭 1=光标显示
// B 0=光标闪烁 1=光标不闪烁
#define LCD_CMD_DISPLAY 0x08
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
#ifdef LCD_8BIT_DB
#define LCD_CMD_MODE 0x38 // 8位总线, 双行, 5x7点阵
#else
#define LCD_CMD_MODE 0x28 // 4位总线, 双行, 5x7点阵
#endif

// 初始化
extern void lcd_init();
/* 读数据
 * 状态字读操作：输入: RS=低、RW=高、E=高, 输出:读出状态字,
 * 数据读出操作：输入: RS=高、RW=高、E=高, 输出:读出为数据,
 */
extern byte lcd_read(bit type);
extern byte lcd_read_state(); // 不检查忙
extern byte lcd_read_dat();
/**
 * 检查LCD忙状态
 * 返回1时, 忙, 等待
 * 返回0时, 闲, 可写指令与数据
 */
extern bit lcd_busy(); // 调用lcd_read_state()
/* 写数据
 * 指令写入操作：输入: RS=低、RW=低、E=下降沿, 输出:无,
 * 数据写入操作：输入: RS=高、RW=低、E=下降沿, 输出:无.
 */
extern void lcd_write(bit type, byte content);
extern void lcd_write_cmd(byte cmd);
extern void lcd_write_dat(byte dat);
/**
 * 向CGRAM中写入自定义字符
 * 无需处理地址
 * pos 0-7 写入自定义cgram中的位置
 */ 
extern void lcd_write_cgram(byte pos, byte *arr);
/**
 * 设定下一个要写的字符的地址(DDRAM)
 */
extern void lcd_set_pos(bit row, byte col);
// 输出
/**
 * 在特定位置显示字符
 */
extern void lcd_disp(bit row, byte col, byte ch);
/**
 * 显示字符串
 * 不要忘记字符数组末尾要有'\0'
 */
extern void lcd_print(byte *str);
#endif
