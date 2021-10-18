#include "LCD.h"

static inline void _delayFourCycles(unsigned int __count) {
  if ( __count == 0 )    
  __asm__ __volatile__( "rjmp 1f\n 1:" );    // 2 cycles
  else
  __asm__ __volatile__ (
  "1: sbiw %0,1" "\n\t"                  
  "brne 1b"                              // 4 cycles/loop
  : "=w" (__count)
  : "0" (__count)
  );
}
// #define delay_usec(us)  _delayFourCycles( ( ( 1*(F_CPU/4000) )*us)/1000 )
#define delay_usec(us)  _delayFourCycles( ( ( 1*(F_CPU/4000) )*us)/1000 )
#define lcd_e_delay() __asm__ __volatile__( "rjmp 1f\n 1:" );

volatile uint8_t dataport = 0;

bool LCD_backlight;
uint8_t LCD_lines;
uint8_t LCD_displayfunction;

void LCD_init() {
  // Initialize the pcf8574.
  pcf8574_init();

  // Reset dataport
  dataport = 0b00000000;
  pcf8574_setoutput(dataport);

  // Wait 15ms or more after power-on.
  x_delay(16);

  // Function set 1.
  dataport |= LCD_FUNCTIONSET | LCD_FS_8BITMODE;
  pcf8574_setoutput(dataport);
  LCD_e_toggle();

  // Wait for more than 4.1 ms.  
  delay_usec(4992);

  // Function set 2.
  LCD_e_toggle();

  // Wait for more than 100 µs
  delay_usec(64);
    
  // Function set 3.
  LCD_e_toggle();

  // Wait for more than 100 µs
  delay_usec(64);

  // Function set 4.
  dataport = LCD_FUNCTIONSET | LCD_FS_4BITMODE;
  pcf8574_setoutput(dataport);
  LCD_e_toggle();

  // Wait for more than 100 µs
  delay_usec(64);

  // Function set 5
  LCD_write_command(LCD_FUNCTIONSET | LCD_FS_2LINE | LCD_FS_5x8DOTS);

  LCD_write_command(LCD_DISPLAYCONTROL);  // Turn display off.
  LCD_write_command(LCD_CLEARDISPLAY);    // Clear Display.
  LCD_write_command(LCD_ENTRYMODESET | LCD_EM_SHIFT | LCD_EM_INC);
  LCD_write_command(LCD_DISPLAYCONTROL | LCD_DC_DISPLAYON | LCD_DC_CURSORON | LCD_DC_BLINKON);  // Turn display on


  /* now configure for 4bit mode */
  // dataport &= ~_BV(LCD_DP_D0);
  // pcf8574_setoutput(dataport);
  // LCD_e_toggle();
  // delay_usec(64);           /* some displays need this additional delay */
  

  /* from now the LCD only accepts 4 bit I/O, we can use lcd_command() */    

  // Set bit mode.
  // dataport |= LCD_8BITMODE;

  // Set num of lines.
  // LCD_lines = lines;
  // if (lines > 1) dataport |= LCD_2LINE;

  // Set dot size.
  // LCD_displayfunction |= LCD_4BITMODE;

  // LCD_write_command(LCD_FUNCTION_4BIT_2LINES);      /* function set: display lines  */


  
}


// Toggle Enable Pin to initiate write.
void LCD_e_toggle(void) {
  pcf8574_setoutputpin(LCD_DP_E, true);
  lcd_e_delay();
  pcf8574_setoutputpin(LCD_DP_E, false);
}


uint8_t LCD_read(uint8_t rs) {
  uint8_t data = 0;

  // Set the rs and rw pins.
  if (rs) dataport |= LCD_DP_RS;
  else dataport &= ~LCD_DP_RS;
  dataport &= ~LCD_DP_RW;
  pcf8574_setoutput(dataport);

  // Get the output.
  uint8_t output;

  // Read high nibble.
  pcf8574_getinput(&output);
  lcd_e_delay();
  data |= output & 0b11110000;
  pcf8574_setoutputpin(LCD_DP_E, false);

  lcd_e_delay();

  pcf8574_getinput(&output);
  lcd_e_delay();
  data |= (output & 0b11110000) >> 4;
  pcf8574_setoutputpin(LCD_DP_E, false);

  return data;
}

// Loops while lcd is busy, returns address counter.
uint8_t LCD_waitbusy(void) {
  uint8_t c;

  // Wait until busy flag is cleared.
  while (c & _BV(LCD_BUSY)) {
    c = LCD_read(0);
  }

  // The address counter is updated 4us after the busy flag is cleared.
  while ((c = LCD_read(0)) & (1<<LCD_BUSY)) {}

  // Now read the address counter and return it.
  return (LCD_read(0));
}

void LCD_write(uint8_t data, uint8_t rs) {
  // Set the rs and rw pins.
  if (rs) dataport |= LCD_DP_RS;
  else dataport &= ~LCD_DP_RS;
  dataport &= ~LCD_DP_RW;
  pcf8574_setoutput(dataport);

  // Output high nibble.
  dataport = (data & 0b11110000) | (dataport & 0b00001111);
  pcf8574_setoutput(dataport);
  LCD_e_toggle();

  // Output low nibble.
  dataport = ((data << 4) & 0b11110000) | (dataport & 0b00001111);
  pcf8574_setoutput(dataport);
  LCD_e_toggle();

  // Set all data pins high (inactive).
  dataport |= 0b11110000;
  pcf8574_setoutput(dataport);
}

void LCD_write_command(uint8_t cmd) {
  LCD_waitbusy();
  LCD_write(cmd, 0);
}

void LCD_set_backlight(bool on) {
  if (on) {
    dataport |= LCD_DP_LED;
  } else {
    dataport &= ~LCD_DP_LED;
  }
  pcf8574_setoutput(dataport);
}
void LCD_gotoxy(uint8_t x, uint8_t y) {
  if (y == 0) LCD_write_command(LCD_SETDDRAMADDR + LCD_START_LINE1 + x);
  else if (y == 1) LCD_write_command(LCD_SETDDRAMADDR + LCD_START_LINE2 + x);
  else if (y == 2) LCD_write_command(LCD_SETDDRAMADDR + LCD_START_LINE3 + x);
  else LCD_write_command(LCD_SETDDRAMADDR + LCD_START_LINE4 + x);
}

void LCD_putc(char c) {
  uint8_t pos;

  pos = LCD_waitbusy();   // read busy-flag and address counter

  LCD_write((1<<LCD_DDRAM)+LCD_START_LINE2,0);

  if ( pos == LCD_START_LINE1+LCD_DISP_LENGTH ) {
      LCD_write((1<<LCD_DDRAM)+LCD_START_LINE2,0);    
  }else if ( pos == LCD_START_LINE2+LCD_DISP_LENGTH ) {
      LCD_write((1<<LCD_DDRAM)+LCD_START_LINE3,0);
  }else if ( pos == LCD_START_LINE3+LCD_DISP_LENGTH ) {
      LCD_write((1<<LCD_DDRAM)+LCD_START_LINE4,0);
  }else if ( pos == LCD_START_LINE4+LCD_DISP_LENGTH ) {
      LCD_write((1<<LCD_DDRAM)+LCD_START_LINE1,0);
  }
}