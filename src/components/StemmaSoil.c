#include "StemmaSoil.h"
#include <stdio.h>

float temp;
char * tempStr;

void StemmaSoil_init() {
    temp = getTemp();
    tempStr = floatToString(temp);
    x_usart_puts(tempStr);
}

char * floatToString(float val) {
    int len = snprintf(NULL, 0, "%f", val);
    char * result = (char *)malloc(len + 1);
    snprintf(result, len + 1, "%f", val);
    // do stuff with result
    //free(result);
    return result;
}

float getTemp() {
    /*def get_temp(self):
        """Read the temperature"""
        buf = bytearray(4)
        self.read(_STATUS_BASE, _STATUS_TEMP, buf, 0.005)
        buf[0] = buf[0] & 0x3F
        ret = struct.unpack(">I", buf)[0]
    return 0.00001525878 * ret */
    int32_t ret;
    uint8_t buf[4];
    if (x_twi_gets(STEMMASOIL_STATUS_BASE + STEMMASOIL_STATUS_TEMP, buf, 4, true)) {
        ret = ((uint32_t)buf[0] << 24) | ((uint32_t)buf[1] << 16) |
                    ((uint32_t)buf[2] << 8) | (uint32_t)buf[3];
    } else {x_crash();}
    return (1.0 / (1UL << 16)) * ret;
}

// UwU
//uint32_t getMoist() {
    /*def moisture_read(self):
        """Read the value of the moisture sensor"""
        buf = bytearray(2)

        self.read(_TOUCH_BASE, _TOUCH_CHANNEL_OFFSET, buf, 0.005)
        ret = struct.unpack(">H", buf)[0]
        time.sleep(0.001)

        # retry if reading was bad
        count = 0
        while ret > 4095:
            self.read(_TOUCH_BASE, _TOUCH_CHANNEL_OFFSET, buf, 0.005)
            ret = struct.unpack(">H", buf)[0]
            time.sleep(0.001)
            count += 1
            if count > 3:
                raise RuntimeError("Could not get a valid moisture reading.")

    return ret */
    //uint8_t buf[2];
    //uint8_t i;
    //for (i = 0; i < 3; i++) buf[i] = NULL;
    
//}