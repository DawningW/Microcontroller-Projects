#include "epd.h"

#include "delay.h"

const uint8_t EPD_lut[30] =
{
  0x66, 0xa6, 0x6a, 0x66, 0x55, 0x99, 0xaa, 0x66,
  0x55, 0x95, 0xaa, 0xaa, 0x59, 0x55, 0xaa, 0x55,
  0x88, 0x11, 0x11, 0x88,
  0xf8, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0x8f, 0x8f,
};

static void EPD_WriteByte(uint8_t byte)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    if (byte & 0x80)
    {
      EPD_SDA_HIGH;
    }
    else
    {
      EPD_SDA_LOW;
    }
    byte <<= 1;
    EPD_SCL_HIGH;
    _NOP();
    EPD_SCL_LOW;
  }
}

static void EPD_WriteCMD(uint8_t cmd)
{
  EPD_DC_LOW;
  EPD_WriteByte(cmd);
}

static void EPD_WriteCMD1DAT(uint8_t cmd, uint8_t data)
{
  EPD_WriteCMD(cmd);
  EPD_DC_HIGH;
  EPD_WriteByte(data);
}

static void EPD_WriteCMD2DAT(uint8_t cmd, uint8_t data, uint8_t data2)
{
  EPD_WriteCMD1DAT(cmd, data);
  EPD_WriteByte(data2);
}

static void EPD_WriteCMD3DAT(uint8_t cmd, uint8_t data, uint8_t data2, uint8_t data3)
{
  EPD_WriteCMD1DAT(cmd, data);
  EPD_WriteByte(data2);
  EPD_WriteByte(data3);
}

static void EPD_WriteCMD4DAT(uint8_t cmd, uint8_t data, uint8_t data2, uint8_t data3, uint8_t data4)
{
  EPD_WriteCMD1DAT(cmd, data);
  EPD_WriteByte(data2);
  EPD_WriteByte(data3);
  EPD_WriteByte(data4);
}

static void EPD_WriteCMDnDAT(uint8_t cmd, const uint8_t *data, uint16_t length)
{
  EPD_WriteCMD(cmd);
  EPD_DC_HIGH;
  for (uint16_t i = 0; i < length; i++)
  {
    EPD_WriteByte(data[i]);
  }
}

void EPD_Init()
{
  P2DIR |= BIT3 | BIT4 | BIT6;
  P2OUT &= ~(BIT3 | BIT4 | BIT6);
  P3DIR |= BIT1 | BIT4 | BIT5 | BIT6 | BIT7;
  P3OUT &= ~(BIT1 | BIT4 | BIT5 | BIT6 | BIT7);

  EPD_RST_LOW;
  delay_ms(10);
  EPD_RST_HIGH;
  delay_ms(10);
  EPD_CS1_LOW;
  EPD_CS2_LOW;
  EPD_WriteCMD(0x12); // SWRESET
  while (EPD_IsBusy());
  EPD_WriteCMD3DAT(0x01, 0x2b, 0x01, 0x00); // Driver Output control
  EPD_WriteCMD1DAT(0x3a, 0x16); // Set dummy line period
  EPD_WriteCMD1DAT(0x3b, 0x08); // Set Gate line width
  EPD_WriteCMD1DAT(0x2c, 0xb9); // Write VCOM register
  EPD_WriteCMDnDAT(0x32, EPD_lut, sizeof(EPD_lut)); // Write LUT register
  EPD_WriteCMD1DAT(0x21, 0x83); // Display Update Control 1
  EPD_WriteCMD1DAT(0x3c, 0x33); // Border Waveform Control
  EPD_WriteCMD1DAT(0x11, 0x03); // Data Entry mode setting
  EPD_WriteCMD2DAT(0x44, 0x00, 0x18); // Set RAM X - address Start / End position
  EPD_WriteCMD4DAT(0x45, 0x00, 0x00, 0x2b, 0x01); // Set Ram Y - address Start / End position
  EPD_CS1_HIGH;
  EPD_CS2_HIGH;
}

void EPD_Sleep()
{
  EPD_CS1_LOW;
  EPD_CS2_LOW;
  EPD_WriteCMD1DAT(0x10, 0x01); // Enter Deep Sleep mode
  EPD_CS1_HIGH;
  EPD_CS2_HIGH;
}

bool EPD_IsBusy()
{
  return EPD_BUSY;
}

void EPD_Clear()
{
  EPD_CS1_LOW;
  EPD_CS2_LOW;
  EPD_WriteCMD1DAT(0x4e, 0x00);
  EPD_WriteCMD2DAT(0x4f, 0x00, 0x00);
  EPD_WriteCMD(0x24);
  EPD_DC_HIGH;
  for (uint16_t i = 0; i < EPD_BUFFER_SIZE; i++)
  {
    EPD_WriteByte(0xff);
  }
  EPD_CS1_HIGH;
  EPD_CS2_HIGH;
}

void EPD_WriteLeft(const uint8_t *dat)
{
  EPD_CS1_LOW;
  EPD_WriteCMD1DAT(0x4e, 0x00);
  EPD_WriteCMD2DAT(0x4f, 0x00, 0x00);
  EPD_WriteCMDnDAT(0x24, dat, EPD_BUFFER_SIZE);
  EPD_CS1_HIGH;
}

void EPD_WriteRight(const uint8_t *dat)
{
  EPD_CS2_LOW;
  EPD_WriteCMD1DAT(0x4e, 0x00);
  EPD_WriteCMD2DAT(0x4f, 0x00, 0x00);
  EPD_WriteCMDnDAT(0x24, dat, EPD_BUFFER_SIZE);
  EPD_CS2_HIGH;
}

void EPD_Refresh()
{
  EPD_CS1_LOW;
  EPD_CS2_LOW;
  EPD_WriteCMD1DAT(0x22, 0xc7); // Display Update Control 2
  EPD_WriteCMD(0x20); // Master Activation
  while (EPD_IsBusy());
  EPD_CS1_HIGH;
  EPD_CS2_HIGH;
}
