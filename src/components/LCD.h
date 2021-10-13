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

  // flags for function set
  #define LCD_8BITMODE 0x10
  #define LCD_4BITMODE 0x00
  #define LCD_2LINE 0x08
  #define LCD_1LINE 0x00
  #define LCD_5x10DOTS 0x04
  #define LCD_5x8DOTS 0x00

  // flags foor backlight
  #define LCD_BACKLIGHT 0x08
  #define LCD_NOBACKLIGHT 0x00  

  void LCD(void);
  void LCD_init(uint8_t cols, uint8_t lines);
  void LCD_set_backlight(bool val);
#endif