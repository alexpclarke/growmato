#include "acx/acx.h"
#include "acx/acx-twi.h"

void LCD() {
  uint8_t LCD_ADDRESS = 0x27;
  uint8_t u8data;

  x_twi_init();


  x_twi_start();
  if (x_twi_getStatus() != 0x08) return false;
  x_twi_write((LCD_ADDRESS) | (uint8_t)((u16addr & 0x0700)>>7));




  while (1) {

    x_delay(2000);
  }
}