// Author:  Alexander Clarke
// Date:    2021-09-16

/* ----- Includes ----- */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include <stdlib.h>
#include <stdbool.h>
#include "acx.h"
#include "acx-queue.h"

#ifndef ACX_SERIAL_H_
  #define ACX_SERIAL_H_

  // Make sure NULL is defined for pointers.
  // #ifndef NULL
  //   #define NULL ((void*)0)
  // #endif

  /* ----- Serial Port Registers type ----- */
  typedef struct {
    volatile uint8_t  UCSRnA;
    volatile uint8_t  UCSRnB;
    volatile uint8_t  UCSRnC;
    volatile uint8_t  rsvd;
    volatile uint16_t UBRRn;
    volatile uint8_t  UDRn;
  } SERIAL_PORT_REGS;

  typedef struct {
    QCB* rxq;
    QCB* txq;
    SERIAL_PORT_REGS* regs;
  } SERIAL_PORT;

  /* ----- Framing Settings ----- */
  #define P_NONE 0
  #define P_EVEN 1
  #define P_ODD 2

  /* ----- Prototypes ----- */
  bool x_serial_init(uint32_t speed, uint8_t data_bits, uint8_t parity, uint8_t stop_bits, bool u2x);
  int x_serial_getc();
  int x_serial_putc(char c);
  int x_serial_gets(int maxlen, char* buffer);
  int x_serial_puts(char* buffer);
  uint16_t x_calc_baud(uint32_t speed, bool u2x);
#endif