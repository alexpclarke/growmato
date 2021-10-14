#include "LCD.h"

bool LCD_backlight;
uint8_t LCD_lines;
uint8_t LCD_displayfunction;

void LCD() {
  LCD_init(20, 4);

  while (1) {
    LCD_set_backlight(!LCD_backlight);
    x_delay(4000);
  }
}

void LCD_init(uint8_t cols, uint8_t lines) {
  // Reset the backlight.
  LCD_backlight = false;

  // Clear the display function.
  LCD_displayfunction = 0b00000000;
  
  // Set bit mode.
  LCD_displayfunction |= LCD_4BITMODE;

  // Set num of lines.
  LCD_lines = lines;
  if (lines > 1) LCD_displayfunction |= LCD_2LINE;

  // Set dot size.
  LCD_displayfunction |= LCD_4BITMODE;

  // TODO: Initialization dance.
}

void LCD_set_backlight(bool val) {
  LCD_backlight = val;
  uint8_t command = LCD_CLEARDISPLAY;
  if (LCD_backlight) {
    command &= 0x08;
  }
  // x_twi_transmit(LCD_ADDRESS, &command, 1, true);
}

void LCD_write() {
  
}