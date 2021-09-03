#include "main.h"

using namespace std;

#include "MenuItem.h"
#include "MainMenu.h"
#include "SetTemp.h"

MainMenu menu0(4, "Main Menu");
SetTemp menu1(0, "Set Temp");

void timerIsr() {
  encoder->service();
}

void setup() {
  // Setup serial for debugging.
  Serial.begin(9600);

  // Initialize the LCD.
  lcd = new LiquidCrystal_I2C(0x27, 16, 2);
  lcd->init();
  lcd->backlight();

  // Initialize the LCD.
  encoder = new ClickEncoder(A1, A0, A2);
  encoder->setAccelerationEnabled(false);

  // Initialize the timer.
  Timer1.initialize(1000);
  Timer1.attachInterrupt(timerIsr); 


  // SetTemp menu1;
  // menu0.addChild(&menu1);

  lcd->clear();

  // currentMenuItem = new MainMenu(4, "Main Menu");
  // currentMenuItem->addChild(SetTemp(0, "Set Temp"));

  
  menu0.addChild(&menu1);

  currentMenuItem = &menu0;
  // currentMenuItem->Print();
}

void loop() {
  currentMenuItem->Print();

  // Read the encoder and apply the scroll.
  last = encoder->getValue();
  if (last == 1) currentMenuItem->ScrollLeft();
  else if (last == -1) currentMenuItem->ScrollRight();
  if (last != 0) currentMenuItem->Print();

  // Delay 50ms.
  delay(20);
}