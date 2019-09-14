#ifndef _LCD_h_
#define _LCD_h_

#include <reg51.h>
#include <intrins.h>

// 基本数据类型
#define uchar unsigned char
#define uint unsigned int

// 根据实际电路板性能的好坏可以适当增加或减少以下延时
#define delayNOP(); {_nop_();_nop_();_nop_();_nop_();_nop_();};
	
// 选择LCD接口
// 若采用四线传输方式,MCU口的高四位和LCD数据口的高四位相连,传输数据时先发送高四位,再发送低四位数据
// #define LCD_4DB // 4数据线
#define LCD_8DB // 8数据线
sfr LCD_DB = 0x80;
sbit LCD_RS = P2^7;             
sbit LCD_RW = P2^6;
sbit LCD_E = P2^5;

/* 控制线说明
 * 状态字读操作：输入: RS=低、RW=高、E=高, 输出:读出状态字,
 * 数据读出操作：输入: RS=高、RW=高、E=高, 输出:读出为数据,
 * 指令写入操作：输入: RS=低、RW=低、E=上升沿, 输出:无,
 * 数据写入操作：输入: RS=高、RW=低、E=上升沿, 输出:无.
 */
#define LCD_CMD_4DB 0x28
#define LCD_CMD_8DB 0x38
#define LCD_CMD_CLR 0x01
#ifdef LCD_4DB
#define LCD_MODE LCD_CMD_4DB
#endif
#ifdef LCD_8DB
#define LCD_MODE LCD_CMD_8DB
#endif

/* 命令字
* 以下几条(指令)按位或,就可得组合
* 初始化命令:
// 0x28 //设置5*7 二行 4数据线
// 0x38 //设置5*7 二行 8数据线
// 0x01 //清屏
// 0x02 //光标复位指令
// 0x0e //整体显示开,光标开
// 0x08 //整体显示关
// 0x0c //整体显示开,光标关
// 0x0f //整体显示开,光标开,光标闪烁
// 0x0e //整体显示开,光标开,光标不闪烁
// 0x04 //字符不移,光标左移
// 0x06 //字符不移,光标右
// 0x05 //字符移,光标左移
// 0x07 //字符移,光标右移
* 控制移动命令:
// 0x10   //左移动光标
// 0x14   //右移动光标
// 0x18   //左移动字符
// 0x1c   //右移动字符
* 示例:
// LCD1602a 5*8 自定义字符CGRAM 控制
// 命令格式：0 1 A5 A4 A3 A2 A1 A0   ;A5-A0为 CGRAM 地址
// 从低位起每8个CGRAM组成一个自定义字符,共可定义8个
// 例如将自定义字符写入CGRAM的0x08-0x0f间，
// 调用lcd_wdat(0x02)就能显示
*/

extern uchar code TAB_LCD_Hex[];

extern void delay(uchar x); 
extern bit lcd_busy(); 
extern void lcd_wcmd(uchar cmd);
extern void lcd_wdat(uchar dat);
extern void lcd_init();  
extern void lcd_PrPos(uchar row ,uchar col);
extern void lcd_PrHexByte(uchar prbyte);
extern void lcd_PrString(uchar *prstring); 
extern void lcd_wCGRAM(uchar cg_pos,uchar *cg_arr);

#endif
