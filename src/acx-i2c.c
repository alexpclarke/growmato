#include "acx-i2c.h"

bool x_i2c_init() {
  return true;
}

bool x_i2c_putc(uint8_t device_address, uint8_t c) {
  // Set the I2C bus to write mode and set to idle.
  DDRC |= 0b00110000;
  PORTC |= 0b00110000;

  // Start Condition.
  // SCL remains high while SDA goes from high to low.

  // Send Address Packet. 7 address bits, one read/write bit and an acknowledge
  // bit

  // Send Data Packet. 
  // 

  // Stop Condition.
  // SCL remains high while SDA goes from low to high.

  return true;
}