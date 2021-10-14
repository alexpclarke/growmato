#include "LCD.h"

static inline void _delayFourCycles(unsigned int __count)
{
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
#define delay_usec(us)  _delayFourCycles( ( ( 1*(F_CPU/4000) )*us)/1000 )
#define lcd_e_delay() __asm__ __volatile__( "rjmp 1f\n 1:" );

volatile uint8_t dataport = 0;

bool LCD_backlight;
uint8_t LCD_lines;
uint8_t LCD_displayfunction;

void LCD_init(uint8_t cols, uint8_t lines) {
  // Initialize the pcf8574.
  pcf8574_init();

  // Reset dataport
  dataport = 0b00000000;
  pcf8574_setoutput(dataport);

  // Wait 16ms or more after power-on.
  x_delay(16);

  // Reset the backlight.
  // LCD_backlight = false;
  
  // // Set bit mode.
  // dataport |= LCD_8BITMODE;

  // // Set num of lines.
  // LCD_lines = lines;
  // if (lines > 1) dataport |= LCD_2LINE;

  // // Set dot size.
  // LCD_displayfunction |= LCD_4BITMODE;

  dataport |= _BV(LCD_DATA1_PIN);  // _BV(LCD_FUNCTION)>>4;
  dataport |= _BV(LCD_DATA0_PIN);  // _BV(LCD_FUNCTION_8BIT)>>4;

  // Initial write.
  pcf8574_setoutput(dataport);
  LCD_e_toggle();
  delay_usec(4992);

  /* repeat last command */ 
  LCD_e_toggle();
  delay_usec(64);
    
  /* repeat last command a third time */
  LCD_e_toggle();
  delay_usec(64);

  /* now configure for 4bit mode */
  dataport &= ~_BV(LCD_DATA0_PIN);
  pcf8574_setoutput(dataport);
  LCD_e_toggle();
  delay_usec(64);           /* some displays need this additional delay */
  
  /* from now the LCD only accepts 4 bit I/O, we can use lcd_command() */    

  LCD_command(LCD_FUNCTION_4BIT_2LINES);      /* function set: display lines  */

  LCD_command(LCD_DISP_OFF);              /* display off                  */
  LCD_clrscr();                           /* display clear                */
  LCD_command(LCD_MODE_DEFAULT);          /* set entry mode               */
  LCD_command(LCD_DISP_ON_CURSOR);                  /* display/cursor control       */
}


// Toggle Enable Pin to initiate write.
void LCD_e_toggle(void) {
  pcf8574_setoutputpin(LCD_E_PIN, true);
  lcd_e_delay();
  pcf8574_setoutputpin(LCD_E_PIN, false);
}

uint8_t LCD_waitbusy(void) {
  // TODO
  return 0x00;
}

void LCD_command(uint8_t cmd) {
  LCD_waitbusy();
  LCD_write(cmd, 0);
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