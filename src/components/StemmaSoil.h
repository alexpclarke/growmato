#ifndef STEMMASOIL_H_
    #define STEMMASOIL_H_

    #include "acx/acx.h"
    #include "acx/acx-twi.h"

    #define STEMMASOIL_STATUS_BASE 0x00
    #define STEMMASOIL_STATUS_TEMP 0x04
    #define STEMMASOIL_TOUCH_BASE 0x0F
    #define STEMMASOIL_TOUCH_CHANNEL_OFFSET 0x10

    float getTemp();
    char * floatToString(float val);
    //uint16_t getMoist(uint8_t pin);
    uint16_t getMoist();
#endif