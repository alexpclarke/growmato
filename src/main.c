// Authors:   Alex Clarke
// Date:      2021-09-16

/* ----- Includes ----- */
#include "acx.h"
#include "acx-serial.h"

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

void USART() {
  x_serial_init(115200, 8, P_NONE, 1, false);

  char* buff = "test";
  while (1) {
    // buff = x_serial_getc();
    // if (buff != -1) {
      // x_serial_putc(buff);
    // }
    x_serial_puts(buff);
    // x_serial_putc('x');
    x_delay(500);
  }
}

int main(void) {
  x_init();

  // x_new(1, flash, true);
  // x_new(2, flash1, true);
  // x_new(0, flash2, true);
  x_new(0, USART, true);
}