// Author:  Alexander Clarke
// Date:    2021-09-16

/* ----- Includes ----- */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include <stdlib.h>
#include <stdbool.h>
#include "acx.h"

#ifndef ACX_I2C_H_
  #define ACX_I2C_H_

  bool x_i2c_init();
  bool x_i2c_putc(uint8_t device_address, uint8_t c);
  int x_i2c_puts(uint8_t device_address, uint8_t* buffer);
  bool x_i2c_getc(uint8_t device_address, uint8_t* dest);
  int x_i2c_gets(uint8_t device_address, uint8_t* dest_buffer);

#endif