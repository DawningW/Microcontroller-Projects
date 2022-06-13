#ifndef __SOFT_SPI_H__
#define __SOFT_SPI_H__

#include "system.h"

#define SPI_CS   P14
#define SPI_SCK  P15
#define SPI_MOSI P16
#define SPI_MISO P17

uint8_t sspi_send_and_read_data(uint8_t dat);

#endif
