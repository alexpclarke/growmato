#include "acx/acx.h"
#include "acx/acx-usart.h"
#include "acx/acx-twi.h"

#define LCD_ADDRESS 0x27

#define LCD_CLEARDISPLAY    0x01
#define LCD_RETURNHOME      0x02
#define LCD_ENTRYMODESET    0x04
#define LCD_DISPLAYCONTROL  0x08
#define LCD_CURSORSHIFT     0x10
#define LCD_FUNCTIONSET     0x20
#define LCD_SETCGRAMADDR    0x40
#define LCD_SETDDRAMADDR    0x80

void LCD() {
  x_usart_init(115200, 8, P_NONE, 1, false);

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
    if (x_twi_writeTo(LCD_ADDRESS, &command, 1)) {
      // x_usart_puts("success\n");
    } else {
      // x_usart_puts("fail\n");
    }
    x_delay(1000);
  }
}