#ifndef LCD_H_
  #define LCD_H_

  #include "acx/acx.h"
  #include "acx/acx-twi.h"
  #include "components/PCF8574.h"

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

  #define LCD_DATA0_PIN    4            /**< pin for 4bit data bit 0  */
  #define LCD_DATA1_PIN    5            /**< pin for 4bit data bit 1  */
  #define LCD_DATA2_PIN    6            /**< pin for 4bit data bit 2  */
  #define LCD_DATA3_PIN    7            /**< pin for 4bit data bit 3  */
  #define LCD_RS_PIN       0            /**< pin  for RS line         */
  #define LCD_RW_PIN       1            /**< pin  for RW line         */
  #define LCD_E_PIN        2            /**< pin  for Enable line     */
  #define LCD_LED_PIN      3            /**< pin  for Led             */

  #define LCD_FUNCTION_4BIT_1LINE  0x20   /* 4-bit interface, single line, 5x7 dots */
  #define LCD_FUNCTION_4BIT_2LINES 0x28   /* 4-bit interface, dual line,   5x7 dots */
  #define LCD_FUNCTION_8BIT_1LINE  0x30   /* 8-bit interface, single line, 5x7 dots */
  #define LCD_FUNCTION_8BIT_2LINES 0x38   /* 8-bit interface, dual line,   5x7 dots */

  /* display on/off, cursor on/off, blinking char at cursor position */
  #define LCD_DISP_OFF             0x08   /* display off                            */
  #define LCD_DISP_ON              0x0C   /* display on, cursor off                 */
  #define LCD_DISP_ON_BLINK        0x0D   /* display on, cursor off, blink char     */
  #define LCD_DISP_ON_CURSOR       0x0E   /* display on, cursor on                  */
  #define LCD_DISP_ON_CURSOR_BLINK 0x0F   /* display on, cursor on, blink char      */

  /* instruction register bit positions, see HD44780U data sheet */
  #define LCD_CLR               0      /* DB0: clear display                  */
  #define LCD_HOME              1      /* DB1: return to home position        */
  #define LCD_ENTRY_MODE        2      /* DB2: set entry mode                 */
  #define LCD_ENTRY_INC         1      /*   DB1: 1=increment, 0=decrement     */
  #define LCD_ENTRY_SHIFT       0      /*   DB2: 1=display shift on           */
  #define LCD_ON                3      /* DB3: turn lcd/cursor on             */
  #define LCD_ON_DISPLAY        2      /*   DB2: turn display on              */
  #define LCD_ON_CURSOR         1      /*   DB1: turn cursor on               */
  #define LCD_ON_BLINK          0      /*     DB0: blinking cursor ?          */
  #define LCD_MOVE              4      /* DB4: move cursor/display            */
  #define LCD_MOVE_DISP         3      /*   DB3: move display (0-> cursor) ?  */
  #define LCD_MOVE_RIGHT        2      /*   DB2: move right (0-> left) ?      */
  #define LCD_FUNCTION          5      /* DB5: function set                   */
  #define LCD_FUNCTION_8BIT     4      /*   DB4: set 8BIT mode (0->4BIT mode) */
  #define LCD_FUNCTION_2LINES   3      /*   DB3: two lines (0->one line)      */
  #define LCD_FUNCTION_10DOTS   2      /*   DB2: 5x10 font (0->5x7 font)      */
  #define LCD_CGRAM             6      /* DB6: set CG RAM address             */
  #define LCD_DDRAM             7      /* DB7: set DD RAM address             */
  #define LCD_BUSY              7      /* DB7: LCD is busy                    */

  /* set entry mode: display shift on/off, dec/inc cursor move direction */
  #define LCD_ENTRY_DEC            0x04   /* display shift off, dec cursor move dir */
  #define LCD_ENTRY_DEC_SHIFT      0x05   /* display shift on,  dec cursor move dir */
  #define LCD_ENTRY_INC_           0x06   /* display shift off, inc cursor move dir */
  #define LCD_ENTRY_INC_SHIFT      0x07   /* display shift on,  inc cursor move dir */

  #define LCD_MODE_DEFAULT     ((1<<LCD_ENTRY_MODE) | (1<<LCD_ENTRY_INC) )

  // flags foor backlight
  #define LCD_BACKLIGHT 0x08
  #define LCD_NOBACKLIGHT 0x00  

  void LCD(void);
  void LCD_init(uint8_t cols, uint8_t lines);
  void LCD_e_toggle(void);
  void LCD_write(uint8_t data, uint8_t rs);
  void LCD_write_command(uint8_t cmd);
  void LCD_write_data(uint8_t data);
  uint8_t LCD_read(uint8_t rs);
  uint8_t LCD_waitbusy(void);
  void LCD_gotoxy(uint8_t x, uint8_t y);
  int LCD_getxy(void);
  void LCD_clrscr(void);
  void LCD_led(uint8_t onoff);
  void LCD_home(void);
  void LCD_putc(char c);
  void LCD_puts(const char* s);


  uint8_t LCD_create_custom_char(uint8_t charnum, const uint8_t * values);
  void LCD_clear_CGRAM();
#endif