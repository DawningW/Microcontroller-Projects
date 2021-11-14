#include "usart.h"	

// 加入以下代码, 支持printf函数, 而不需要选择use MicroLIB
#pragma import(__use_no_semihosting)
// 标准库需要的支持函数
struct __FILE
{
    int handle;
};
FILE __stdout;
// 定义_sys_exit()以避免使用半主机模式
void _sys_exit(int x)
{
    x = x;
}
// 重定义fputc函数
int fputc(int ch, FILE *f)
{
    uart_send((uint8_t) ch);
    return ch;
}

uint8_t USART_RX_BUF[USART_BUF_LEN + 1];
// 接收状态标记
// bit15   接收完成标志
// bit14   接收到0x0d
// bit13~0 接收到的有效字节数目
uint16_t USART_RX_STA = 0;

void uart_init(uint32_t bound)
{
    USART_RX_BUF[USART_BUF_LEN] = 0;
    
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);
    // 先使能时钟, 再设置引脚复用
    
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    USART_InitTypeDef USART_InitStructure;
    USART_InitStructure.USART_BaudRate = bound;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1, &USART_InitStructure);
    
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    USART_Cmd(USART1, ENABLE);
    USART_ClearFlag(USART1, USART_FLAG_TC);
}

void uart_send(uint8_t c)
{
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    USART_SendData(USART1, c);
    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
}

void uart_print(const char *str)
{
    while (*str) uart_send(*str++);
}

void uart_println(const char *str)
{
    uart_print(str);
    uart_print("\r\n");
}

void USART1_IRQHandler(void)
{
    uint8_t ch;
    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) // 接收中断(接收到的数据必须是0x0d 0x0a结尾)
    {
        ch = USART_ReceiveData(USART1);
        if ((USART_RX_STA & 0x8000) == 0)
        {
            if (USART_RX_STA & 0x4000)
            {
                if (ch != 0x0a) USART_RX_STA = 0;
                else USART_RX_STA |= 0x8000;
            }
            else
            {
                if (ch == 0x0d) USART_RX_STA |= 0x4000;
                else
                {
                    USART_RX_BUF[USART_RX_STA & 0x3fff] = ch;
                    USART_RX_STA++;
                    if (USART_RX_STA > USART_BUF_LEN) USART_RX_STA = 0;
                }
            }
        }
    }
}
