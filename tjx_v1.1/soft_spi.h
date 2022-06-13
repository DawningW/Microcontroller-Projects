#ifndef __SOFT_SPI_H__
#define __SOFT_SPI_H__

#include "system.h"

#define SPI_CS   P25
#define SPI_SCK  P27
#define SPI_MOSI P26
#define SPI_MISO P26

void sspi_init();
void sspi_send_byte(uint8_t byte);
uint8_t sspi_read_byte();

#endif
