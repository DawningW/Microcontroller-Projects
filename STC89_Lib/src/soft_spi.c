#include "soft_spi.h"

#if COMPILE_SOFT_SPI == 1

// 延时5us
#define sspi_delay() do { _nop_(); _nop_(); _nop_(); _nop_(); } while (0)

void sspi_init()
{
    SPI_SCLK = SPI_CPOL;
    SPI_MOSI = 1;
    SPI_MISO = 1;
    SPI_CS = 1;
}

uint8_t sspi_read_and_write(uint8_t write_dat)
{
    uint8_t i, read_dat = 0;
    SPI_SCLK = SPI_CPOL;
    for (i = 0; i < 8; i++)
    {
#if SPI_CPHA == 0
        read_dat <<= 1;
        read_dat |= SPI_MISO;
#endif
        if (write_dat & 0x80)
            SPI_MOSI = 1;
        else
            SPI_MOSI = 0;
        write_dat <<= 1;
        sspi_delay();
        SPI_SCLK = !SPI_CPOL;
        sspi_delay();
#if SPI_CPHA == 1
        read_dat <<= 1;
        read_dat |= SPI_MISO;
#endif
        SPI_SCLK = SPI_CPOL;
    }
    return read_dat;
}

#endif // COMPILE_SOFT_SPI
