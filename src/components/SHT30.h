#include "acx/acx.h"
#include <math.h>
#include "MachineData.h"
#include "tools.h"

#ifndef SHT30_H_
  #define SHT30_H_

  #define SHT30_ADDRESS 0x44

  void SHT30_thread();
  void SHT30_init(void);
  void SHT30_read();
  uint8_t SHT30_CRC8(uint8_t vals[2]);

#endif