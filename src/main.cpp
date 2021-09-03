#include "main.h"

using namespace std;

#include "MenuItem.h"
#include "MenuLeaf.h"
#include "MenuBranch.h"

void testFunc(void) {
  Serial.println("test function");
}

MenuBranch branch0("Test Branch", 4);
MenuLeaf leaf0("Test Leaf 0", &testFunc, &testFunc, &testFunc, &testFunc);
MenuLeaf leaf1("Test Leaf 1", &testFunc, &testFunc, &testFunc, &testFunc);
MenuLeaf leaf2("Test Leaf 2", &testFunc, &testFunc, &testFunc, &testFunc);

void timerIsr() {
  encoder->service();
}

void setup(void) {
  // Setup serial for debugging.
  Serial.begin(9600);

  // Initialize the LCD.
  lcd = new LiquidCrystal_I2C(0x27, 16, 2);
  lcd->init();
  lcd->backlight();

  // Initialize the Encoder.
  encoder = new ClickEncoder(A1, A0, A2);
  encoder->setAccelerationEnabled(false);

  // Initialize the timer.
  Timer1.initialize(1000);
  Timer1.attachInterrupt(timerIsr); 

  // Clear the LCD in case there was anything on there already.
  lcd->clear();

  branch0.addChild(&leaf0);
  branch0.addChild(&leaf1);
  branch0.addChild(&leaf2);

  currentMenuItem = &branch0;
  currentMenuItem->Print();
}

void loop(void) {
  // Read the encoder and apply the scroll.
  last = encoder->getValue();
  if (last == 1) {
    currentMenuItem->ScrollLeft();
  } else if (last == -1) {
    currentMenuItem->ScrollRight();
  }
}