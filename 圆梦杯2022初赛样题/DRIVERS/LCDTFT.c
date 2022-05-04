#include "LCDTFT.h"
#include "FONTTFT.h"

static u32 abs(s32 res)
{
	return res < 0 ? -res : res;
}

void TFT_GPIO_config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.Mode = GPIO_PullUp;
	GPIO_InitStructure.Pin = GPIO_Pin_All;
	GPIO_Inilize(GPIO_P7, &GPIO_InitStructure);
	GPIO_InitStructure.Mode = GPIO_PullUp;
	GPIO_InitStructure.Pin = GPIO_Pin_All;
	GPIO_Inilize(GPIO_P6, &GPIO_InitStructure);
	GPIO_InitStructure.Mode = GPIO_PullUp;
	GPIO_InitStructure.Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;
	GPIO_Inilize(GPIO_P0, &GPIO_InitStructure);
}

static void Delay(u8 delay)
{
	u8 j;
	while (delay--)
		for (j = 0; j < 1; j++);
}

void TFT_WR_REG(u16 cmd)
{
	TFTCSLow();
	TFTRSLow();
	TFTRWLow();
	TFTDataLow = cmd;
	TFTDataHigh = cmd >> 8;
	TFTCSHigh();
	Delay(8);
}

void TFT_WR_Data(u16 dat)
{
	TFTCSLow();
	TFTRSHigh();
	TFTRWLow();
	TFTDataLow = dat;
	TFTDataHigh = dat >> 8;
	TFTCSHigh();
	Delay(5);
}

void TFT_Set_Window(u16 startX, u16 startY, u16 endX, u16 endY)
{
	TFT_WR_REG(0x0020);
	TFT_WR_Data(startX);
	TFT_WR_REG(0x0021);
	TFT_WR_Data(endX);
	TFT_WR_REG(0x0022);
	TFT_WR_Data(startY);
	TFT_WR_REG(0x0023);
	TFT_WR_Data(endY);
	TFT_WR_REG(0x002F);
}

void TFT_SetCursor(u16 xCursor, u16 yCursor)
{
	TFT_WR_REG(0x0024);
	TFT_WR_Data(xCursor);
	TFT_WR_REG(0x0025);
	TFT_WR_Data(yCursor);
	TFT_WR_REG(0x002F);
}

void TFT_DrawPoint(u16 x, u16 y, u16 color)
{
	TFT_SetCursor(x, y);
	TFT_WR_Data(color);
}

void TFT_FillRect(u16 startX, u16 startY, u16 endX, u16 endY, u16 color)
{
	u16 i, j;
	TFT_Set_Window(startX, startY, endX, endY);
	for (i = startX; i <= endX; i++)
	{
		for (j = startY; j <= endY; j++)
		{
			TFT_WR_Data(color);
		}
	}
}

// 默认320*240大小
// 逐行从左到右扫描，逐列从上到下扫描
void TFT320_Clear(u16 color)
{
	TFT_FillRect(0, 0, 319, 239, color);
}

// 画线
// x1,y1: 起点坐标
// x2,y2: 终点坐标
void TFT_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2, u16 color)
{
	u16 x, y, t;
	if ((x1 == x2) && (y1 == y2))
		TFT_DrawPoint(x1, y1, color);
	else if (abs(y2 - y1) > abs(x2 - x1)) // 斜率大于1
	{
		if (y1 > y2)
		{
			t = y1;
			y1 = y2;
			y2 = t;
			t = x1;
			x1 = x2;
			x2 = t;
		}
		for (y = y1; y <= y2; y++) // 以y轴为基准
		{
			x = (u32)(y - y1) * (x2 - x1) / (y2 - y1) + x1;
			TFT_DrawPoint(x, y, color);
		}
	}
	else // 斜率小于等于1
	{
		if (x1 > x2)
		{
			t = y1;
			y1 = y2;
			y2 = t;
			t = x1;
			x1 = x2;
			x2 = t;
		}
		for (x = x1; x <= x2; x++) // 以x轴为基准
		{
			y = (u32)(x - x1) * (y2 - y1) / (x2 - x1) + y1;
			TFT_DrawPoint(x, y, color);
		}
	}
}

void TFT_WriteASCII16(u16 x, u16 y, u16 Color, u16 BK_Color, u8 *Matrix)
{
	u8 i, j;
	u8 temp;
	TFT_Set_Window(x, y, x + 7, y + 15);
	for (i = 0; i < 16; i++)
	{
		temp = Matrix[i];
		for (j = 0; j < 8; j++)
		{
			if (temp & (0x80 >> j))
			{
				TFT_WR_Data(Color);
			}
			else
			{
				TFT_WR_Data(BK_Color);
			}
		}
	}
}

void TFT_WriteCN16(u16 x, u16 y, u16 Color, u16 BK_Color, u8 *Matrix)
{
	u8 i, j, temp;
	TFT_Set_Window(x, y, x + 15, y + 15);
	for (i = 0; i < 32; i++)
	{
		temp = Matrix[i];
		for (j = 0; j < 8; j++)
		{
			if (temp & (0x80 >> j))
			{
				TFT_WR_Data(Color);
			}
			else
			{
				TFT_WR_Data(BK_Color);
			}
		}
	}
}

u8 en_char_index_TFT(u8 T)
{
	return T - 32;
}

// 返回一个16*16字符索引位置
u8 cn_char_index_TFT(u8 *s)
{
	u8 i;
	for (i = 0; zm16x16_index_TFT[i] != 0; i += 2)
		if (zm16x16_index_TFT[i] == *s && zm16x16_index_TFT[i + 1] == *(s + 1))
			break;
	return i / 2;
}

// 显示字符串
void TFT_ShowStr(u16 x, u16 y, u16 color, u16 bkColor, u8 *s)
{
	u8 i;
	for (i = 0; s[i] != 0; i++)
	{
		if (x >= 304)
		{
			// 换行处理
			x = 0;
			y += 16;
		}
		if (y >= 224)
		{
			y = 0;
		}
		if (s[i] < 0x80)
		{
			// 英文
			TFT_WriteASCII16(x, y, color, bkColor, zm8x16_Table_TFT[en_char_index_TFT(s[i])]);
			x += 8;
		}
		else
		{
			// 汉字
			TFT_WriteCN16(x, y, color, bkColor, zm16x16_Table_TFT[cn_char_index_TFT(&s[i])]);
			x += 16;
			i++;
		}
	}
}

// code 字符指针开始
// 从字库中查找出字模
// code 字符串的开始地址,GBK码
// mat  数据存放地址 (size/8+((size%8)?1:0))*(size) bytes大小
// void Get_Hz16Mat(const u8 *ccode, u8 *mat)
// {
// 	unsigned char qh, ql;
// 	unsigned char i;
// 	unsigned long foffset;
// 	qh = *ccode;
// 	ql = *(++ccode);
// 	if (qh < 0xA1 || ql < 0xA1 || ql == 0xff || qh > 0xf7 || (qh > 0xA9 && qh < 0xB0)) // 非 常用汉字
// 	{
// 		for (i = 0; i < 32; i++)
// 			*mat++ = 0x00; // 填充满格
// 		return;			   // 结束访问
// 	}
// 	ql -= 0xA1;
// 	/*
// 	if(qh<0xB0)qh-=0xA1;
// 	else qh-=0xA7;
// 	*/
// 	qh -= 0xA1;
// 	foffset = ((unsigned long)94 * qh + ql) * 32; // 得到字库中的字节偏移量
// 	SPIFlashReadData(foffset, mat, 32);
// }

// 显示任意16大小字符串
void TFT_ShowAnyStr(u16 x, u16 y, u16 color, u16 bkColor, const u8 *s)
{
	u8 i, mat[32];
	for (i = 0; s[i] != 0; i++)
	{
		if (x >= 304)
		{
			// 换行处理
			x = 0;
			y += 16;
		}
		if (y >= 224)
		{
			y = 0;
		}
		if (s[i] < 0x80)
		{
			// 英文
			TFT_WriteASCII16(x, y, color, bkColor, zm8x16_Table_TFT[en_char_index_TFT(s[i])]);
			x += 8;
		}
		// else
		// {
		// 	Get_Hz16Mat(&s[i], mat);
		// 	TFT_WriteCN16(x, y, color, bkColor, mat);
		// 	x += 16;
		// 	i++;
		// }
	}
}
