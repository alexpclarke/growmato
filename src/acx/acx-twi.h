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
  #define TWS             (TWSR & 0xF8)

  // TWI states.
  #define TWI_READY     0
  #define TWI_INIT      1
  #define TWI_REPSTART  2
  #define TWI_MRX       3
  #define TWI_MTX       4
  #define TWI_SRX       5
  #define TWI_STX       6

  /* ----- TWI Statuses ----- */

  // Master
  #define TWS_START                  0x08   // Start sent
  #define TWS_REP_START              0x10   // Repeated Start sent
  // Master Tramsmitter
  #define TWS_MT_SLA_ACK             0x18   // SLA+W sent and ACK received
  #define TWS_MT_SLA_NACK            0x20   // SLA+W sent and NACK received
  #define TWS_MT_DATA_ACK            0x28   // DATA sent and ACK received
  #define TWS_MT_DATA_NACK           0x30   // DATA sent and NACK received
  #define TWS_MT_ARB_LOST            0x38
  // Master Reciever
  #define TWS_MR_SLA_ACK             0x40   // SLA+R sent, ACK received
  #define TWS_MR_SLA_NACK            0x48   // SLA+R sent, NACK received
  #define TWS_MR_DATA_ACK            0x50   // SLA+R sent, NACK received
  #define TWS_MR_DATA_NACK           0x58   // Data received, NACK returned
  #define TWS_MR_ARB_LOST            0x38
  // Slave Reciever
  #define TWS_SR_SLA_ACK             0x60
  #define TWS_SR_ARB_LOST_SLA_ACK    0x68
  #define TWS_SR_GCALL_ACK           0x70
  #define TWS_SR_ARB_LOST_GCALL_ACK  0x78
  #define TWS_SR_DATA_ACK            0x80
  #define TWS_SR_DATA_NACK           0x88
  #define TWS_SR_GCALL_DATA_ACK      0x90
  #define TWS_SR_GCALL_DATA_NACK     0x98
  #define TWS_SR_STOP                0xA0
  // Slave Tramsmitter
  #define TWS_ST_SLA_ACK             0xA8
  #define TWS_ST_ARB_LOST_SLA_ACK    0xB0
  #define TWS_ST_DATA_ACK            0xB8 
  #define TWS_ST_DATA_NACK           0xC0
  #define TWS_ST_LAST_DATA           0xC8
  // Misc.
  #define TWS_NO_INFO                0xF8
  #define TWS_BUS_ERROR              0x00

  void x_twi_init(void);
  void x_twi_disable(void);
  void twi_setAddress(uint8_t address);
  void x_twi_set_frequency(uint32_t freq, uint8_t prescalar);  
  void x_twi_start(void);
  void x_twi_stop(void);
  void x_twi_write(uint8_t u8data);
  void x_twi_ack(void);
  void x_twi_nack(void);
  bool x_twi_putc(uint8_t address, uint8_t data, bool sensStop);
  bool x_twi_puts(uint8_t address, uint8_t* data, uint8_t length, bool sendStop);
  bool x_twi_getc(uint8_t address, uint8_t* dest);
#endif