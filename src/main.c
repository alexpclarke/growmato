// Authors:   Alex Clarke
// Date:      2021-09-16

/* ----- Includes ----- */
#include "acx.h"
// #include "acx-serial.h"

/* ----- Main Functions ----- */

void flash() {
  DDRB |= 0b00100000;
  while (1) {
    PORTB ^= 0b00100000;
    x_delay(2000);
  }
}


void USART() {
  // x_serial_init(115200, 8, P_NONE, 1, false);

  while (1) {
    // x_serial_putc('x');
    x_delay(2000);
  }
}


int main(void) {
  x_init();

  x_new(1, flash, true);
  // x_new(0, USART, true);
}