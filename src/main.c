// Authors:   Alex Clarke
// Date:      2021-09-16

/* ----- Includes ----- */
#include "acx.h"
#include "acx-serial.h"

/* ----- Main Functions ----- */

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

  while (1) {
    x_serial_puts("test");
    x_delay(2000);
  }
}

int main(void) {
  x_init();

  x_new(1, flash0, true);
  x_new(2, flash1, true);
  x_new(3, flash2, true);
  x_new(0, USART, true);
}