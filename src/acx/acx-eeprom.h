#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include <stdlib.h>
#include <stdbool.h>
#include "acx.h"
#include "acx-twi.h"

#ifndef ACX_EEPROM_H_
  #define ACX_EEPROM_H_

  uint8_t x_eeprom_putc(uint16_t u16addr, uint8_t u8data);
  uint8_t x_eeprom_getc(uint16_t u16addr, uint8_t* u8data);
#endif