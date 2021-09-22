#include "acx-queue.h"

// Initialize a specified Queue. Returns true if successful.
bool Q_init(QCB qcb, uint8_t* buffer_) {
  qcb.in = 0;
  qcb.out = 0;
  qcb.flags = Q_EMPTY;
  qcb.available = 0;
  qcb.buffer = buffer_;
  return true;
}

// Gets a char from specified queue.
bool Q_getc(QCB qcb, uint8_t* valPtr) {
  // Check that there are available characters to get. If there are none, fail.
  if (qcb.flags & Q_EMPTY) return false;

  // Enter atomic block while we change things.
  ATOMIC_BLOCK (ATOMIC_RESTORESTATE) {
    // Copy the value in the buffer at the out index to pval.
    *valPtr = qcb.buffer[qcb.out];

    // Advance the out index.
    qcb.out++;
    qcb.out %= BUFFER_SIZE;

    // Update available.
    qcb.available--;

    // Disable full flag.
    qcb.flags &= !Q_FULL;

    // Update the empty flag.
    if (qcb.available > 0) {
      qcb.flags &= !Q_EMPTY;
    } else {
      qcb.flags |= Q_EMPTY;
    }
  }

  // Successfully got a character.
  return true;
}

// Writes a char to specified queue.
bool Q_putc(QCB qcb, uint8_t val) {
  // Check that the requested queue is full. If it is, fail.
  if (qcb.flags & Q_FULL) return false;

  // Enter atomic block while we change things.
  ATOMIC_BLOCK (ATOMIC_RESTORESTATE) {
    // Copy the value in the buffer at the out index to pval.
    qcb.buffer[qcb.in] = val;

    // Advance the in index.
    qcb.in++;
    qcb.in %= BUFFER_SIZE;

    // Update available.
    qcb.available++;

    // Disable empty flag.
    qcb.flags &= !Q_EMPTY;

    // Update the full flag.
    if (qcb.available == BUFFER_SIZE) {
      qcb.flags |= Q_FULL;
    } else {
      qcb.flags &= !Q_FULL;
    }
  }

  // Successfully put a character on the stack.
  return true;
}