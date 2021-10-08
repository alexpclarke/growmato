/* ----- Includes ----- */
#include "acx/acx.h"
#include "acx/acx-usart.h"
// #include "acx/acx-twi.h"
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

void USART_print() {
  x_usart_init(9600, 8, P_NONE, 1, false);

  while (1) {
    x_usart_puts("test\n");
    x_delay(2000);
  }
}

void USART_echo() {
  x_usart_init(9600, 8, P_NONE, 1, false);

  uint8_t maxlen = 20;
  char* buff = calloc(maxlen, sizeof(char));

  while (1) {
    if (x_usart_gets(maxlen, buff)) {
      x_usart_puts(buff);
    }
    x_yield();
  }
}

int main(void) {
  x_init();
  
  x_new(0, USART_echo, true);
}