# Soil Moisture Sensor:

## Specifications:
### Power Options: 
    Off/On
### Data type: 
    int mositureLevel
    bool power
### Address: 
    0x36 (STEMMA)


## Adafruit STEMMA (I2C):
    https://learn.adafruit.com/introducing-adafruit-stemma-qt/technical-specs

## SparkFun Qwiic (I2C):
    https://www.sparkfun.com/products/17731
    http://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-2586-AVR-8-bit-Microcontroller-ATtiny25-ATtiny45-ATtiny85_Datasheet.pdf
    https://www.sparkfun.com/qwiic

# Humidity Sensor:

## Specifications: 
### Power Options: 
    Off/On
### Data type:
    int humidityLevel
    int temperature
    bool power
### address: 
    Can be selected (SHT30)

## Adafruit SHT30 Temperature and Humidity Sensor (I2C)
    https://www.adafruit.com/product/5064
    https://www.mouser.com/datasheet/2/682/Sensirion_Humidity_Sensors_SHT3x_Datasheet_digital-971521.pdf

## Adafruit encased temp & humidity (I2C):
    https://www.adafruit.com/product/1293
    https://cdn-shop.adafruit.com/datasheets/AM2315.pdf

## Adafruit AHT20 (I2C):
    https://www.adafruit.com/product/4566
    https://learn.adafruit.com/adafruit-aht20

## Adafruit DHT11 (I2C):
    https://www.adafruit.com/product/386
    https://learn.adafruit.com/dht?view=all




## LCD Display

Purchase Link: [Amazon](https://www.amazon.com/SunFounder-Serial-Module-Arduino-Mega2560/dp/B01GPUMP9C/ref=sr_1_5?dchild=1&keywords=i2c+lcd&qid=1631548048&sr=8-5)

### Associated Values
```c
bool backlight_on;
bool screen_inverted;
```

### Connections
GND - GND
VCC - 5V
SDA - SDA
SCL - SCL

### LCD Controller
Model: HD44780U
Datasheet: https://www.sparkfun.com/datasheets/LCD/HD44780.pdf
Supply Voltage: 2.7V - 5.5V (5V typical)
Max Frequency: 2MHz

### I2C Driver
Model: PCF8574T
Datasheet: https://www.nxp.com/docs/en/data-sheet/PCF8574_PCF8574A.pdf
Default Address: 0x3F


# Parts for watering solution:

## Specifications:


## Video for watering solution: 
    https://www.youtube.com/watch?v=ioSYlxHlYdI

## Valve:
    https://bc-robotics.com/shop/water-solenoid-valve-dn15/
    https://bc-robotics.com/tutorials/controlling-a-solenoid-valve-with-arduino/