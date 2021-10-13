/* ----- Includes ----- */
#include <util/atomic.h>
#include <stdlib.h>
#include <stdbool.h>
#include "acx.h"

#ifndef ACX_QUEUE_H_
  #define ACX_QUEUE_H_

  // Buffer assignment.
  #define USART_RX_BUFFER 0
  #define USART_TX_BUFFER 1
  #define TWI_RX_BUFFER 2
  #define TWI_TX_BUFFER 3

  // The size of each serial buffer.
  #define B_SIZE 0x40
  #define B_MAX 4
  #define B_START (T_START - (B_MAX * B_SIZE))
  #define BN_IN(n) _SFR_MEM8(B_START + (B_SIZE * n))
  #define BN_OUT(n) _SFR_MEM8(B_START + (B_SIZE * n) + 1)
  #define BN_AVAILABLE(n) _SFR_MEM8(B_START + (B_SIZE * n) + 2)
  #define BN_BASE(n) ((uint8_t*)(B_START + (B_SIZE * n) + 3))

  void b_init(uint8_t b_id);
  bool b_putc(uint8_t b_id, uint8_t val);
  bool b_getc(uint8_t b_id, uint8_t* valPtr);
  bool b_isEmpty(uint8_t b_id);
#endif

