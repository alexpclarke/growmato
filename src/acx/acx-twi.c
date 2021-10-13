#include "acx-twi.h"
#include "acx-usart.h"

static volatile uint8_t twi_mode;
static volatile uint8_t twi_rep_start; 
// static volatile uint8_t twi_send_stop;

void x_twi_init() {
  // initialize state
  twi_mode = TWI_READY;
  // twi_sendStop = true;
  twi_rep_start = false;

  // activate internal pullups for the SDA and SCL pins.
  DDRC |= _BV(DDC4) | _BV(DDC5);
  PORTC |= _BV(PORTC4) | _BV(PORTC5);

  // Set prescalar and bit rate.
  x_twi_set_frequency(TWI_FREQUENCY, TWI_PRESCALAR);
  
  // Enable TWI and interrupts.
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
  // DDRC &= ~(_BV(DDC4) | _BV(DDC5));
  // PORTC &= ~(_BV(PORTC4) | _BV(PORTC5));
}

// Send the START signal, enable interrupts and TWI, clear TWINT flag to resume
// transfer.
void x_twi_start() {
  // Ensure that TWI module and interrupts are enabled, clear the rest. 
  TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWSTA) | _BV(TWINT);

  // Enable the start condition bit.
  // TWCR |= _BV(TWSTA);
  
  // Clear the TWINT.
  // TWCR |= _BV(TWINT);
}

// Send the STOP signal, enable interrupts and TWI, clear TWINT flag.
void x_twi_stop() {
  // Ensure that TWI module and interrupts are enabled, clear the rest. 
  TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWSTO) | _BV(TWINT);

  // Enable the stop condition bit.
  // TWCR |= _BV(TWSTO);

  // Clear the TWINT.
  // TWCR |= _BV(TWINT);
}

// Used to resume a transfer, clear TWINT and ensure that TWI and interrupts are
// enabled.
void x_twi_write(uint8_t val) {
  // Load the value into TWDR.
  TWDR = val;

  // Ensure that TWI module and interrupts are enabled, clear the rest. 
  TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWINT);

  // Clear the TWINT bit to continue transfer.
  // TWCR |= _BV(TWINT);
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
  while (twi_mode != TWI_READY) x_yield();

  // Set repstart mode.
  twi_rep_start = !sendStop;

  // Initialize the buffer.
  b_init(TWI_TX_BUFFER);

  // Put the address into the stack.
  // uint8_t temp_addr = 0b10000000;
  uint8_t temp_addr = ((address << 1) & 0xFE);
  b_putc(TWI_TX_BUFFER, temp_addr);

  // Copy the data to the buffer.
  for (int i = 0; i < length; i++) {
    b_putc(TWI_TX_BUFFER, data[i]);
  }

  if (twi_mode == TWI_REPSTART) {
    // TODO
  } else {
    // Become master.
    twi_mode = TWI_INIT;

    // Send start condition.
    x_twi_start();
  }

  return true;
}

ISR (TWI_vect) {
  switch (TWS) {
    case TWS_MT_SLA_ACK:
      twi_mode = TWI_MTX;
    case TWS_START:
    case TWS_MT_DATA_ACK:
      ;
      uint8_t val;
      if (b_getc(TWI_TX_BUFFER, &val)) {
        // If there is more data to send, send it.
        x_twi_write(val);
      } else if (twi_rep_start) {
        // If in a repeated start, send start.
        x_twi_start();
      } else {
        // Nothing left to transmit. Set the bus to ready and send stop signal.
        twi_mode = TWI_READY;
        x_twi_stop();
      }

      break;

    case TWS_MT_SLA_NACK:
    case TWS_MT_ARB_LOST:
    default:
      x_usart_putc('x');
      x_twi_disable();
      break;
  }
}