#include "1602A.h"

/**
 * 初始化
 */
void lcd_init()
{
	delay(15);
	lcd_write_cmd(LCD_MODE);
	delay(5);
	lcd_write_cmd(LCD_MODE);
	delay(5);
	lcd_write_cmd(LCD_MODE);
	delay(5);
	lcd_write_cmd(LCD_MODE); // 8位总线, 双行, 5x7
	delay(5);
	lcd_write_cmd(LCD_CMD_DISPLAY_CONTROL | 0x4); // 开启显示, 关闭光标, 不闪烁
	delay(5);
	lcd_write_cmd(LCD_CMD_CLR);
	delay(5);
	lcd_write_cmd(LCD_CMD_ENTRY_MODE | 0x2); // 光标右移, 字符不移位
	delay(5);
}

/**
 * 延时子程序
 * 11.0592MHZ
 */
void delay(uchar ms)
{
	uchar i;
	while (ms-- != 0)
	{
		for (i = 0; i < 91; i++);
	}
}

/**
 * 检查LCD忙状态
 * 返回1时, 忙, 等待
 * 返回0时, 闲, 可写指令与数据
 */
static bit lcd_busy()
{
	bit result;
	LCD_RS = 0;
	LCD_RW = 1;
	_nop_();
	LCD_E = 1;
	_nop_();
	result = (bit)(LCD_DB & 0x80);
	_nop_();
	LCD_E = 0;
	return result;
}
/**
 * 写指令数据
 * RS=0 RW=0 E=下降沿 D0-D7=指令
 */
void lcd_write_cmd(uchar cmd)
{
	// while (lcd_busy());
	LCD_RS = 0;
	LCD_RW = 0;
	_nop_();
#ifdef LCD_4DB
	LCD_DB &= 0x0F; //清高四位
	LCD_DB |= (cmd & 0xF0); //送高四位
	_nop_();
	LCD_E = 1;
	_nop_();
	LCD_E = 0;
	_nop_();
	LCD_DB &= 0x0F; //清高四位
	LCD_DB |= (cmd << 4); //送低四位
#endif
#ifdef LCD_8DB
	LCD_DB = cmd;
#endif
	_nop_();
	LCD_E = 1;
	_nop_();
	LCD_E = 0;
}

/**
 * 设定下一个要写的字符的地址(DDRAM)
 */
void lcd_set_address(bit row, uchar col)
{
	if (row == 0)
		lcd_write_cmd(0x80 | col);
	else if (row == 1)
		lcd_write_cmd(0x80 | 0x40 | col);
}

/**
 * 写显示数据
 * RS=1 RW=0 E=下降沿 D0-D7=数据
 */
void lcd_write_dat(uchar dat)
{
	// while(lcd_busy());
	LCD_RS = 1;
	LCD_RW = 0;
	_nop_();
#ifdef LCD_4DB
	LCD_DB &= 0x0F; //清高四位
	LCD_DB |= (dat & 0xF0); //送高四位
	_nop_();
	LCD_E = 1;
	_nop_();
	LCD_E = 0;
	LCD_DB &= 0x0F; //清高四位
	LCD_DB |= (dat << 4); //送低四位
#endif
#ifdef LCD_8DB
	LCD_DB = dat;
#endif
	_nop_();
	LCD_E = 1;
	_nop_();
	LCD_E = 0;
}

/**
 * 读显示数据
 * RS=1 RW=1 E=1 D0-D7=数据
 */
// TODO 未完成
uchar lcd_read_dat()
{
	return 0;
}

/**
 * 向CGRAM中写入自定义字符
 * 无需处理地址
 * pos 0-7 写入自定义cgram中的位置
 */ 
void lcd_write_cgram(uchar pos, uchar *arr)
{
	uchar i;
	pos <<= 3; // pos *= 8, 获得地址
	pos |= 0x40; // 设定CGRAM地址命令
	for (i = 0; i < 8; i++)
	{
		lcd_write_cmd(pos);
		lcd_write_dat(*arr);
		pos++;
		arr++;
	}
}

/**
 * 显示字符串
 * 不要忘记字符数组末尾要有'\0'
 */
void lcd_print(uchar *str)
{
	while ((*str) != '\0')
	{
		lcd_write_dat(*str);
		str++;
	}
}

/**
 * 在特定位置显示字符
 */
void lcd_disp(bit row, uchar col, uchar ch)
{
	lcd_set_address(row, col);
	lcd_write_dat(ch);
}
