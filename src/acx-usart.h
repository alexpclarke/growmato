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

#ifndef ACX_USART_H_
  #define ACX_USART_H_

  /* ----- Framing Settings ----- */
  #define P_NONE 0
  #define P_EVEN 1
  #define P_ODD 2

  #define USART_RX_BUFF_START
  #define USART_TX_BUFF_START

  /* ----- Prototypes ----- */
  bool x_usart_init(uint32_t speed, uint8_t data_bits, uint8_t parity, uint8_t stop_bits, bool u2x);
  bool x_usart_putc(uint8_t c);
  int x_usart_puts(char* buffer);
  bool x_usart_getc(uint8_t* dest);
  int x_usart_gets(uint8_t maxlen, uint8_t* dest_buffer);
  uint16_t x_calc_baud(uint32_t speed, bool u2x);
#endif