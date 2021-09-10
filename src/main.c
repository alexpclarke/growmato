// Authors:   Alex Clarke
// Date:      2021-09-09

/* ----- Includes ----- */
#include <avr/io.h>
#include "acx.h"

/* ----- Main Functions ----- */

void flash() {
  // DDRB |= 0b01000000;
  while (1) {
    // PORTB ^= 0b01000000;
    x_delay(100);
    // x_yield();
  }
}

int main(void) {
  x_init();

  x_new(1, flash, false);
  
  DDRB |= 0b00100000;
  while (1) {
    PORTB ^= 0b00100000;
    x_delay(1000);
  }
}