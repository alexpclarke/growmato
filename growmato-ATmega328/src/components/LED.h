
#include "acx/acx.h"

#ifndef LED_H_
  #define LED_H_

  #define LED_OFF   0x00
  #define LED_ON    0x01
  #define LED_FLASH 0x02

  void LED_thread(void);
  void LED_init(void);
  void LED_setR(uint8_t mode);
  void LED_setG(uint8_t mode);
  void LED_setB(uint8_t mode);
#endif