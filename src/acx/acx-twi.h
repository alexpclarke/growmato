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

  #define TWI_PRESCALAR 0x00
  #define TWI_FREQUENCY 
  #define TWI_S_START      0x08  // A START condition has been transmitted
  #define TWI_S_RSTART     0x10  //TWI repeat start
  #define TWI_S_SLAW_ACK   0x18   //slave ACK has been received
  #define TWI_S_SLAW_NACK  0x20
  #define TWI_S_DATA_ACK   0x28  //master ACK has been received
  #define TWI_S_DATA_NACK  0x30
  #define TWI_S_LOST       0x38
  #define TWI_S_SLAR_ACK   0x40
  #define TWI_S_SLAR_NACK  0x48

  void x_twi_init(void);
  void x_twi_start(void);
  void x_twi_stop(void);
  void x_twi_write(uint8_t u8data);
  uint8_t x_twi_read_ack(void);
  uint8_t x_twi_read_nack(void);
  uint8_t x_twi_get_status(void);

  bool x_twi_putc(uint8_t device_address, uint8_t c);
  int x_twi_puts(uint8_t device_address, uint8_t* buffer);
  bool x_twi_getc(uint8_t device_address, uint8_t* dest);
  int x_twi_gets(uint8_t device_address, uint8_t* dest_buffer);

#endif