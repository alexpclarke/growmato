#include "acx-twi.h"
#include "acx-usart.h"

static volatile uint8_t twi_state;
static volatile uint8_t twi_slarw;
static volatile uint8_t twi_sendStop;
static volatile uint8_t twi_inRepStart;
static volatile uint8_t twi_error;

void x_twi_init() {
  // initialize state
  twi_state = TWI_READY;
  twi_sendStop = true;
  twi_inRepStart = false;

  // activate internal pullups for the SDA and SCL pins.
  DDRC |= _BV(DDC4) | _BV(DDC5);
  // PORTC |= _BV(PORTC4) | _BV(PORTC5);

  // Set prescalar and bit rate.
  x_twi_set_frequency(TWI_FREQUENCY, TWI_PRESCALAR);
  
  // Enable TWI, acks, and interrupts.
  TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWEA);
}


void x_twi_set_frequency(uint32_t freq, uint8_t prescalar) {
  // Set prescalar and bit rate.
  TWSR = prescalar & 0x00000011;
  TWBR = ((CLOCK_HZ / freq) - 16) / 2;
}

void twi_setAddress(uint8_t address) {
  TWAR = address << 1;
}


void x_twi_disable() {
  // disable TWI, acks, and interrupts.
  TWCR = ~(_BV(TWEN) | _BV(TWIE) | _BV(TWEA));

  // deactivate internal pullups for twi.
  DDRC &= ~(_BV(DDC4) | _BV(DDC5));
  PORTC &= ~(_BV(PORTC4) | _BV(PORTC5));
}


// Sends start signal and wait for it to finish the job.
void x_twi_start() {
  TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWEA) | _BV(TWINT) | _BV(TWSTA);
 
  // while ((TWCR & _BV(TWSTO))) {
  //   // x_yield();
  // }

  // update twi state
  twi_state = TWI_READY;
}

void x_twi_stop() {
  TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWEA) | _BV(TWINT) | _BV(TWSTO);

  while ((TWCR & _BV(TWSTO))) {
    // x_yield();
  }

  // update twi state
  twi_state = TWI_READY;
}

void x_twi_write(uint8_t u8data) {
  TWDR = u8data;
  TWCR = _BV(TWINT) | _BV(TWEN);
  while ((TWCR & _BV(TWINT)) == 0) {
    // x_yield();
  }
}

uint8_t x_twi_read_ack() {
  TWCR = _BV(TWINT) | _BV(TWEN) | _BV(TWEA);
  while ((TWCR & _BV(TWINT)) == 0) {
    // x_yield();
  }
  return TWDR;
}

// read byte with NACK.
uint8_t x_twi_read_nack() {
  TWCR = _BV(TWINT) | _BV(TWEN);
  while ((TWCR & _BV(TWINT)) == 0) {
    // x_yield();
  }
  return TWDR;
}

// sends byte or readys recieve line.
void x_twi_reply(bool ack) {
  // transmit master read ready signal, with or without ack
  if (ack) {
    TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWINT) | _BV(TWEA);
  } else {
    TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWINT);
  }
}



bool x_twi_writeTo(uint8_t address, uint8_t* data, uint8_t length) {
  // Ensure the buffer is large enough.
  if (length > B_SIZE) return false;

  // Wait untill TWI is ready
  while (twi_state != TWI_READY) {
    x_yield();
  }

  // become master.
  twi_state = TWI_MTX;

  // Initialize the two buffers.
  // b_init(TWI_RX_BUFFER);
  b_init(TWI_TX_BUFFER);

  // Copy the data to the buffer.
  for (int i = 0; i < length; i++) {
    b_putc(TWI_TX_BUFFER, data[i]);
  }

  // set SLARW
  twi_slarw = 1;
  twi_slarw |= address << 1;

  // Send start condition.
  x_twi_start();

  // wait for write to complete
  while (twi_state == TWI_MTX) {
    x_yield();
  }

  return true;
}





ISR (TWI_vect) {
  uint8_t val = TWIS;
  uint8_t myval;

  switch (val) {
    case TWIS_START:
    case TWIS_REP_START:
      x_usart_putc('s');
      TWDR = twi_slarw;
      x_twi_reply(1);
      break;
    case TWIS_MT_SLA_ACK:
    case TWIS_MT_DATA_ACK:
      x_usart_putc('a');
      if (b_getc(TWI_TX_BUFFER, &myval)) {
        TWDR = myval;
      } else {
        x_twi_stop();
      }
      break;
    case TWIS_MT_SLA_NACK:
    case TWIS_MT_DATA_NACK:
      x_usart_putc('n');
      break;
    case TWIS_MR_SLA_NACK:
      x_twi_stop();
      break;
    default:
      x_usart_putc('x');
      break;
  }
}