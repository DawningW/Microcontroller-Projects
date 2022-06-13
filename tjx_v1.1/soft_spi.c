#include "soft_spi.h"

static void sspi_delay(uint16_t t)
{
    uint8_t i;
    while (t--)
    {
        for (i = 0; i < 5; i++);
    }
}

uint8_t sspi_send_and_read_data(uint8_t dat)
{
    uint8_t i;
    uint8_t ret = 0;
    SPI_SCK = 0;
    for (i = 0; i < 8; i++)
    {
        SPI_MOSI = dat & 0x01;
        SPI_SCK = 1;
        sspi_delay(1);
        SPI_SCK = 0;
        dat >>= 1;
        sspi_delay(1);
        ret |= ((uint8_t) SPI_MISO << i);
    }
    sspi_delay(1);
    return ret;
}
