#include "LCD.h"

void LCD() {
  x_twi_init();

  uint8_t command = LCD_CLEARDISPLAY;

  // x_usart_putc('\n');

  // x_twi_putc(0b00110000, LCD_ADDRESS);

  // x_usart_putc('\n');

  // //display off
  // x_twi_putc(0b00001000, LCD_ADDRESS);

  // x_usart_putc('\n');

  // //clear
  // x_twi_putc(0b00000001, LCD_ADDRESS);

  // x_usart_putc('\n');

  // //Entry mode set
  // x_twi_putc(0b00000110, LCD_ADDRESS);

  while (1) {
    // x_twi_writeTo(LCD_ADDRESS, &command, 1);
    x_delay(1000);
  }
}