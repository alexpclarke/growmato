#include "acx-buffer.h"

void b_init(uint8_t b_id) {
  BN_IN(b_id) = 0;
  BN_OUT(b_id) = 0;
  BN_AVAILABLE(b_id) = (uint8_t)0;
}

// Gets a char from specified queue.
bool b_getc(uint8_t b_id, uint8_t* valPtr) {
  // Check that there are available characters to get. If there are none, fail.
  if (!BN_AVAILABLE(b_id)) return false;

  // Enter atomic block while we change things.
  ATOMIC_BLOCK (ATOMIC_RESTORESTATE) {
    // Copy the value in the buffer at the out index to pval.
    *valPtr = BN_BASE(b_id)[BN_OUT(b_id)];

    // Advance the out index.
    BN_OUT(b_id)++;
    BN_OUT(b_id) %= B_SIZE;

    // Update available.
    BN_AVAILABLE(b_id)--;
  }

  // Successfully got a character.
  return true;
}

// Writes a char to specified queue.
bool b_putc(uint8_t b_id, uint8_t val) {
  // Check that the requested queue is full. If it is, fail.
  if (BN_AVAILABLE(b_id) >= B_SIZE) return false;

  // Enter atomic block while we change things.
  ATOMIC_BLOCK (ATOMIC_RESTORESTATE) {
    // Copy the value in the buffer at the out index to pval.
    BN_BASE(b_id)[BN_IN(b_id)] = val;

    // Advance the in index.
    BN_IN(b_id)++;
    BN_IN(b_id) %= B_SIZE;

    // Update available.
    BN_AVAILABLE(b_id)++;
  }

  // Successfully put a character on the stack.
  return true;
}