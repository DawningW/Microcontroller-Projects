#ifndef __OLED_H
#define __OLED_H
#include "system.h"

// OLED模式设置
// 0: 4线串行模式
// 1: 并行8080模式
#define OLED_MODE 0

#define OLED_SCLK_Clr() GPIO_ResetBits(GPIOG, GPIO_Pin_12) // CLK
#define OLED_SCLK_Set() GPIO_SetBits(GPIOG, GPIO_Pin_12)
#define OLED_SDIN_Clr() GPIO_ResetBits(GPIOD, GPIO_Pin_5) // DIN
#define OLED_SDIN_Set() GPIO_SetBits(GPIOD, GPIO_Pin_5)
#define OLED_RST_Clr() GPIO_ResetBits(GPIOB, GPIO_Pin_0) // RES
#define OLED_RST_Set() GPIO_SetBits(GPIOB, GPIO_Pin_0)
#define OLED_DC_Clr() GPIO_ResetBits(GPIOD, GPIO_Pin_15) // DC
#define OLED_DC_Set() GPIO_SetBits(GPIOD, GPIO_Pin_15)
#define OLED_CS_Clr()  GPIO_ResetBits(GPIOD, GPIO_Pin_1) // CS
#define OLED_CS_Set()  GPIO_SetBits(GPIOD, GPIO_Pin_1)

#define	OLED_CMD_CONTRAST 0x81 // 设置对比度(范围0x00~0xFF)
#define OLED_CMD_ENTIRE_DISPLAY 0xA4 // 是否整片全亮
#define OLED_CMD_INVERSE_DISPLAY 0xA6 // 是否反白显示
#define OLED_CMD_DISPLAY 0xAE // 是否开启屏幕
#define OLED_CMD_LADDR 0x00 // 页地址模式下设置列起始地址低4位
#define OLED_CMD_HADDR 0x10 // 页地址模式下设置列起始地址高4位
#define OLED_CMD_PAGE_ADDR 0xB0 // 设置目标显示位置页起始地址(0~7)
#define OLED_CMD_ADDR_MODE 0x20 // 设置内存地址模式(水平地址模式, 垂直地址模式, 页地址模式(默认))
#define OLED_CMD_START_LINE 0x40 // 设置屏幕起始行(范围0x00~0x3F)
#define OLED_CMD_SEGMENT_REMAP 0xA0 // 设置段与列的映射关系, 即左右反转(|0x01为反转)
#define OLED_CMD_OUTPUT_DIRECTION 0xC0 // 设置列输出扫描方向, 即上下反转(|0x08为反转)
#define OLED_CMD_DISPLAY_OFFSET 0xD3 // 设置显示偏移(范围0~63)
#define OLED_CMD_CHARGE_PUMP 0x8D // 电荷泵(0x14开, 0x10关)

#define OLED_WIDTH 128
#define OLED_HEIGHT 64
#define SIZE 16

void oled_init(void);
void oled_write(bool type, uint8_t byte);
void oled_write_cmd(uint8_t cmd);
void oled_write_dat(uint8_t dat);
void oled_display(bool show);
void oled_clear(void);
void oled_set_pos(uint8_t x, uint8_t y);
void oled_fill(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t dot);
void oled_disp_char(uint8_t x, uint8_t y, uint8_t c);
void oled_disp_string(uint8_t x, uint8_t y, const char *str);
void oled_disp_chinese(uint8_t x, uint8_t y, uint8_t num);
void oled_draw_image(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t img[]);

#endif
