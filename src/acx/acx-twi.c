#include "acx-twi.h"
#include "acx-usart.h"

void x_twi_init() {
  // Set prescalar to 0.
  TWSR = TWI_PRESCALAR & 0x00000011;

  // Set TWI Bit Rate to 400kHz.
  TWBR = 0x0C;
  
  // Enable TWI.
  TWCR = (1 << TWEN);
}

// Sends start signal and wait for it to finish the job.
void x_twi_start() {
  TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
  while ((TWCR & (1 << TWINT)) == 0) {
    // x_yield();
  }
}

void x_twi_stop() {
  TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
}

void x_twi_write(uint8_t u8data) {
  TWDR = u8data;
  TWCR = (1 << TWINT) | (1 << TWEN);
  while ((TWCR & (1 << TWINT)) == 0) {
    // x_yield();
  }
}

uint8_t x_twi_read_ack() {
  TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
  while ((TWCR & (1 << TWINT)) == 0) {
    // x_yield();
  }
  return TWDR;
}

// read byte with NACK.
uint8_t x_twi_read_nack() {
  TWCR = (1 << TWINT) | (1 << TWEN);
  while ((TWCR & (1 << TWINT)) == 0) {
    // x_yield();
  }
  return TWDR;
}

uint8_t x_twi_get_status() {
  uint8_t status;
  status = TWSR & 0xF8;
  return status;
}

bool x_twi_putc(uint8_t device_address, uint8_t c) {
  // Start Condition.
  x_twi_start();

  x_usart_puts("start\n");

  // Check acknowledgement.
  if (x_twi_get_status() != TWI_S_START) return false;

  x_usart_puts("start ack\n");

  // Send Address Packet. 7 address bits and one read/write.
  // x_twi_write((device_address << 1) & 0b11111110);
  x_twi_write((device_address << 1) | 1);

  x_usart_puts("send addr\n");
  // x_twi_write(device_address << 1);

  uint8_t status1 = x_twi_get_status();
  for (int i = 0; i < 8; i++) {
    if (status1 >> (7 - i) & 1) {
      x_usart_putc('1');
    } else {
      x_usart_putc('0');
    }
  }
  x_usart_putc('\n');

  // Check acknowledgement.
  if (x_twi_get_status() != TWI_S_SLAW_ACK) return false;

  x_usart_puts("addr ack\n");

  // Send Data Packet.
  x_twi_write(c);

  x_usart_puts("send data\n");

  // Check acknowledgement.
  if (x_twi_get_status() != TWI_S_DATA_ACK) return false;

  x_usart_puts("data ack\n");

  // Stop Condition.
  x_twi_stop();

  x_usart_puts("stop\n");

  return true;
}