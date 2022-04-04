#ifndef PCF8574_H_
  #define PCF8574_H_

  #include "acx/acx.h"
  #include "acx/acx-twi.h"

  /* ----- GENERAL INFO ----- */

  #define PCF8574_ADDRESS 0x27
  #define PCF8574_MAXPINS 8

  /* ----- PUBLIC PROTOTYPES----- */
  
  void pcf8574_init(void);
  uint8_t pcf8574_getoutput();
  bool pcf8574_setoutput(uint8_t data);
  uint8_t pcf8574_getoutputpin(uint8_t pin);
  bool pcf8574_setoutputpin(uint8_t pin, bool data);
  bool pcf8574_getinput(uint8_t* dest);
#endif
