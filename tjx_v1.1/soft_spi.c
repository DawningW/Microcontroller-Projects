#include "soft_spi.h"

static void sspi_delay()
{
    uint8_t i = 5;
    while (i--);
}

void sspi_init()
{

}

void sspi_send_byte(uint8_t byte)
{
    uint8_t i;
    SPI_SCK = 0;
    for (i = 0; i < 8; i++)
    {
        SPI_MOSI = byte & 0x01;
        SPI_SCK = 1;
        sspi_delay();
        SPI_SCK = 0;
        byte >>= 1;
        sspi_delay();
    }
}

uint8_t sspi_read_byte()
{
    uint8_t byte = 0, i;
    SPI_SCK = 0;
    for (i = 0; i < 8; i++)
    {
        SPI_SCK = 1;
        sspi_delay();
        SPI_SCK = 0;
        sspi_delay();
        byte |= ((uint8_t) SPI_MISO << i);
    }
    sspi_delay();
    return byte;
}
