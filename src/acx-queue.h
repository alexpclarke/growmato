/* ----- Includes ----- */
#include <util/atomic.h>
#include <stdlib.h>
#include <stdbool.h>

#ifndef ACX_QUEUE_H_
  #define ACX_QUEUE_H_

  // The size of each serial buffer.
  #define BUFFER_SIZE 64

  // Masks for empty and full flags.
  #define Q_EMPTY 0b00000001
  #define Q_FULL 0b00000010

  // Defines the queue control block
  typedef struct {
    uint8_t   in;         // index of next char to be placed in queue (if not full)
    uint8_t   out;        // index of next char to be removed from queue (if not empty)
    uint8_t   flags;      // stores full and empty flags
    uint16_t  available;  // number of bytes available to be read from queue
    uint8_t*  buffer;     // pointer to queue data buffer
  } QCB;

  bool Q_init(QCB qcb, uint8_t* buffer_);
  bool Q_putc(QCB qcb, uint8_t val);
  bool Q_getc(QCB qcb, uint8_t* valPtr);
#endif

