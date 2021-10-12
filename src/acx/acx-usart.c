#include "acx-usart.h"

// If the we recieve a character, push it onto our rx buffer.
ISR (USART_RX_vect) {
  b_putc(USART_RX_BUFFER, UDR0);
}

// If the data register is empty and we have something in the tx buffer, pop a
// value from the buffer and transmit it. If there are no characters left in the
// buffer, turn the data register empty interrupt off.
ISR (USART_UDRE_vect) {
  uint8_t val;
  if (b_getc(USART_TX_BUFFER, &val)) {
    UDR0 = val;
  } else {
    UCSR0B &= ~_BV(UDRIE0);
  }
}

// Initializes the USART port.
bool x_usart_init(uint32_t speed, uint8_t data_bits, uint8_t parity, uint8_t stop_bits, bool u2x) {
  // Clear global interupts.
  cli();

  // Initialize the rx and tx buffers.
  b_init(USART_RX_BUFFER);
  b_init(USART_TX_BUFFER);

  // Set baud rate. If the baudrate is invalid, fail to initialize.
  if (!x_usart_set_baud(speed, u2x)) return false;

  // Enable receiver and transmitter.
  UCSR0B |= _BV(RXEN0) | _BV(TXEN0); 

  // Enable rx complete interupt;
  UCSR0B |= _BV(RXCIE0);

  // Clear the framing.
  UCSR0C = 0b00000000;

  // Set the parity.
  if (!x_usart_set_parity(parity)) return false;

  // Set the stop bits.
  if (!x_usart_set_stop_bits(stop_bits)) return false;

  // Set data bits.
  if (!x_usart_set_data_bits(data_bits)) return false;

  // Set global interupts.
  sei();

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
  UCSR0B |= _BV(UDRIE0);
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
  UCSR0B |= _BV(UDRIE0);

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
uint8_t x_usart_gets(uint8_t maxlen, char* pdata){
  // Validate pdata.
  if (pdata == ((void*)0)) return 0;

  // Get all the characters untill we hit a new line, a null or getc fails.
  uint8_t i;
  uint8_t val;
  
  for (i = 0; i < maxlen - 1; i++) {
    if (!b_getc(USART_RX_BUFFER, &val)) break;
    if (val == '\n' || val == '\0') break;
    pdata[i] = val;
  }

  // Set null terminating bit.
  pdata[i + 1] = '\0';

  // Return the number of characters retrieved.
  return i;
}

// Given a baud rate, calculate the speed at which it should run.
bool x_usart_set_baud(uint32_t speed, bool u2x) {
  // Clear and set U2X.
  UCSR0A &= ~_BV(U2X0);
  if (u2x) UCSR0A |= _BV(U2X0);

  // Set the UBRR0 speed.
  switch (speed) {
    case 2400:
      UBRR0 = u2x ? 832 : 416;
      return true;
    case 4800:
      UBRR0 = u2x ? 416 : 207;
      return true;
    case 9600:
      UBRR0 = u2x ? 207 : 103;
      return true;
    case 14400:
      UBRR0 = u2x ? 138 : 68;
      return true;
    case 19200:
      UBRR0 = u2x ? 103 : 51;
      return true;
    case 28800:
      UBRR0 = u2x ? 68 : 34;
      return true;
    case 38400:
      UBRR0 = u2x ? 51 : 25;
      return true;
    case 57600:
      UBRR0 = u2x ? 34 : 16;
      return true;
    case 76800:
      UBRR0 = u2x ? 25 : 12;
      return true;
    case 115200:
      UBRR0 = u2x ? 16 : 8;
      return true;
    case 230400:
      UBRR0 = u2x ? 8 : 3;
      return true;
    case 250000:
      UBRR0 = u2x ? 7 : 3;
      return true;
    case 500000:
      UBRR0 = u2x ? 3 : 1;
      return true;
    case 1000000:
      UBRR0 = u2x ? 1 : 0;
      return true;
    default:
      UBRR0 = 0;
      return false;
  }
}

bool x_usart_set_data_bits(uint8_t data_bits) {
  // Clear UCSZ0
  UCSR0C &= ~(_BV(UCSZ01) | _BV(UCSZ00));

  // Set the data bits.
  switch (data_bits) {
    case 5:
      return true;
    case 6:
      UCSR0C |= _BV(UCSZ00);
      return true;
    case 7:
      UCSR0C |= _BV(UCSZ01);
      return true;
    case 8:
      UCSR0C |= _BV(UCSZ00) | _BV(UCSZ01);
      return true;
    default:
      return false;
  }
}

bool x_usart_set_parity(uint8_t parity) {
  // Clear UPM0.
  UCSR0C &= ~(_BV(UPM01) | _BV(UPM00));

  // Set UPM0.
  switch (parity) {
    case P_NONE:
      return true;
    case P_ODD:
      UCSR0C |= _BV(UPM01) | _BV(UPM00);
      return true;
    case P_EVEN:
      UCSR0C |= _BV(UPM01);
      return true;
    default:
      return false;
  }
}

bool x_usart_set_stop_bits(uint8_t stop_bits) {
  // Clear USBS0.
  UCSR0C &= ~_BV(USBS0);

  // Set UBS0.
  switch (stop_bits) {
    case 1:
      return true;
    case 2:
      UCSR0C |= _BV(USBS0);
      return true;
    default:
      return false;
  }
}