#ifndef LCD_H_
  #define LCD_H_

  #include "acx/acx.h"
  #include "acx/acx-twi.h"

  #define LCD_ADDRESS 0x27

  #define LCD_CLEARDISPLAY    0x01
  #define LCD_RETURNHOME      0x02
  #define LCD_ENTRYMODESET    0x04
  #define LCD_DISPLAYCONTROL  0x08
  #define LCD_CURSORSHIFT     0x10
  #define LCD_FUNCTIONSET     0x20
  #define LCD_SETCGRAMADDR    0x40
  #define LCD_SETDDRAMADDR    0x80

#endif