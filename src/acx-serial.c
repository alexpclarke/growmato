// Author:  Alex Clarke
// Date:    2021-09-16

#include "acx-serial.h"

/* ----- DATA ----- */

// Initialize serial port transmit and receive buffers.
uint8_t rx_buffer[BUFFER_SIZE];
uint8_t tx_buffer[BUFFER_SIZE];

// Initialize transmit and receive queue control blocks.
volatile QCB rx_queue;
volatile QCB tx_queue;

// /* ----- Inerupt Service Routines ----- */

// ISR (USART_RX_vect) {
//   Q_putc(rx_queue, UDR0);
// }

ISR (USART_UDRE_vect) {
  uint8_t val;
  ATOMIC_BLOCK (ATOMIC_RESTORESTATE) {
    if (Q_getc(tx_queue, &val)) {
      UDR0 = val;
    } else {
      UCSR0B &= !(1 << UDRIE0);
    }
  }
}


// /* ----- SERIAL PORT FUNCTIONS ----- */

bool x_serial_init(uint32_t speed, uint8_t data_bits, uint8_t parity, uint8_t stop_bits, bool u2x) {
  Q_init(rx_queue, rx_buffer);
  Q_init(tx_queue, tx_buffer);

  // Set baud rate.
  uint16_t bitTime = x_calc_baud(speed, u2x);
  UBRR0H = (uint8_t)(bitTime >> 8);
  UBRR0L = (uint8_t)bitTime;

  // Enable receiver and transmitter.
  UCSR0B = (1<<RXEN0)|(1<<TXEN0);

  // Set the framing.
  uint8_t framing = 0b00000000;

  // Set the parity.
  if (parity == P_EVEN) framing |= 0b00100000;
  else if (parity == P_ODD) framing |= 0b00110000;
  else if (parity != P_NONE) return false;

  // Set the stop bits.
  if (stop_bits == 2) framing |= 0b00001000;
  else if (stop_bits != 1) return false;

  // Set data bits.
  if (data_bits == 6) framing |= 0b00000001;
  else if (data_bits == 7) framing |= 0b00000010;
  else if (data_bits == 8) framing |= 0b00000011;
  else if (data_bits != 5) return false;

  UCSR0C = framing;

  // If the baud rate is invalid, fail to initialize, otherwise, we successfull
  // initialized.
  if (!bitTime) return false;
  else return true;
}

// Sends a character over the designated port. Returns the number of characters
// put.
int x_serial_putc(char data) {
  // Block until we are able to push our data to the tx queue.
  while(!Q_putc(tx_queue, data)) {
    x_yield();
  };

  // Enable the data regester empty interupt.
  UCSR0B |= (1 << UDRIE0);

  // Return the number of characters successfully put.
  return 1;
}

// // Gets a character from port n. Returns either a character if one is found or
// // -1 if none is found.
// int x_serial_getc() {
//   // Get a character from the queue, if none is found return -1.
//   uint8_t val;
//   if (Q_getc(rx_queue, &val)) {
//     return (int)val;
//   } else {
//     return -1;
//   }
// }

// int x_serial_puts(char* pdata) {
//   // Loop through each character in the string and pass it to Serial_putc.
//   int i;
//   for (i = 0; pdata[i] != '\0'; i++) {
//     x_serial_putc(pdata[i]);
//   }

//   // Return the number of characters successfully put.
//   return i;
// }

// int x_serial_gets(int maxlen, char* pdata){
//   // Validate pdata.
//   if (pdata == NULL) return 0;

//   // Get all the characters untill we hit a new line, a null or getc fails.
//   int i;
//   for (i = 0; i < maxlen - 1; i++) {
//     int val = x_serial_getc(0);
//     if (val == '\n' || val == '\0' || val == -1) break;
//     pdata[i] = val;
//   }

//   // Set null terminating bit.
//   pdata[i + 1] = '\0';

//   // Return the number of characters retrieved.
//   return i;
// }


/* ----- Helper Functions ----- */

// Given a baud rate, calculate the speed at which it should run.
uint16_t x_calc_baud(uint32_t speed, bool u2x) {
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