#ifndef LCD_H_
  #define LCD_H_

  #include "acx/acx.h"
  #include "acx/acx-twi.h"
  #include "components/PCF8574.h"

  #define LCD_ADDRESS 0x27
  #define LCD_COLS    20
  #define LCD_ROWS    4

  // LCD commands.
  #define LCD_CLEARDISPLAY    0x01
  #define LCD_RETURNHOME      0x02
  #define LCD_ENTRYMODESET    0x04
  #define LCD_DISPLAYCONTROL  0x08
  #define LCD_CURSORSHIFT     0x10
  #define LCD_FUNCTIONSET     0x20
  #define LCD_SETCGRAMADDR    0x40
  #define LCD_SETDDRAMADDR    0x80

  // Flags for display control.
  #define LCD_DC_BLINKON    0x01
  #define LCD_DC_BLINKOFF   0x00
  #define LCD_DC_CURSORON   0x02
  #define LCD_DC_CURSOROFF  0x00
  #define LCD_DC_DISPLAYON  0x04
  #define LCD_DC_DISPLAYOFF 0x00

  // Flags for function set.
  #define LCD_FS_5x8DOTS  0x00
  #define LCD_FS_5x10DOTS 0x04
  #define LCD_FS_1LINE    0x00
  #define LCD_FS_2LINE    0x08
  #define LCD_FS_4BITMODE 0x00
  #define LCD_FS_8BITMODE 0x10

  //Flags for Entry Mode.
  #define LCD_EM_SHIFT    0x01
  #define LCD_EM_NOSHIFT  0x00
  #define LCD_EM_INC      0x02
  #define LCD_EM_DEC      0x00

  // DDRAM addresses of the start of each LCD line.
  #define LCD_LINE0 0x00
  #define LCD_LINE1 0x40
  #define LCD_LINE2 0x14
  #define LCD_LINE3 0x54

  // Dataport pins (IR Write)
  #define LCD_DP_RS   0  // Register select.
  #define LCD_DP_RW   1  // Read or write.
  #define LCD_DP_E    2  // Starts data read/write.
  #define LCD_DP_LED  3
  #define LCD_DP_DB0  4
  #define LCD_DP_DB1  5
  #define LCD_DP_DB2  6
  #define LCD_DP_DB3  7

  // RS values.
  #define LCD_RS_IR 0
  #define LCD_RS_DR 1

  // (IR Read)
  #define LCD_AC_MASK 0b01111111
  #define LCD_BF      7

  // Dont touch these.
  void LCD_e_toggle(void);
  bool LCD_read(uint8_t* dest, uint8_t rs);
  void LCD_waitbusy(void);
  void LCD_write(uint8_t data, uint8_t rs);

  // Use these.
  void LCD_init(void);
  void LCD_get_pos(uint8_t* x, uint8_t* y);
  void LCD_set_backlight(bool val);
  void LCD_set_wrap(bool val);
  bool LCD_set_pos(uint8_t x, uint8_t y);
  void LCD_clear(void);
  bool LCD_putc(char c);
  uint8_t LCD_puts(char* s);
  // void LCD_clear_CGRAM(void);
  // uint8_t LCD_create_custom_char(uint8_t charnum, const uint8_t * values);
  

  uint8_t LCD_create_custom_char(uint8_t charnum, const uint8_t * values);
  void LCD_clear_CGRAM();
#endif