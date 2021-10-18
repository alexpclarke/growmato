#ifndef PCF8574_H_
  #define PCF8574_H_

  #include "acx/acx.h"
  #include "acx/acx-twi.h"

  #define PCF8574_ADDRESS (0x27) //device base address

  #define PCF8574_MAXPINS 8       // Max number of pins

  void pcf8574_init(void);
  uint8_t pcf8574_getoutput();
  bool pcf8574_setoutput(uint8_t data);
  uint8_t pcf8574_getoutputpin(uint8_t pin);
  bool pcf8574_setoutputpin(uint8_t pin, bool data);
  bool pcf8574_getinput(uint8_t* dest);
  bool pcf8574_getinputpin(bool* dest);
#endif