#include "acx-twi.h"

volatile uint8_t twi_state;
volatile uint8_t twi_send_stop;
volatile uint8_t twi_ack;

// Initialize the TWI bus.
void x_twi_init() {
  // Initialize state.
  twi_state = TWI_READY;
  twi_send_stop = true;

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

bool x_twi_is_ready() {
  return (twi_state == TWI_READY || twi_state == TWI_REPSTART);
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
  // Ensure that TWI module and interrupts are enabled, clear the rest. 
  // Enable the start condition bit.
  // Clear the TWINT.
  TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWSTA) | _BV(TWINT);
}

// Send the STOP signal, enable interrupts and TWI, clear TWINT flag.
void x_twi_stop() {
  if (twi_send_stop) {
    // Ensure that TWI module and interrupts are enabled, clear the rest. 
    // Enable the stop condition bit.
    // Clear the TWINT.
    TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWSTO) | _BV(TWINT);

    // Set status to ready.
    twi_state = TWI_READY;
  } else {
    // Dont send a stop and send a repeated start instead to keep control.
    x_twi_start();
  }
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

bool x_twi_putc(uint8_t address, uint8_t data, bool sendStop) {
  // Wait untill TWI is ready
  while (!x_twi_is_ready()) x_yield();

  // Set repstart mode.
  twi_send_stop = sendStop;

  // Initialize the buffer.
  b_init(TWI_TX_BUFFER);

  // Put the address into the stack.
  uint8_t temp_addr = ((address << 1) & 0b11111110);
  // uint8_t temp_addr = address;
  b_putc(TWI_TX_BUFFER, temp_addr);

  // Copy the data to the buffer.
  b_putc(TWI_TX_BUFFER, data);

  if (twi_state == TWI_REPSTART) {
    uint8_t val;
    if (b_getc(TWI_TX_BUFFER, &val)) {
      // Become master.
      twi_state = TWI_INIT;
      
      x_twi_write(val);
    } else {
      return false;
    }
  } else {
    // Become master.
    twi_state = TWI_INIT;

    // Send start condition.
    x_twi_start();
  }

  // while (twi_ack == TWI_ACK_NONE) x_yield();
  while (twi_ack == TWI_ACK_NONE) {};
  uint8_t resp = twi_ack;
  twi_ack = TWI_ACK_NONE;
  return resp == TWI_ACK_ACK;
}

bool x_twi_puts(uint8_t address, uint8_t* data, uint8_t length, bool sendStop) {
  // Ensure the buffer is large enough.
  if (length > B_SIZE) return false;

  // Wait untill TWI is ready
  while (!x_twi_is_ready()) x_yield();

  // Set repstart mode.
  twi_send_stop = sendStop;

  // Initialize the buffer.
  b_init(TWI_TX_BUFFER);

  // Put the address into the stack.
  // uint8_t temp_addr = 0b10000000;
  uint8_t temp_addr = ((address << 1) & 0b11111110);
  b_putc(TWI_TX_BUFFER, temp_addr);

  // Copy the data to the buffer.
  for (int i = 0; i < length; i++) {
    b_putc(TWI_TX_BUFFER, data[i]);
  }

  if (twi_state == TWI_REPSTART) {
    uint8_t val;
    if (b_getc(TWI_TX_BUFFER, &val)) {
      x_twi_write(val);
    } else {
      return false;
    }
  } else {
    // Become master.
    twi_state = TWI_INIT;

    // Send start condition.
    x_twi_start();
  }

  // while (twi_ack == TWI_ACK_NONE) x_yield();
  while (twi_ack == TWI_ACK_NONE) {};
  uint8_t resp = twi_ack;
  twi_ack = TWI_ACK_NONE;
  return resp == TWI_ACK_ACK;
}

bool x_twi_getc(uint8_t address, uint8_t* dest, bool send_stop) {
  return x_twi_gets(address, dest, 1, send_stop);
}

bool x_twi_gets(uint8_t address, uint8_t* dest, uint8_t max_length, bool send_stop) {
  // Check if number of bytes to read can fit in the RXbuffer.
  if (max_length >= B_SIZE) return false;

  // Reset RX buffer.
  b_init(TWI_RX_BUFFER);

  // Wait untill TWI is ready
  while (!x_twi_is_ready()) x_yield();

  // Set repstart mode.
  twi_send_stop = send_stop;

  // Initialize the TX buffer.
  b_init(TWI_TX_BUFFER);

  // Put the address into the stack.
  uint8_t temp_addr = ((address << 1) | 0b00000001);
  b_putc(TWI_TX_BUFFER, temp_addr);
  
  if (twi_state == TWI_REPSTART) {
    uint8_t val;
    if (b_getc(TWI_TX_BUFFER, &val)) {
      x_twi_write(val);
    } else {
      return false;
    }
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
    case TWS_MT_SLA_ACK: {
      twi_state = TWI_MTX;
    }
    case TWS_START:
    case TWS_MT_DATA_ACK: {
      // If there is more data to send, send it, otherwise send a stop signal.
      uint8_t val;
      if (b_getc(TWI_TX_BUFFER, &val)) x_twi_write(val);
      else {
        twi_ack = TWI_ACK_ACK;
        x_twi_stop();
      }
      break;
    }
    case TWS_REP_START: {
      // Switch to Repeat Start mode.
      twi_state = TWI_REPSTART;
      break;
    }
    case TWS_MR_SLA_ACK: {
      twi_ack = TWI_ACK_ACK;
      
      // Switch to Master Receiver mode.
      twi_state = TWI_MRX;

      // If there is more than one byte, receive data and return an ACK,
      // otherwise when a data byte is received, return NACK.
      if (b_get_available(TWI_RX_BUFFER) > 1) {
        x_twi_ack();
      } else {
        x_twi_nack();
      }
      break;
    }

    case TWS_MR_DATA_ACK: {
      twi_ack = TWI_ACK_ACK;

      // Push the recieved byte to the rx buffer.
      b_putc(TWI_RX_BUFFER, TWDR);

      if (b_get_available(TWI_RX_BUFFER) > 1) {
        // If there is more than one byte, receive data and return an ACK.
        x_twi_ack();
      } else {
        // Otherwise when a data byte is received, return NACK.
        x_twi_nack();
      }
      break;
    }

    case TWS_MR_DATA_NACK: {
      twi_ack = TWI_ACK_NACK;

      // Push the recieved byte to the rx buffer.
      b_putc(TWI_RX_BUFFER, TWDR);

      // Send the stop bit.
      x_twi_stop();
      break;
    }
    case TWS_MT_SLA_NACK:
    case TWS_MR_SLA_NACK:
    case TWS_MT_DATA_NACK: {
      twi_ack = TWI_ACK_NACK;
      break;
    }


    default: {
      x_twi_disable();
      break;
    }
  }
}