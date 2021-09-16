// Author:  Alex Clarke
// Date:    2021-09-16

/* ----- Includes ----- */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include <stdlib.h>
#include "acx.h"
#include "acx-serial.h"

/* ----- DATA ----- */

// Initialize serial port transmit and receive buffers.
static uint8_t rx_buffer[RX_BUFFER_SIZE];
static uint8_t tx_buffer[TX_BUFFER_SIZE];

// Initialize transmit and receive queue control blocks.
static QCB rx_queue = {0, 0, (RX_BUFFER_SIZE - 1), 0x01, 0, rx_buffer};
static QCB tx_queue = {0, 0, (TX_BUFFER_SIZE - 1), 0x01, 0, tx_buffer};

static volatile SERIAL_PORT_REGS* regs = (SERIAL_PORT_REGS*)0xC0;

/* ----- QUEUE FUNCTIONS ----- */

// Initialize a specified Queue. Returns true if successful.
bool Q_init(QCB* qPtr, uint8_t* qBuffer) {
  (*qPtr).in = 0;
  (*qPtr).out = 0;
  (*qPtr).flags = Q_EMPTY;
  (*qPtr).available = 0;
  (*qPtr).pQ = qBuffer;
  return true;
}

// Gets a char from specified queue.
bool Q_getc(QCB* qPtr, uint8_t* valPtr) {
  // Check that there are available characters to get. If there are none, fail.
  if ((*qPtr).available == 0) return false;

  // Enter atomic block while we change things.
  ATOMIC_BLOCK (ATOMIC_RESTORESTATE) {
    // Copy the value in the buffer at the out index to pval.
    *valPtr = (*qPtr).pQ[(*qPtr).out];

    // Advance the out index.
    (*qPtr).out++;
    (*qPtr).out &= (*qPtr).smask;

    // Update available.
    (*qPtr).available--;

    // Disable full flag.
    (*qPtr).flags &= !Q_FULL;

    // Update the empty flag.
    if ((*qPtr).available) {
      (*qPtr).flags &= !Q_EMPTY;
    } else {
      (*qPtr).flags |= Q_EMPTY;
    }
  }

  // Successfully got a character.
  return true;
}

// Writes a char to specified queue.
bool Q_putc(QCB* qPtr, uint8_t val) {
  // Check that the requested queue is not full. If it is, fail.
  if ((*qPtr).flags & Q_FULL) return false;

  // Enter atomic block while we change things.
  ATOMIC_BLOCK (ATOMIC_RESTORESTATE) {
    // Copy the value in the buffer at the out index to pval.
    (*qPtr).pQ[(*qPtr).in] = val;

    // Advance the in index.
    (*qPtr).in++;
    (*qPtr).in &= (*qPtr).smask;

    // Update available.
    (*qPtr).available++;

    // Disable empty flag.
    (*qPtr).flags &= !Q_EMPTY;

    // Update the full flag.
    if ((*qPtr).available == (*qPtr).smask) {
      (*qPtr).flags |= Q_FULL;
    } else {
      (*qPtr).flags &= !Q_FULL;
    }
  }

  // Successfully put a character on the stack.
  return true;
}




/* ----- Inerupt Service Routines ----- */

ISR (USART_RX_vect) {
  Q_putc(&rx_queue, UDR0);
}

ISR (USART_UDRE_vect) {
  if (!Q_getc(&tx_queue, &(regs->UDRn))) {
    regs->UCSRnB &= 0b11011111;
  }
}


/* ----- SERIAL PORT FUNCTIONS ----- */

bool x_serial_init(long speed, int framing) {
  // Enable RXENn, TXENn, RXCIEn and UDRIEn for port n.
  regs->UCSRnB |= 0b10111000;

  // Set framing.
  regs->UCSRnC = framing;

  // Set baud rate.
  uint16_t bitTime = BaudRateToBitTime(speed, false);
  regs->UBRRn = bitTime;

  // Enables interrupts by setting the global interrupt mask.
  sei();

  // If the baud rate is invalid, fail to initialize, otherwise, we successfull
  // initialized.
  if (!bitTime) return false;
  else return true;
}

// Sends a character over the designated port. Returns the number of characters
// put.
int x_serial_putc(char data) {
  // Block until we are able to push our data to the tx queue.
  while(Q_putc(&tx_queue, data) == false) {
    x_yield();
  };

  // Enable UDRIEn.
  regs->UCSRnB |= 0b00100000;

  // Return the number of characters successfully put.
  return 1;
}

// Gets a character from port n. Returns either a character if one is found or
// -1 if none is found.
int x_serial_getc() {
  // Get a character from the queue, if none is found return -1.
  uint8_t val;
  if (Q_getc(&rx_queue, &val)) {
    return (int)val;
  } else {
    return -1;
  }
}

int x_serial_puts(char* pdata) {
  // Loop through each character in the string and pass it to Serial_putc.
  int i;
  for (i = 0; pdata[i] != '\0'; i++) {
    x_serial_putc(pdata[i]);
  }

  // Return the number of characters successfully put.
  return i;
}

int x_serial_gets(int maxlen, char* pdata){
  // Validate pdata.
  if (pdata == NULL) return 0;

  // Get all the characters untill we hit a new line, a null or getc fails.
  int i;
  for (i = 0; i < maxlen - 1; i++) {
    int val = x_serial_getc(0);
    if (val == '\n' || val == '\0' || val == -1) break;
    pdata[i] = val;
  }

  // Set null terminating bit.
  pdata[i + 1] = '\0';

  // Return the number of characters retrieved.
  return i;
}


/* ----- Helper Functions ----- */

// Given a baud rate, calculate the speed at which it should run.
uint16_t BaudRateToBitTime(long speed, bool u2x) {
  switch (speed) {
    case 2400:
      return u2x ? 832 : 416;
    case 4800:
      return u2x ? 416 : 207;
    case 9600:
      return u2x ? 207 : 103;
    case 14400:
      return u2x ? 138 : 68;
    case 19200:
      return u2x ? 103 : 51;
    case 28800:
      return u2x ? 68 : 34;
    case 38400:
      return u2x ? 51 : 25;
    case 57600:
      return u2x ? 34 : 16;
    case 76800:
      return u2x ? 25 : 12;
    case 115200:
      return u2x ? 16 : 8;
    case 230400:
      return u2x ? 8 : 3;
    case 250000:
      return u2x ? 7 : 3;
    case 500000:
      return u2x ? 3 : 1;
    case 1000000:
      return u2x ? 1 : 0;
    default:
      return 0;
  }
}