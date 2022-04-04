#include "acx/acx.h"
#include "acx/acx-twi.h"
#include "MachineData.h"
#include "components/LED.h"

#ifndef ESP8266_H_
  #define ESP8266_H_

  #define ESP8266_ADDRESS 0x04

  #define ESP8266_CMD_INIT        0x01
  #define ESP8266_CMD_UPDATE_ENV  0x02
  #define ESP8266_CMD_UPDATE_ENV  0x02

  // Prototypes
  void esp8266_thread(void);
  bool esp8266_init(void);
  bool esp8266_readUpdates(void);
  bool esp8266_update_env(void);
#endif