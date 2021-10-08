// Author:  Alexander Clarke
// Date:    2021-09-16

/* ----- Includes ----- */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include <stdlib.h>
#include <stdbool.h>
#include "acx.h"
#include "acx-buffer.h"

#ifndef ACX_TWI_H_
  #define ACX_TWI_H_

  #define TWI_PRESCALAR   0x00
  #define TWI_FREQUENCY   10000L
  #define TWIS            (TWSR & 0xF8)

  // TWI states.
  #define TWI_READY 0
  #define TWI_MRX   1
  #define TWI_MTX   2
  #define TWI_SRX   3
  #define TWI_STX   4

  /* ----- TWI Statuses ----- */

  // Master
  #define TWIS_START                  0x08 
  #define TWIS_REP_START              0x10
  // Master Tramsmitter
  #define TWIS_MT_SLA_ACK             0x18
  #define TWIS_MT_SLA_NACK            0x20
  #define TWIS_MT_DATA_ACK            0x28
  #define TWIS_MT_DATA_NACK           0x30
  #define TWIS_MT_ARB_LOST            0x38
  // Master Reciever
  #define TWIS_MR_SLA_ACK             0x40
  #define TWIS_MR_SLA_NACK            0x48
  #define TWIS_MR_DATA_ACK            0x50
  #define TWIS_MR_DATA_NACK           0x58
  #define TWIS_MR_ARB_LOST            0x38
  // Slave Reciever
  #define TWIS_SR_SLA_ACK             0x60
  #define TWIS_SR_ARB_LOST_SLA_ACK    0x68
  #define TWIS_SR_GCALL_ACK           0x70
  #define TWIS_SR_ARB_LOST_GCALL_ACK  0x78
  #define TWIS_SR_DATA_ACK            0x80
  #define TWIS_SR_DATA_NACK           0x88
  #define TWIS_SR_GCALL_DATA_ACK      0x90
  #define TWIS_SR_GCALL_DATA_NACK     0x98
  #define TWIS_SR_STOP                0xA0
  // Slave Tramsmitter
  #define TWIS_ST_SLA_ACK             0xA8
  #define TWIS_ST_ARB_LOST_SLA_ACK    0xB0
  #define TWIS_ST_DATA_ACK            0xB8 
  #define TWIS_ST_DATA_NACK           0xC0
  #define TWIS_ST_LAST_DATA           0xC8
  // Misc.
  #define TWIS_NO_INFO                0xF8
  #define TWIS_BUS_ERROR              0x00


  #define TWIS_W

  void x_twi_init(void);
  void x_twi_start(void);
  void x_twi_stop(void);
  void x_twi_write(uint8_t u8data);
  uint8_t x_twi_read_ack(void);
  uint8_t x_twi_read_nack(void);
  uint8_t x_twi_get_status(void);
  void x_twi_set_frequency(uint32_t, uint8_t);
  void twi_setAddress(uint8_t);
  bool x_twi_writeTo(uint8_t address, uint8_t* data, uint8_t length);

  bool x_twi_putc(uint8_t device_address, uint8_t c);
  int x_twi_puts(uint8_t device_address, uint8_t* buffer);
  bool x_twi_getc(uint8_t device_address, uint8_t* dest);
  int x_twi_gets(uint8_t device_address, uint8_t* dest_buffer);

#endif