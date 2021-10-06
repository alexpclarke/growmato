#include "acx-usart.h"

/* ----- Inerupt Service Routines ----- */

// If the we recieve a character, push it onto our rx buffer.
ISR (USART_RX_vect) {
  b_putc(USART_RX_BUFFER, UDR0);
}

// If the data register is empty and we have something in the tx buffer, pop a
// value from teh buffer and transmit it. If there are no characters left in the
// buffer, turn the data register empty interrupt off.
ISR (USART_UDRE_vect) {
  uint8_t val;
  if (b_getc(USART_TX_BUFFER, &val)) {
    UDR0 = val;
  } else {
    UCSR0B &= 0b11011111;
  }
}

/* ----- USART FUNCTIONS ----- */

// Initializes the USART port.
bool x_usart_init(uint32_t speed, uint8_t data_bits, uint8_t parity, uint8_t stop_bits, bool u2x) {
  // Initialize the rx and tx buffers.
  b_init(USART_RX_BUFFER);
  b_init(USART_TX_BUFFER);

  // Set baud rate. If the baudrate is invalid, fail to initialize.
  uint16_t bitTime = x_calc_baud(speed, u2x);
  if (!bitTime) return false;
  UBRR0H = (uint8_t)(bitTime >> 8);
  UBRR0L = (uint8_t)bitTime;

  // Enable receiver and transmitter.
  UCSR0B = (1 << RXEN0) | (1 << TXEN0);

  // Clear the framing.
  UCSR0C = 0b00000000;

  // Set the parity.
  if (parity == P_EVEN) UCSR0C |= 0b00100000;
  else if (parity == P_ODD) UCSR0C |= 0b00110000;
  else if (parity != P_NONE) return false;

  // Set the stop bits.
  if (stop_bits == 2) UCSR0C |= 0b00001000;
  else if (stop_bits != 1) return false;

  // Set data bits.
  if (data_bits == 6) UCSR0C |= 0b00000010;
  else if (data_bits == 7) UCSR0C |= 0b00000100;
  else if (data_bits == 8) UCSR0C |= 0b00000110;
  else if (data_bits != 5) return false;

  // Successfully initialized.
  return true;
}

// Sends a character over the USART port by pushing it to the stack and enabling
// the data register empty interupt.
void x_usart_putc(uint8_t data) {
  // Yield until we are able to push our data to the tx queue.
  while(!b_putc(USART_TX_BUFFER, data)) {
    x_yield();
  };

  // Enable the data regester empty interupt.
  UCSR0B |= (1 << UDRIE0);
}

void x_usart_putc_hex(uint8_t data) {
  // TODO
}

void x_usart_putc_bin(uint8_t data) {
  // TODO
}

// Sends a null terminated string over USART, returning the number of values
// sent.
// TODO: does this need to return the number? Barry's version did but i dont
// know if it's actually useful
int x_usart_puts(char* pdata) {
  // Loop untill we hit a null character, pushing them all onto the tx stack,
  // keeping track of how many we pushed.
  int count = 0;
  for (uint8_t* ptr = (uint8_t*)pdata; *ptr; ptr++) {
    while(!b_putc(USART_TX_BUFFER, *ptr)) {
      x_yield();
    };
    count++;
  }

  // Enable the data regester empty interupt.
  UCSR0B |= (1 << UDRIE0);

  // Return the number of characters successfully put.
  return count;
}

// Attempts to get a character from the rx buffer port and returns whether it
// was successful or not.
bool x_usart_getc(uint8_t* dest) {
  return b_getc(USART_RX_BUFFER, dest);
}

// Attempts to get a string of characters from the rx buffer and return the
// number of characters retrieved.
int x_usart_gets(uint8_t maxlen, uint8_t* pdata){
  // Validate pdata.
  if (pdata == ((void*)0)) return 0;

  // Get all the characters untill we hit a new line, a null or getc fails.
  int i;
  for (i = 0; i < maxlen - 1; i++) {
    int val = x_usart_getc(0);
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