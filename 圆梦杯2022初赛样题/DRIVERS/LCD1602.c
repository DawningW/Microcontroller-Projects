#include "LCD1602.h"

u8 code Xword[] = {
	0x18, 0x18, 0x07, 0x08, 0x08, 0x08, 0x07, 0x00, //℃，代码 0x00
	0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, //一，代码 0x01
	0x00, 0x00, 0x00, 0x0e, 0x00, 0xff, 0x00, 0x00, //二，代码 0x02
	0x00, 0x00, 0xff, 0x00, 0x0e, 0x00, 0xff, 0x00, //三，代码 0x03
	0x00, 0x00, 0xff, 0xf5, 0xfb, 0xf1, 0xff, 0x00, //四，代码 0x04
	0x00, 0xfe, 0x08, 0xfe, 0x0a, 0x0a, 0xff, 0x00, //五，代码 0x05
	0x00, 0x04, 0x00, 0xff, 0x00, 0x0a, 0x11, 0x00, //六，代码 0x06
	0x00, 0x1f, 0x11, 0x1f, 0x11, 0x11, 0x1f, 0x00, //日，代码 0x07
};

void Delay(u8 delay)
{
	u8 j;
	while (delay--)
	{
		for (j = 0; j < 1; j++);
	}
}

void LCD1602_GPIO_config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.Mode = GPIO_PullUp;
	GPIO_InitStructure.Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_Inilize(GPIO_P0, &GPIO_InitStructure);
	GPIO_InitStructure.Mode = GPIO_PullUp;
	GPIO_InitStructure.Pin = GPIO_Pin_All;
	GPIO_Inilize(GPIO_P6, &GPIO_InitStructure);
}

// 远程不需要测忙
void LCD1602_is_busy()
{
	Lcd1602DataIn();
	Lcd1602RSLow();
	Lcd1602RWHigh();
	Lcd1602ENHigh();
	while (Lcd1602Busy());
	Lcd1602ENLow();
	Lcd1602DataOut();
}

void LCD1602_write_cmd(u8 cmd)
{
	// LCD1602_is_busy();
	Lcd1602RSLow();
	Lcd1602RWLow();
	Lcd1602WriteData(cmd);
	Lcd1602ENHigh();
	Delay(5);
	Lcd1602ENLow();
}

void LCD1602_write_data(u8 dat)
{
	// LCD1602_is_busy();
	Lcd1602RSHigh();
	Lcd1602RWLow();
	Lcd1602WriteData(dat);
	Lcd1602ENHigh();
	Delay(5);
	Lcd1602ENLow();
}

// 装入CGRAM
void LCD1602_init_cgram(void)
{
	u8 i;
	LCD1602_write_cmd(0x06); // CGRAM地址自动加1
	LCD1602_write_cmd(0x40); // CGRAM地址设为00处
	for (i = 0; i < 64; i++)
	{
		LCD1602_write_data(Xword[i]); // 按数组写入数据
	}
}

void LCD1602_init()
{
	Lcd1602DataOut();
	LCD1602_write_cmd(CMD_set82);
	LCD1602_write_cmd(CMD_clear);
	LCD1602_write_cmd(CMD_back);
	LCD1602_write_cmd(CMD_add1);
	LCD1602_write_cmd(CMD_dis_gb3);
	LCD1602_init_cgram();
}

// 清屏
void LCD1602_clear(void)
{
	LCD1602_write_cmd(CMD_clear);
}

/********************************************************************************************
// 打印单字符程序 //
// 第一行位置 0x00~0x15  第二行位置 0x40~0x55
// 向LCD发送一个字符,以十六进制（0x00）表示
// 应用举例：print(0xc0,0x30); // 在第二行第一位处打印字符“0”
/********************************************************************************************/
void LCD1602_show_char(u8 pos, u8 dat)
{
	LCD1602_write_cmd(pos | 0x80);
	LCD1602_write_data(dat);
}

/********************************************************************************************
// 打印字符串程序 // （本函数调用指针函数）
// 向LCD发送一个字符串,长度48字符之内
// 第一行位置 0x00~0x15  第二行位置 0x40~0x55
// 应用举例：print(0x80,"doyoung.net"); // 在第一行第一位处从左向右打印doyoung.net字符串
/********************************************************************************************/
void LCD1602_show_str(u8 pos, u8 *str)
{
	LCD1602_write_cmd(pos | 0x80);
	while (*str != '\0')
	{
		LCD1602_write_data(*str++);
	}
}
