#ifndef _LCD1602_h_
#define _LCD1602_h_

#define uchar unsigned char 
#define uint unsigned int 



//------------------------------------------------------
void clr_lcd(void);
Init_lcd(void);
void wr_data(uchar dat);
void wr_commandn(uchar cmd);
void wr_command(uchar cmd);
void delay_nms(uint dly);



#endif