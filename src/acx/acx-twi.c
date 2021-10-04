#include "acx-twi.h"

void x_twi_init() {
  // Set prescalar to 0.
  TWSR = TWI_PRESCALAR;

  // Set TWI Bit Rate to 400kHz.
  TWBR = 0x0C;
  
  // Enable TWI.
  TWCR = (1 << TWEN);
}

// Sends start signal and wait for it to finish the job.
void x_twi_start() {
  TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
  while ((TWCR & (1 << TWINT)) == 0) {
    x_yield();
  }
}

void x_twi_stop() {
  TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
}

void x_twi_write(uint8_t u8data) {
  TWDR = u8data;
  TWCR = (1 << TWINT) | (1 << TWEN);
  while ((TWCR & (1 << TWINT)) == 0) {
    x_yield();
  }
}

uint8_t x_twi_read_ack() {
  TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
  while ((TWCR & (1 << TWINT)) == 0) {
    x_yield();
  }
  return TWDR;
}
//read byte with NACK
uint8_t x_twi_read_nack() {
  TWCR = (1 << TWINT) | (1 << TWEN);
  while ((TWCR & (1 << TWINT)) == 0) {
    x_yield();
  }
  return TWDR;
}

uint8_t x_twi_get_status() {
  uint8_t status;
  //mask status
  status = TWSR & 0xF8;
  return status;
}

bool x_twi_putc(uint8_t device_address, uint8_t c) {
  // Set the I2C bus to write mode and set to idle.
  DDRC |= 0b00110000;
  PORTC |= 0b00110000;

  // Start Condition.
  x_twi_start();

  if (x_twi_get_status() != 0x08) return false;

  // Send Address Packet. 7 address bits, one read/write bit and an acknowledge
  // bit
  x_twi_write(device_address | (uint8_t)((u16addr & 0x0700)>>7));

  // Send Data Packet. 
  // 

  // Stop Condition.
  // SCL remains high while SDA goes from low to high.

  return true;
}