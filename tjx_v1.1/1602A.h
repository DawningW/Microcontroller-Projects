#ifndef __LCD_1602A_H__
#define __LCD_1602A_H__

#include "system.h"

#define MAX_DISPLAY_CHAR 16 // 0x8F - 0x80 + 1
#define MAX_STORE_CHAR 39 // 0xC0 - 0x80
	
// 选择引脚
#define LCD_8BIT_DB // 是否使用8位数据线
// 若采用四线传输方式, MCU口的高四位和LCD数据口的高四位相连, 传输数据时先发送高四位, 再发送低四位数据
#define LCD_DB P0
#define LCD_RS P27
#define LCD_RW P26
#define LCD_E P25

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

/**
 * 初始化
 */
void lcd_init();
/**
 * 读数据
 * 状态字读操作：输入: RS=低、RW=高、E=高, 输出:读出状态字
 * 数据读出操作：输入: RS=高、RW=高、E=高, 输出:读出为数据
 */
BYTE lcd_read(bit type);
BYTE lcd_read_state(); // 不检查忙
BYTE lcd_read_dat();
/**
 * 写数据
 * 指令写入操作：输入: RS=低、RW=低、E=下降沿, 输出:无
 * 数据写入操作：输入: RS=高、RW=低、E=下降沿, 输出:无
 */
void lcd_write(bit type, BYTE content);
void lcd_write_cmd(BYTE cmd);
void lcd_write_dat(BYTE dat);
/**
 * 向CGRAM中写入自定义字符
 * 无需处理地址
 * pos 0-7 写入自定义cgram中的位置
 */ 
void lcd_write_cgram(BYTE pos, BYTE *glyph);
/**
 * 清空屏幕
 */
void lcd_clear();
/**
 * 设定下一个要写的字符的地址(DDRAM)
 */
void lcd_set_pos(bit row, BYTE col);
/**
 * 在特定位置显示字符
 */
void lcd_disp(bit row, BYTE col, char ch);
/**
 * 显示字符串
 * 不要忘记字符数组末尾要有'\0'
 */
void lcd_print(const char *str);

#endif
