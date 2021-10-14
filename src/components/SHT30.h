#ifndef SHT30_h_
    #define SHT30_h_

    #include "acx/acx.h"
    #include "acx/acx-twi.h"

    #define SHT30_ADDRESS 0x44

    #define SHT3X_ACCURACY_HIGH 0x2400
    #define SHT3X_ACCURACY_MEDIUM 0x240b
    #define SHT3X_ACCURACY_LOW 0x2416
    
    #define SHT3X_ACCURACY_HIGH_DURATION 15
    #define SHT3X_ACCURACY_MEDIUM_DURATION 6
    #define SHT3X_ACCURACY_LOW_DURATION 4
    
    #define SHT3X_I2C_ADDRESS_44 0x44
    #define SHT3X_I2C_ADDRESS_45 0x45



#endif