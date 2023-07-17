#include "delay.h"

void delay(uint16_t nClk)
{
  TACCR0 = TAR + nClk * 32;
  TACCTL0 &= ~CCIFG;
  while (!(TACCTL0 & CCIFG));
}

void delay_us(uint16_t us)
{
  for (uint16_t j = 0; j < 5 * us; j++)
  {
    _NOP();
  }
}

void delay_ms(uint16_t ms)
{
  for (uint16_t i = 0; i < ms; i++)
  {
    for (uint16_t j = 0; j < 160; j++)
      _NOP();
  }
}
