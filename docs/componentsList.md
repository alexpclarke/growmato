# Components List

## 1. LCD Display

Purchase Link: [Amazon](https://www.amazon.com/SunFounder-Serial-Module-Arduino-Mega2560/dp/B01GPUMP9C/ref=sr_1_5?dchild=1&keywords=i2c+lcd&qid=1631548048&sr=8-5)

### Associated Values

```c
bool LCD_backlight;
bool LCD_inverted;
uint8_t LCD_displayfunction
```

### Connections

GND - GND
VCC - 5V
SDA - SDA
SCL - SCL

### LCD Controller

Model: HD44780U
Datasheet: [SparkFun](https://www.sparkfun.com/datasheets/LCD/HD44780.pdf)
Supply Voltage: 2.7V - 5.5V (5V typical)
Max Frequency: 2MHz

### I2C Driver

Model: PCF8574T
Datasheet: [NXP](https://www.nxp.com/docs/en/data-sheet/PCF8574_PCF8574A.pdf)
Default Address: 0x3F
Max Frequency: 100 kHz

## 2. Soil Moisture Sensor

Purchase Link: [Adafruit](https://www.adafruit.com/product/4026)
Datasheet: [Adafruit](https://cdn-learn.adafruit.com/downloads/pdf/adafruit-stemma-soil-sensor-i2c-capacitive-moisture-sensor.pdf?timestamp=1634069203)
Default Address: 0x36

### Associated Values

```c
bool power;
uint8_t moisture;
```

### Connections

GND - GND
VCC - 5V
SDA - SDA
SCL - SCL

### Controller

Model: ATSAMD10D14A
Datasheet: [Microchip](https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-42242-SAM-D10_Datasheet.pdf)
Supply Voltage: 1.62V – 3.63V

### SparkFun Qwiic (I2C):
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
### Address: 
    Can be selected (SHT30)

### Commands:
    Fetch Data: 0xe000
    Periodic Measurement with ART: 0x2b32
    Break: 0x3093
    Heater Enable/Disable: 0x30
    Soft Reset: 0x30a2
    Clear status register: 0x3041
    Read out of status of register: 0xf32d


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






# Parts for watering solution:

## Video for watering solution: 
    https://www.youtube.com/watch?v=ioSYlxHlYdI

## Valve:
### Links:
    https://www.adafruit.com/product/997    
    https://media.digikey.com/pdf/Data%20Sheets/Adafruit%20PDFs/997_Web.pdf
    https://bc-robotics.com/tutorials/controlling-a-solenoid-valve-with-arduino/
    https://www.qcsupply.com/120150-solenoid-valve-with-flow-control-1-110-volt.html (Flow Control)
    https://www.qcsupply.com/media/product_attachments/attachment_file/1/2/120150_SS.pdf (Flow Control)

### Specifications:
#### Power Options: 
    Off/On
#### Data type:
    bool Power
#### Address: N/A?


# Realtime Clock

## Datasheet:
    https://datasheets.maximintegrated.com/en/ds/DS1307.pdf

## Address:
    The RTC registers are located in address locations 00h to 07h.

## Power
    VCC = 4.5V - 5.5V