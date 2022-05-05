#include "LCD12864.h"
#include "LCDFONT.h"
#include "delay.h"

void Delay1(u8 delay)
{
	u8 j;
	while (delay--)
	{
		for (j = 0; j < 1; j++);
	}
}

void LCD12864_GPIO_config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.Mode = GPIO_PullUp;
	GPIO_InitStructure.Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4;
	GPIO_Inilize(GPIO_P0, &GPIO_InitStructure);
	GPIO_InitStructure.Mode = GPIO_PullUp;
	GPIO_InitStructure.Pin = GPIO_Pin_All;
	GPIO_Inilize(GPIO_P6, &GPIO_InitStructure);
}

// LCD忙检测
// 但是仿真不需要忙检测
void LCD12864_is_busy()
{
	Lcd12864DataIn();
	Lcd12864RSLow();
	Lcd12864RWHigh();
	Lcd12864ENHigh();
	while (Lcd12864Data() & 0x80);
	Lcd12864ENLow();
	Lcd12864DataOut();
}

void LCD12864_write_cmd(u8 cmd)
{
	// LCD12864_is_busy();
	Lcd12864RSLow();
	Lcd12864RWLow();
	Lcd12864WriteData(cmd);
	Lcd12864ENHigh();
	Delay1(5);
	Lcd12864ENLow();
}

void LCD12864_write_data(u8 dat)
{
	// LCD12864_is_busy();
	Lcd12864RSHigh();
	Lcd12864RWLow();
	Lcd12864WriteData(dat);
	Lcd12864ENHigh();
	Delay1(5);
	Lcd12864ENLow();
}

void LCD12864_init(void)
{
	Lcd12864DataOut();
	Lcd12864CS1Low();
	Lcd12864CS2Low();
	Lcd12864ENLow();
	Delay1(10);
	LCD12864_write_cmd(0x3f); // 开显示
	LCD12864_write_cmd(0xc0); // 设置显示起始行
	LCD12864_clear();		  // 清屏
}

// 清屏
void LCD12864_clear(void)
{
	u8 i, j;
	Lcd12864CS1High();
	Lcd12864CS2High();
	for (i = 0; i < 8; i++)
	{								  // 共8页
		LCD12864_write_cmd(0xB8 + i); // 光标到i页
		LCD12864_write_cmd(0x40);	  // 光标到页首
		for (j = 0; j < 64; j++)
			LCD12864_write_data(0);   // 写0清屏
	}
}

/********* 任意尺寸点阵显示函数 *********|
|    参数：显示位置：row页数，col列号，   |
|    字符大小：x横向像素，y纵向页数       |
|    显示内容：tab字模表                 |
****************************************/
void LCD12864_showXY(u8 row, u8 col, u8 x, u8 y, const u8 *tab)
{
	u8 i, j, right;
	// 置左右屏标志
	if (col < 64)
	{
		right = 0;
	}
	else
	{
		col -= 64;
		right = 1;
	}
	for (i = 0; i < y; i++)
	{
		if (right)
		{
			Lcd12864CS2Low();
			Lcd12864CS1High();
		}
		else
		{
			Lcd12864CS1Low();
			Lcd12864CS2High();
		}
		LCD12864_write_cmd(0xb8 + row + i); // 行
		LCD12864_write_cmd(0x40 + col);		// 列
		for (j = 0; j < x; j++)
		{
			if (col + j < 64)
			{
				// 未跨屏
				LCD12864_write_data(tab[i * x + j]);
			}
			else if (right == 0)
			{
				// 从左屏跨右屏
				Lcd12864CS2Low();
				Lcd12864CS1High();
				LCD12864_write_cmd(0xb8 + row + i);
				LCD12864_write_cmd(0x40 + (col + j - 64));
				LCD12864_write_data(tab[i * x + j]);
			}
		}
	}
}

u8 en_char_index(const u8 s)
{
	return s - 32;
}

// 返回一个16*16字符索引位置
u8 cn_char_index(const u8 *s)
{
	u8 i;
	for (i = 0; zm16x16_index[i] != 0; i += 2)
		if (zm16x16_index[i] == *s && zm16x16_index[i + 1] == *(s + 1))
			break;
	return i / 2;
}

// 显示字符串
void LCD12864_showStr(u8 row, u8 col, const u8 *s)
{
	u8 i;
	// 换行处理
    if (col >= 120)
    {
        col = 0;
        row += 2;
    }
	for (i = 0; s[i] != 0; i++)
	{
		if (s[i] < 0x80)
		{
			LCD12864_showXY(row, col, 8, 2, zm8x16_Table[en_char_index(s[i])]);
			col += 8;
		}
		else
		{
			LCD12864_showXY(row, col, 16, 2, zm16x16_Table[cn_char_index(&s[i])]);
			col += 16;
			i += 1;
		}
		if (col >= 120)
		{
			// 换行处理
			col = 0;
			row += 2;
		}
	}
}
