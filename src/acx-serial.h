// Author:  Alexander Clarke
// Date:    2021-09-16

/* ----- Includes ----- */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include <stdlib.h>
#include <stdbool.h>
#include "acx-srf_defs.h"
#include "acx.h"
#include "acx-queue.h"

#ifndef ACX_SERIAL_H_
  #define ACX_SERIAL_H_

  /* ----- Framing Settings ----- */
  #define P_NONE 0
  #define P_EVEN 1
  #define P_ODD 2

  /* ----- Prototypes ----- */
  bool x_serial_init(uint32_t speed, uint8_t data_bits, uint8_t parity, uint8_t stop_bits, bool u2x);
  int x_serial_getc();
  int x_serial_putc(uint8_t c);
  int x_serial_gets(int maxlen, char* buffer);
  int x_serial_puts(char* buffer);
  uint16_t x_calc_baud(uint32_t speed, bool u2x);
#endif