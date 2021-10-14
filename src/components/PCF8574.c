#include "PCF8574.h"

//pin status
volatile uint8_t pcf8574_pinstatus;

void pcf8574_init() {
  pcf8574_pinstatus = 0;
}

uint8_t pcf8574_getoutput() {
  return pcf8574_pinstatus;
}

uint8_t pcf8574_getoutputpin(uint8_t pin) {
  return (pcf8574_pinstatus >> pin) & 0b00000001;
}

bool pcf8574_setoutput(uint8_t data) {
  return x_twi_putc(PCF8574_ADDRESS, data, true);
}

bool pcf8574_setoutputpin(uint8_t deviceid, uint8_t pin, bool data) {
  // Ensure the pin valid.
  if (pin >= PCF8574_MAXPINS) return false;

  // Set or clear the bit.
  if (data) {
    pcf8574_pinstatus |= (0x01 << pin);
  } else {
    pcf8574_pinstatus &= ~(0x01 << pin);
  }

  // Update device.
  return x_twi_putc(PCF8574_ADDRESS, pcf8574_pinstatus, true);
}

uint8_t pcf8574_getinput() {
  uint8_t val;
  if (x_twi_recieve(PCF8574_ADDRESS, &val)) {
    return val;
  } else {
    return 0x00;
  }
}

bool pcf8574_getinputpin() {
  return x_twi_recieve(PCF8574_ADDRESS);
}