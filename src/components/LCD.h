#ifndef LCD_H_
  #define LCD_H_

  #include "acx/acx.h"
  #include "acx/acx-twi.h"
  #include "components/PCF8574.h"

  #define LCD_ADDRESS 0x27

  // LCD commands.
  #define LCD_CLEARDISPLAY    0x01  // Clears entire display and sets DDRAM address 0 in address counter.
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

  // Dataport pin function.
  #define LCD_DP_RS   0x01  // Register select.
  #define LCD_DP_RW   0x02  // Read or write.
  #define LCD_DP_E    0x04  // Starts data read/write.
  #define LCD_DP_LED  0x08
  #define LCD_DP_DB0  0x10
  #define LCD_DP_DB1  0x20
  #define LCD_DP_DB2  0x40
  #define LCD_DP_DB3  0x80
  








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
  #define LCD_ENTRY_DEC            0x04 0b100  /* display shift off, dec cursor move dir */
  #define LCD_ENTRY_DEC_SHIFT      0x05 0b101  /* display shift on,  dec cursor move dir */
  #define LCD_ENTRY_INC_           0x06 0b110  /* display shift off, inc cursor move dir */
  #define LCD_ENTRY_INC_SHIFT      0x07 0b111  /* display shift on,  inc cursor move dir */

  #define LCD_MODE_DEFAULT     ((1<<LCD_ENTRY_MODE) | (1<<LCD_ENTRY_INC) )

  #define LCD_LINES           4     /**< number of visible lines of the display */
  #define LCD_DISP_LENGTH    20     /**< visibles characters per line of the display */
  #define LCD_LINE_LENGTH  0x40     /**< internal line length of the display    */
  #define LCD_START_LINE1  0x00     /**< DDRAM address of first char of line 1 */
  #define LCD_START_LINE2  0x40     /**< DDRAM address of first char of line 2 */
  #define LCD_START_LINE3  0x14     /**< DDRAM address of first char of line 3 */
  #define LCD_START_LINE4  0x54     /**< DDRAM address of first char of line 4 */
  #define LCD_WRAP_LINES      1     /**< 0: no wrap, 1: wrap at end of visibile line */



  void LCD_init();
  void LCD_e_toggle(void);
  void LCD_write(uint8_t data, uint8_t rs);
  void LCD_write_command(uint8_t cmd);
  void LCD_write_data(uint8_t data);
  bool LCD_read(uint8_t* dest, uint8_t rs);
  uint8_t LCD_waitbusy(void);
  void LCD_gotoxy(uint8_t x, uint8_t y);
  int LCD_getxy(void);
  void LCD_putc(char c);
  void LCD_puts(const char* s);
  void LCD_set_backlight(bool val);

  uint8_t LCD_create_custom_char(uint8_t charnum, const uint8_t * values);
  void LCD_clear_CGRAM();
#endif