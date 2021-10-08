// Includes.
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include <stdlib.h>
#include <stdbool.h>
#include "acx.h"
#include "acx-buffer.h"

#ifndef ACX_USART_H_
  #define ACX_USART_H_

  /* ----- Framing Settings ----- */
  #define P_NONE 0
  #define P_EVEN 1
  #define P_ODD 2

  /* ----- Prototypes ----- */
  bool x_usart_init(uint32_t speed, uint8_t data_bits, uint8_t parity, uint8_t stop_bits, bool u2x);
  void x_usart_putc(uint8_t c);
  void x_usart_putc_hex(uint8_t c);
  void x_usart_putc_bin(uint8_t c);
  int x_usart_puts(char* buffer);
  bool x_usart_getc(uint8_t* dest);
  uint8_t x_usart_gets(uint8_t maxlen, char* dest_buffer);
  bool x_usart_set_baud(uint32_t speed, bool u2x);
  bool x_usart_set_data_bits(uint8_t data_bits);
  bool x_usart_set_parity(uint8_t parity);
  bool x_usart_set_stop_bits(uint8_t stop_bits);
#endif