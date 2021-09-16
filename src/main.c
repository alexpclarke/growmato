// Authors:   Alex Clarke
// Date:      2021-09-16

/* ----- Includes ----- */
#include <avr/io.h>
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

int main(void) {
  x_init();
  x_new(1, flash, true);

  x_serial_init(115200, SERIAL_8N1);
  int maxlen = 20;
  static char buff[20];
  // uint8_t buff[4] = {'t', 'e', 's', 't'};

  int count = 0;
  while (1) {
    count = x_serial_gets(maxlen, buff);
    if (count > 0) {
      x_serial_puts(buff);
    }
    x_yield();
  }
}