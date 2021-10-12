#include "StemmaSoil.h"

void StemmaSoil() {
    /*
        soilSensor.moisture_read()
        soilSensor.get_temp()
        x_delay(1000)
    */
}

uint32_t getTemp() {
    /*def get_temp(self):
        """Read the temperature"""
        buf = bytearray(4)
        self.read(_STATUS_BASE, _STATUS_TEMP, buf, 0.005)
        buf[0] = buf[0] & 0x3F
        ret = struct.unpack(">I", buf)[0]
    return 0.00001525878 * ret */
    uint8_t buf[4];

}

// UwU
uint32_t getMoist() {
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
    uint8_t buf[2];
}