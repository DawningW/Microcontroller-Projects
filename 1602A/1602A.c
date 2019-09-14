#include "1602A.h"
uchar code TAB_LCD_Hex[ ] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
/*************************************************************/
/*                                                           */
/* 延时子程序                                                */
/*                                                           */
/*************************************************************/
void delay(uchar x)
{
		uchar j;
    while((x--)!=0)
    {
     for(j=0;j<125;j++)            ;
    }
}
/*************************************************************/
/*                                                           */
/*检查LCD忙状态                                              */
/*lcd_busy为1时，忙，等待。lcd-busy为0时,闲，可写指令与数据  */
/*                                                           */
/*************************************************************/
static bit lcd_busy()
{                          
    bit result;     
    LCD_RS = 0;
    LCD_RW = 1;
    LCD_E = 1;
    delayNOP();
    result = (bit)(LCD_DB&0x80);
    LCD_E = 0;
    return result; 
}
/*******************************************************************/
/*                                                                 */
/*写指令数据到LCD                                                  */
/*RS=L，RW=L，E=高脉冲，D0-D7=指令码。                             */
/*                                                                 */
/*******************************************************************/
void lcd_wcmd(uchar cmd)
{                          
   while(lcd_busy());
    LCD_RS = 0;
    LCD_RW = 0;
    LCD_E = 0;
    _nop_();
    _nop_(); 
#ifdef LCD_8DB
    LCD_DB = cmd;
#endif
#ifdef LCD_4DB
LCD_DB &= 0x0F;            //清高四位
LCD_DB |= (cmd & 0xF0);    //送高四位
    delayNOP();
    LCD_E = 1;        //上跳沿时输入数据
    delayNOP();
    LCD_E = 0; 
LCD_DB &= 0x0F;            //清高四位
LCD_DB |= (cmd << 4);      //送低四位
#endif 
delayNOP();
    LCD_E = 1;         //上跳沿时输入数据
    delayNOP();
    LCD_E = 0; 
}
/*******************************************************************/
/*                                                                 */
/*写显示数据到LCD                                                  */
/*RS=H，RW=L，E=高脉冲，D0-D7=数据。                               */
/*                                                                 */
/*******************************************************************/
void lcd_wdat(uchar dat)
{                          
   while(lcd_busy());
    LCD_RS = 1;
    LCD_RW = 0;
    LCD_E = 0;
#ifdef LCD_8DB
    LCD_DB = dat;
#endif
#ifdef LCD_4DB
LCD_DB &= 0x0F;            //清高四位
LCD_DB |= (dat & 0xF0);   //送高四位数据
    delayNOP();
    LCD_E = 1;            //上跳沿时输入数据
    delayNOP();
    LCD_E = 0;
LCD_DB &= 0x0F;            //清高四位
LCD_DB |= (dat << 4);      //送低四位数据
#endif
delayNOP();
    LCD_E = 1;
    delayNOP();         //上跳沿时输入数据
    LCD_E = 0; 
}
/*************************************************************/
/*                                                           */
/* LCD初始化设定                                             */
/*                                                           */
/*************************************************************/
void lcd_init()
{ 
    delay(15);                //等待电源稳定    
    lcd_wcmd(LCD_MODE);      //设置显示模式
    delay(5);
    lcd_wcmd(LCD_MODE);         
    delay(5);
    lcd_wcmd(LCD_MODE);         
    delay(5);
    lcd_wcmd(0x0c);      //显示开，关光标
    delay(5);
    lcd_wcmd(0x06);      //移动光标
    delay(5);
    lcd_wcmd(0x01);      //清除LCD的显示内容
    delay(5);
}
/*************************************************************/
/*                                                           */
/* 设定显示位置                                              */
/*                                                           */
/*************************************************************/
void lcd_PrPos(uchar row,uchar col)
{                          
if(row == 0)
       lcd_wcmd(col | 0x80);     //数据指针=80+地址变量
    if(row == 1)
       lcd_wcmd(col | 0x80 | 0x40); 
}
/*************************************************************/
/*                                                           */
/* 显示字符串                                                */
/*                                                           */
/*************************************************************/
void lcd_PrString(uchar *prstring) //显示一个字符串的子程序
{
     while((*prstring) != '\0')
{
       lcd_wdat(*prstring);
    prstring++;
}
}

/*************************************************************/
/*                                                           */
/* 显示二进制字节                                            */
/*                                                           */
/*************************************************************/ 
void lcd_PrHexByte(uchar prbyte) //显示一个字符子程序
{
     ACC = prbyte;
ACC >>= 4;
     lcd_wdat(TAB_LCD_Hex[ACC]);   //显示高四位
     ACC = prbyte;
ACC &= 0x0F;
     lcd_wdat(TAB_LCD_Hex[ACC]);   //显示低四位
}
/*************************************************************/
/*                                                           */
/* 写入自定义字符                                           */
/*                                                           */
/*************************************************************/ 
void lcd_wCGRAM(uchar cg_pos,uchar *cg_arr)   //cg_pos 0-7 写入自定义cgram中的位置
{
	uchar i;
	cg_pos <<= 3;   // cg_pos = cg_pos * 8,转化成地址
	cg_pos |= 0x40; // 转化成命令字
	for(i = 0; i < 8; i++ )
	{
			 lcd_wcmd(cg_pos);
		 lcd_wdat(*cg_arr);
		 cg_pos++;
		 cg_arr++;
	}
}
