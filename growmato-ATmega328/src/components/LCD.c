#include "LCD.h"

#define lcd_e_delay() __asm__ __volatile__( "rjmp 1f\n 1:" );

volatile uint8_t dataport = 0;
volatile bool LCD_backlight;
volatile bool LCD_wrap;

void LCD_thread() {
  LCD_init();
  LCD_set_backlight(true);
  LCD_set_pos(1, 0);
  LCD_puts("- Growmato  v1.1 -");

  LCD_set_pos(0, 1);
  LCD_puts("Temp.:  XXXF");
  LCD_set_pos(0, 2);
  LCD_puts("R.H.:   000%");
  LCD_set_pos(0, 3);
  LCD_puts("Moist.: 000%");

  while (1) {
    x_delay(2000);
    LCD_set_pos(8, 1);
    LCD_puts(intToString(myEnv.AirTempF));
    LCD_set_pos(8, 2);
    LCD_puts(intToString(myEnv.AirRH));
    LCD_set_pos(8, 3);
    LCD_puts(intToString(myEnv.SoilMoist));
  }
}

// Initializes the LCD.
void LCD_init() {
  // Initialize the pcf8574.
  pcf8574_init();

  // Reset dataport
  dataport = 0b00000000;
  pcf8574_setoutput(dataport);

  // Wait 15ms or more after power-on.
  x_delay_usec(16000);

  // Function set 1.
  dataport |= LCD_FUNCTIONSET | LCD_FS_8BITMODE;
  pcf8574_setoutput(dataport);
  LCD_e_toggle();

  // Wait for more than 4.1 ms.  
  x_delay_usec(4992);

  // Function set 2.
  LCD_e_toggle();

  // Wait for more than 100 µs
  x_delay_usec(64);
    
  // Function set 3.
  LCD_e_toggle();

  // Wait for more than 100 µs
  x_delay_usec(64);

  // Function set 4.
  dataport = LCD_FUNCTIONSET | LCD_FS_4BITMODE;
  pcf8574_setoutput(dataport);
  LCD_e_toggle();

  // Wait for more than 100 µs.
  x_delay_usec(64);

  // Function set 5. (the one that counts)
  LCD_write(LCD_FUNCTIONSET | LCD_FS_2LINE | LCD_FS_5x8DOTS, LCD_RS_IR);

  // Turn display off.
  LCD_write(LCD_DISPLAYCONTROL, LCD_RS_IR);

  // Clear Display.
  LCD_write(LCD_CLEARDISPLAY, LCD_RS_IR);

  // Set entry mode.
  LCD_write(LCD_ENTRYMODESET | LCD_EM_NOSHIFT | LCD_EM_INC, LCD_RS_IR);

  // Set display control.
  LCD_write(LCD_DISPLAYCONTROL | LCD_DC_DISPLAYON | LCD_DC_CURSOROFF
    | LCD_DC_BLINKOFF, LCD_RS_IR);

  // Reset values.
  LCD_set_wrap(false);
  LCD_set_backlight(false);
}

// Toggle Enable Pin to initiate write.
void LCD_e_toggle(void) {
  pcf8574_setoutputpin(LCD_DP_E, true);
  lcd_e_delay();
  pcf8574_setoutputpin(LCD_DP_E, false);
}

// Reads from the LCD. Returns whether it is successful or not.
bool LCD_read(uint8_t* dest, uint8_t rs) {
  // Set the rs and rw pins.
  if (rs) dataport |= _BV(LCD_DP_RS);
  else dataport &= ~_BV(LCD_DP_RS);
  dataport |= _BV(LCD_DP_RW);
  pcf8574_setoutput(dataport);

  // Get the output.
  uint8_t data = 0b00000000;
  uint8_t output;

  // Read high 4 bits.
  pcf8574_setoutputpin(LCD_DP_E, true);
  lcd_e_delay();
  if (!pcf8574_getinput(&output)) return false;
  lcd_e_delay();
  data |= output & 0b11110000;
  pcf8574_setoutputpin(LCD_DP_E, false);
  lcd_e_delay();

  // Read low 4 bits.
  pcf8574_setoutputpin(LCD_DP_E, true);
  lcd_e_delay();
  if (!pcf8574_getinput(&output)) return false;
  lcd_e_delay();
  data |= (output & 0b11110000) >> 4;
  pcf8574_setoutputpin(LCD_DP_E, false);

  // Successfully read.
  return true;
}

// Loops while LCD is busy, returns address counter.
void LCD_waitbusy(void) {
  // Wait until busy flag is cleared.
  uint8_t c;
  while (c & _BV(LCD_BF)) {
    if (!LCD_read(&c, 0)) {
      // failed to read
      x_crash();
    }
  }

  // The address counter is updated 4us after the busy flag is cleared.
  x_delay_usec(4);
}

// Write a byte to the LCD.
void LCD_write(uint8_t data, uint8_t rs) {
  // Wait until the LCD is ready.
  LCD_waitbusy();

  // Set the rs and rw pins.
  if (rs) dataport |= _BV(LCD_DP_RS);
  else dataport &= ~_BV(LCD_DP_RS);
  dataport &= ~_BV(LCD_DP_RW);
  pcf8574_setoutput(dataport);

  // Output high 4 bits.
  dataport = (data & 0b11110000) | (dataport & 0b00001111);
  pcf8574_setoutput(dataport);
  LCD_e_toggle();

  // Output low 4 bits.
  dataport = ((data << 4) & 0b11110000) | (dataport & 0b00001111);
  pcf8574_setoutput(dataport);
  LCD_e_toggle();

  // Set all data pins high (inactive).
  dataport |= 0b11110000;
  pcf8574_setoutput(dataport);
}

// Turn the backlight on or off.
void LCD_set_backlight(bool val) {
  if (val) {
    dataport |= _BV(LCD_DP_LED);
  } else {
    dataport &= ~_BV(LCD_DP_LED);
  }
  pcf8574_setoutput(dataport);
}

// Sets the cursor to the (x, y) coordinate.
bool LCD_set_pos(uint8_t x, uint8_t y) {
  // Validate the x and y.
  if (x >= LCD_COLS || y >= LCD_ROWS) return false;

  if (y == 0) {
    LCD_write(LCD_SETDDRAMADDR + LCD_LINE0 + x, LCD_RS_IR);
  } else if (y == 1) {
    LCD_write(LCD_SETDDRAMADDR + LCD_LINE1 + x, LCD_RS_IR);
  } else if (y == 2) {
    LCD_write(LCD_SETDDRAMADDR + LCD_LINE2 + x, LCD_RS_IR);
  } else {
    LCD_write(LCD_SETDDRAMADDR + LCD_LINE3 + x, LCD_RS_IR);
  }

  // Succeessfully got the coordinates.
  return true;
}

// Get the row and column of the 
void LCD_get_pos(uint8_t* x, uint8_t* y) {
  // Wait untill the LCD is ready.
  LCD_waitbusy();

  // Read the position.
  uint8_t pos;
  LCD_read(&pos, LCD_RS_IR);

  *y = pos % LCD_COLS;
  *x = pos - (*y * LCD_COLS);
}

// Clears the screen.
void LCD_clear() {
  LCD_write(LCD_CLEARDISPLAY, LCD_RS_IR);
}

// Write a character to the LCD.
bool LCD_putc(char c) {
  uint8_t x;
  uint8_t y;
  LCD_get_pos(&x, &y);

  // Ensure the character isn't being written off the edge.
  if (x >= LCD_COLS) {
    if (LCD_wrap) {
      // Attempt to update the position, if that fails, fail.
      y++;
      x = 0;
      if(!LCD_set_pos(x, y)) return false;
    } else {
      // Trying to write to an invalid position.
      return false;
    }
  }

  // Write the character to the DDRAM
  LCD_write(c, LCD_RS_DR);

  // Successfully wrote the character.
  return true;
}

// Write a string to the LCD.
uint8_t LCD_puts(char* s) {
  char* ptr = s;
  while (*ptr != '\0') {
    LCD_putc(*ptr);
    ptr++;
  }
  return 0;
}

// Sets the wrap property.
void LCD_set_wrap(bool on) {
  LCD_wrap = on;
}