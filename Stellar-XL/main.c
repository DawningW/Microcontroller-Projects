#include <msp430.h>
#include "delay.h"
#include "epd.h"
#include "images.h"

void main()
{
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;

  EPD_Init();
  EPD_Clear();
  EPD_WriteLeft(image_left);
  EPD_WriteRight(image_right);
  EPD_Refresh();
  EPD_Sleep();

  while (1)
  {

  }
}
