#include <Arduino.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <ClickEncoder.h>
#include <TimerOne.h>

#define MAX_LABEL_LENGTH 14
#define MIN_TEMP 0
#define MAX_TEMP 100

class MenuItem;
class MenuBranch;
class MenuLeaf;

LiquidCrystal_I2C* lcd;
ClickEncoder* encoder;
int16_t last;
MenuItem* currentMenuItem;