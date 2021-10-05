/* ----- Includes ----- */
#include "acx/acx.h"
#include "acx/acx-usart.h"
#include "acx/acx-twi.h"
// #include "LCD.c"


void LCD(void);

/* ----- Main Functions ----- */

void flash() {
  DDRB |= 0b00100000;
  while (1) {
    PORTB ^= 0b00100000;
    x_delay(500);
  }
}

void flash0() {
  DDRB |= 0b00000001;
  while (1) {
    PORTB ^= 0b00000001;
    x_delay(500);
  }
}

void flash1() {
  DDRB |= 0b00000010;
  x_delay(500);
  while (1) {
    PORTB ^= 0b00000010;
    x_delay(500);
  }
}

void flash2() {
  DDRB |= 0b00000100;
  while (1) {
    PORTB ^= 0b00000100;
    x_delay(500);
  }
}

void USART0() {
  x_usart_init(115200, 8, P_NONE, 1, false);

  while (1) {
    x_usart_puts("test\n");
    x_delay(2000);
  }
}

void USART1() {
  // x_usart_init(115200, 8, P_NONE, 1, false);
  // int maxlen = 20;
  // char* buff = calloc(maxlen, sizeof(char));

  // uint8_t buff;
  while (1) {
    // if (x_usart_getc(&buff)) {
    //   // x_usart_putc(buff);
    //   x_usart_putc('x');
    // } else {
    //   x_yield();
    // }
    x_yield();
  }
}

int main(void) {
  x_init();
  
  // x_new(1, flash0, true);
  // x_new(2, flash1, true);
  // x_new(3, flash2, true);
  // x_new(4, flash, true);
  x_new(0, LCD, true);
}