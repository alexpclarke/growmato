#include <Arduino.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <ClickEncoder.h>
#include <TimerOne.h>

// #include <Vector.h>


#define MENU_SIZE 5
#define MIN_TEMP 0
#define MAX_TEMP 100

class MenuItem;
class MainMenu;
class SetTemp;

LiquidCrystal_I2C* lcd;
ClickEncoder* encoder;
int16_t last;
MenuItem* currentMenuItem;

int16_t temp;