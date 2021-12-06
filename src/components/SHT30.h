#include "acx/acx.h"
#ifndef SHT30_H_
  #define SHT30_H_

  #define SHT30_ADDRESS 0x44

  bool SHT30_init(void);
  float SHT30_getTemp(void);
  float SHT30_getRelHumidity(void);
  float SHT30_getAbsHumidity(void);

#endif