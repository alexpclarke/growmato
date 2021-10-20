#include "pcf8574.h"

/* ----- GLOBAL VARIABLES ----- */

//pin status
volatile uint8_t pcf8574_pinstatus;

/* ----- PCF8574 FUNCTIONS ----- */

// Reset the pcf8574 status.
void pcf8574_init() {
  pcf8574_pinstatus = 0b00000000;
}

// Get the status.
uint8_t pcf8574_getoutput() {
  return pcf8574_pinstatus;
}

uint8_t pcf8574_getoutputpin(uint8_t pin) {
  return (pcf8574_pinstatus >> pin) & 0b00000001;
}

bool pcf8574_setoutput(uint8_t data) {
  pcf8574_pinstatus = data;
  return x_twi_putc(PCF8574_ADDRESS, data, true);
}

bool pcf8574_setoutputpin(uint8_t pin, bool data) {
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

bool pcf8574_getinput(uint8_t* dest) {
  return x_twi_getc(PCF8574_ADDRESS, dest, true);
}