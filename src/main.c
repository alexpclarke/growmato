// Authors:   Alex Clarke
// Date:      2021-09-10

/* ----- Includes ----- */
#include <avr/io.h>
#include "acx.h"

/* ----- Main Functions ----- */

void flash() {
  DDRB |= 0b00100000;
  while (1) {
    PORTB ^= 0b00100000;
    x_delay(500);
  }
}

int main(void) {
  x_init();
  x_new(0, flash, true);
}