#ifndef __SOFT_SPI_H__
#define __SOFT_SPI_H__

#include "system.h"

#define SPI_CS   P25
#define SPI_SCLK P27
#define SPI_MOSI P26
#define SPI_MISO P26
// 空闲时时钟极性(CPOL) 0为低电平, 1为高电平
#define CPOL 0
// 数据有效时钟缘相位(CPHA) 0为奇数缘, 1为偶数缘
#define CPHA 0
// 模式0: CPOL=0, CPHA=0
// 模式1: CPOL=0, CPHA=1
// 模式2: CPOL=1, CPHA=0
// 模式3: CPOL=1, CPHA=1

void sspi_init();
// 工作在模式2 (CPHA=0, CPOL=1)
void sspi_send_byte(uint8_t byte);
uint8_t sspi_read_byte();
// 工作在由宏指定的模式上
uint8_t sspi_read_and_write(uint8_t write_dat);
// 你问我为啥有两组, 当然是从不同的地方抄的啦

#endif
