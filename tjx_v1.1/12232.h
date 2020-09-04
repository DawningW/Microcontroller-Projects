#ifndef _LCD_h_
#define _LCD_h_

#include <intrins.h>
#include "system.h"
#include "registers.h"

// 选择引脚
sfr LCD_DB = 0x80;
sbit LCD_RESET = P1^0;
sbit LCD_E1 = P1^1;
sbit LCD_E2 = P1^2;
sbit LCD_RW = P1^3;
sbit LCD_A0 = P1^4;

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
// x: 起始行(1~31)
#define LCD_CMD_LINE 0xC0
// 页地址设置
// 1 0 1 1 1 0 x x
// x: 页地址(0~3)
#define LCD_CMD_PAGE 0xB8
// 列地址设置
// 0 x x x x x x x
// x: 列地址(0~61)
#define LCD_CMD_ROW 0x00
// "读-修改-写"模式设置
// 1 1 1 0 0 0 0 0
// 执行该指令以后，每执行一次写数据指令列地址自动加 1；但执行读数据指令时列地址不会改变。这个状态一直持续到执行“END”指令。
// 注意：在“读-修改-写”模式下，除列地址设置指令之外，其他指令照常执行。 
#define LCD_CMD_ADD 0xE0
// END 指令
// 1 1 1 0 1 1 1 0
// 关闭“读-修改-写”模式，并把列地址指针恢复到打开“读-修改-写”模式前的位置。
#define LCD_CMD_END 0xEE

// 初始化
extern void lcd_init()
extern byte lcd_read(bit type, bit side)
extern byte lcd_read_state(bit side)
extern byte lcd_read_dat(bit side)
extern bit lcd_busy(bit side)
extern void lcd_write(bit type, bit side, byte content)
extern void lcd_write_cmd(bit side, byte cmd)
#define lcd_write_cmd_all(cmd) lcd_write_cmd(0, cmd);lcd_write_cmd(1, cmd);
extern void lcd_write_dat(bit side, byte dat)

#endif
