/* ----- Includes ----- */
#include "acx/acx.h"
#include "components/LCD.h"

void flash() {
  DDRB |= _BV(DDB5);
  while (1) {
    PORTB ^= _BV(PORTB5);
    x_delay(500);
  }
}

void USART_print() {
  x_usart_init(9600, 8, P_NONE, 1, false);

  while (1) {
    x_usart_puts("test\n");
    x_delay(2000);
  }
}

void USART_echo() {
  uint8_t maxlen = 20;
  char* buff = calloc(maxlen, sizeof(char));

  while (1) {
    if (x_usart_gets(maxlen, buff)) {
      x_usart_puts(buff);
    }
    x_yield();
  }
}

void LCD() {
  DDRB |= _BV(DDB0) | _BV(DDB1) | _BV(DDB2);

  LCD_init();
  LCD_set_backlight(true);
  LCD_gotoxy(3, 3);

  // LCD_write_command(LCD_CURSORSHIFT);

  // LCD_gotoxy(3, 3);
  // LCD_putc('c');

  // LCD_gotoxy(2, 4);

  // bool backlight = true;

  while (1) {
    // LCD_putc('x');
    // LCD_set_backlight(backlight);
    // backlight = !backlight;
    x_delay(2000);
  }
}

int main(void) {
  x_init();
  x_usart_init(9600, 8, P_NONE, 1, false);
  x_twi_init();
  
  x_new(1, flash, true);
  // x_new(2, USART_echo, true);
  x_new(0, LCD, true);


  // while(1) {
  //   x_yield();
  // }
}