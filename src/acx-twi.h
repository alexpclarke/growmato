// Author:  Alexander Clarke
// Date:    2021-09-16

/* ----- Includes ----- */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include <stdlib.h>
#include <stdbool.h>
#include "acx.h"

#ifndef ACX_TWI_H_
  #define ACX_TWI_H_

  void x_twi_init(void);
  void x_twi_start(void);
  void x_twi_stop(void);
  void x_twi_write(uint8_t u8data);
  uint8_t x_twi_read_ack(void);
  uint8_t x_twi_read_nack(void);
  uint8_t x_twi_getStatus(void);

  bool x_twi_putc(uint8_t device_address, uint8_t c);
  int x_twi_puts(uint8_t device_address, uint8_t* buffer);
  bool x_twi_getc(uint8_t device_address, uint8_t* dest);
  int x_twi_gets(uint8_t device_address, uint8_t* dest_buffer);

#endif