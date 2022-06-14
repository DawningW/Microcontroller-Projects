#include "soft_spi.h"

// 延时5us
#define sspi_delay() do { _nop_(); _nop_(); _nop_(); _nop_(); } while (0)

void sspi_init()
{
    SPI_CS = 1;
    SPI_SCLK = CPOL;
    SPI_MOSI = 1;
    SPI_MISO = 1;
}

void sspi_send_byte(uint8_t byte)
{
    uint8_t i;
    for (i = 0; i < 8; i++)
    {
        SPI_SCLK = 0;
        SPI_MOSI = byte & 0x80;
        SPI_SCLK = 1;
        byte <<= 1;
        sspi_delay();
    }
}

uint8_t sspi_read_byte()
{
    uint8_t byte = 0, i;
    for (i = 0; i < 8; i++)
    {
        byte <<= 1;
        while (SPI_SCLK == 1);
        while (SPI_SCLK == 0);
        byte |= SPI_MISO;
    }
    return byte;
}

uint8_t sspi_read_and_write(uint8_t write_dat)
{
    uint8_t i, read_dat = 0;
    SPI_SCLK = CPOL;
    for (i = 0; i < 8; i++)
    {
#if CPHA == 0
        read_dat <<= 1;
        read_dat |= SPI_MISO;
#endif
        if (write_dat & 0x80)
            SPI_MOSI = 1;
        else
            SPI_MOSI = 0;
        write_dat <<= 1;
        sspi_delay();
        SPI_SCLK = !CPOL;
        sspi_delay();
#if CPHA == 1
        read_dat <<= 1;
        read_dat |= SPI_MISO;
#endif
        SPI_SCLK = CPOL;
    }
    return read_dat;
}
