#include "acx-twi.h"

static volatile uint8_t twi_state;
static volatile uint8_t twi_in_rep_start;

void x_twi_init() {
  // initialize state
  twi_state = TWI_READY;
  // twi_sendStop = true;
  twi_in_rep_start = false;

  // activate internal pullups for the SDA and SCL pins.
  // DDRC |= _BV(DDC4) | _BV(DDC5);
  // PORTC |= _BV(PORTC4) | _BV(PORTC5);

  // Set prescalar and bit rate.
  x_twi_set_frequency(TWI_FREQUENCY, TWI_PRESCALAR);
  
  // Enable TWI, acks, and interrupts.
  TWCR = _BV(TWEN) | _BV(TWIE);
}

// Set prescalar and bit rate.
void x_twi_set_frequency(uint32_t freq, uint8_t prescalar) {
  TWSR = prescalar & 0x00000011;
  TWBR = ((CLOCK_HZ / freq) - 16) / 2;
}

// Set device's slave address.
void twi_setAddress(uint8_t address) {
  TWAR = address << 1;
}

// Disable the TWI Bus.
void x_twi_disable() {
  // disable TWI, acks, and interrupts.
  TWCR = ~(_BV(TWEN) | _BV(TWIE) | _BV(TWEA));

  // deactivate internal pullups for twi.
  DDRC &= ~(_BV(DDC4) | _BV(DDC5));
  PORTC &= ~(_BV(PORTC4) | _BV(PORTC5));
}

// Send the START signal, enable interrupts and TWI, clear TWINT flag to resume
// transfer.
void x_twi_start() {
  // Enable the start condition bit.
  TWCR |= _BV(TWSTA);

  // Ensure that TWI module and interrupts are enabled.
  TWCR |= _BV(TWEN) | _BV(TWIE);
  
  // Clear the TWINT.
  TWCR |= _BV(TWINT);
}

// Send the STOP signal, enable interrupts and TWI, clear TWINT flag.
void x_twi_stop() {
  // TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWEA) | _BV(TWINT) | _BV(TWSTO);
  TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWINT) | _BV(TWSTO);

  // update twi state
  // twi_state = TWI_READY;
}

// Used to resume a transfer, clear TWINT and ensure that TWI and interrupts are
// enabled.
void x_twi_write(uint8_t val) {
  // Load the value into TWDR.
  TWDR = val;

  // Clear the TWINT bit to continue transfer.
  // TWCR |= _BV(TWINT) | _BV(TWEN) | _BV(TWIE);
  TWCR |= _BV(TWINT);
}

// FOR MR mode. Resume a transfer, ensure that TWI and interrupts are enabled
// and respond with an ACK if the device is addressed as a slave or after it
// receives a byte.
void x_twi_ack() {
  TWCR = _BV(TWINT) | _BV(TWEN) | _BV(TWIE) | _BV(TWEA);
}

// FOR MR mode. Resume a transfer, ensure that TWI and interrupts are enabled
// but DO NOT respond with an ACK if the device is addressed as a slave or after
// it receives a byte.
void x_twi_nack() {
  TWCR = _BV(TWINT) | _BV(TWEN) | _BV(TWIE);
}

bool x_twi_transmit(uint8_t address, uint8_t* data, uint8_t length, bool sendStop) {
  // Ensure the buffer is large enough.
  if (length > B_SIZE) return false;

  // Wait untill TWI is ready
  while (twi_state != TWI_READY) x_yield();

  // Initialize the buffer.
  b_init(TWI_TX_BUFFER);

  // Copy the data to the buffer.
  for (int i = 0; i < length; i++) {
    b_putc(TWI_TX_BUFFER, data[i]);
  }

  if (twi_state == TWI_REPSTART) {
    // TODO
  } else {
    // Become master.
    twi_state = TWI_INIT;

    // Send start condition.
    x_twi_start();
  }

  return true;
}

ISR (TWI_vect) {
  switch (TWS) {
    case TWS_START:
      x_usart_puts("start sent\n");
      break;
    
    case TWS_MT_SLA_ACK:
      x_usart_puts("tx success\n");
      break;

    case TWS_MT_SLA_NACK:
      x_usart_puts("tx fail\n");
      break;
    
    case TWS_MT_ARB_LOST:
      x_usart_puts("tx arbitration lost\n");
      break;

    default:
      x_usart_putc('x');
      break;
  }
}