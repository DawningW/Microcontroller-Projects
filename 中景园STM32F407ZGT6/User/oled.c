#include "oled.h"
#include <stdlib.h>
#include "delay.h"
#include "oledfont.h"

void oled_init(void)
{
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_GPIOG, ENABLE); //使能PORTA~E,PORTG时钟

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_1 | GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_Init(GPIOG, &GPIO_InitStructure);

    OLED_RST_Set();
    delay_ms(100);
    OLED_RST_Clr();
    delay_ms(200);
    OLED_RST_Set();

    oled_write_cmd(OLED_CMD_DISPLAY);
    
    oled_write_cmd(0xDA); // 设置列引脚硬件配置
    oled_write_cmd(0x12); // A5=0 A4=1
    oled_write_cmd(0xD5); // 设置显示时钟分频值/震荡频率
    oled_write_cmd(0x80); // Set Clock as 100 Frames/Sec
    oled_write_cmd(0xD9); // 设置预充电周期
    oled_write_cmd(0xF1); // Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
    oled_write_cmd(0xDB); // 设置VCOMH反压值
    oled_write_cmd(0x20); // ~0.77*Vcc
    
    oled_write_cmd(OLED_CMD_ADDR_MODE);
    oled_write_cmd(0x02); // 设置内存地址模式为页地址模式
    oled_write_cmd(OLED_CMD_LADDR | 0x02);
    oled_write_cmd(OLED_CMD_HADDR | 0x00);
    
    oled_write_cmd(OLED_CMD_CONTRAST);
    oled_write_cmd(0xFF); // 设置对比度为255
    oled_write_cmd(OLED_CMD_ENTIRE_DISPLAY);
    oled_write_cmd(OLED_CMD_INVERSE_DISPLAY);
    oled_write_cmd(OLED_CMD_START_LINE);
    
    oled_write_cmd(OLED_CMD_SEGMENT_REMAP | 0x01);
    oled_write_cmd(OLED_CMD_OUTPUT_DIRECTION | 0x08);
    oled_write_cmd(OLED_CMD_DISPLAY_OFFSET);
    oled_write_cmd(0x00); // 不偏移
    
    oled_display(true);
    oled_clear();
    oled_set_pos(0, 0);
}

void oled_write(bool type, uint8_t byte)
{
    if (type)
        OLED_DC_Set();
    else
        OLED_DC_Clr();
    OLED_CS_Clr();
    for (uint8_t i = 0; i < 8; ++i)
    {
        OLED_SCLK_Clr();
        if (byte & 0x80)
            OLED_SDIN_Set();
        else
            OLED_SDIN_Clr();
        OLED_SCLK_Set();
        byte <<= 1;
    }				 		  
    OLED_CS_Set();
    OLED_DC_Set();
}

void oled_write_cmd(uint8_t cmd)
{
    oled_write(false, cmd);
}

void oled_write_dat(uint8_t dat)
{
    oled_write(true, dat);
}

void oled_display(bool show)
{
    oled_write_cmd(OLED_CMD_CHARGE_PUMP);
    if (show)
    {
        oled_write_cmd(0x14); // 打开电源
        oled_write_cmd(OLED_CMD_DISPLAY | 0x01);
    }
    else
    {
        oled_write_cmd(0X10); // 关闭电源
        oled_write_cmd(OLED_CMD_DISPLAY);
    }
}

void oled_clear(void)
{
    for (uint8_t i = 0; i < 8; ++i)
    {
        oled_write_cmd(OLED_CMD_PAGE_ADDR + i);
        oled_write_cmd(OLED_CMD_LADDR);
        oled_write_cmd(OLED_CMD_HADDR);
        for (uint8_t n = 0; n < 128; ++n) oled_write_dat(0);
    }
}

void oled_set_pos(uint8_t x, uint8_t y)
{
    oled_write_cmd(OLED_CMD_PAGE_ADDR + y);
    oled_write_cmd(OLED_CMD_HADDR | ((x & 0xf0) >> 4));
    oled_write_cmd(OLED_CMD_LADDR | (x & 0x0f));
}

void oled_fill(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t dot)
{

}

void oled_disp_char(uint8_t x, uint8_t y, uint8_t c)
{
    c -= ' '; // 得到偏移后的值
    if (x > OLED_WIDTH - 1) { x = 0; y += 2; }
    if (SIZE == 16)
    {
        oled_set_pos(x, y);
        for (uint8_t i = 0; i < 8; ++i)
            oled_write_dat(F8X16[c * 16 + i]);
        oled_set_pos(x, y + 1);
        for (uint8_t i = 0; i < 8; ++i)
            oled_write_dat(F8X16[c * 16 + i + 8]);
    }
    else
    {
        oled_set_pos(x, y + 1);
        for (uint8_t i = 0; i < 6; ++i)
            oled_write_dat(F6x8[c][i]);
    }
}

void oled_disp_string(uint8_t x, uint8_t y, const char *str)
{
    while (*str)
    {
        oled_disp_char(x, y, *str++);
        x += 8;
        if (x > 120) { x = 0; y += 2; }
    }
}

void oled_disp_chinese(uint8_t x, uint8_t y, uint8_t num)
{

}

void oled_draw_image(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t img[])
{
    unsigned int j = 0;
    unsigned char x, y;
    if (y2 % 8 == 0) y = y2 / 8;
    else y = y2 / 8 + 1;
    for(y = y1; y < y2; y++)
    {
        oled_set_pos(x1, y1);
        for(x = x1; x < x2; x++)
        {
            // oled_write_dat(BMP[j++]);
        }
    }
}
