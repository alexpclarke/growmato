/* ----- Includes ----- */
#include "acx/acx.h"
#include "components/LCD.h"
#include "components/LED.h"
#include "MachineData.h"



void flashLED() {
  DDRB |= _BV(DDB0) | _BV(DDB1) | _BV(DDB2);
  while (1) {
    PORTB &= ~_BV(PORTB2);
    PORTB |= _BV(PORTB0);
    x_delay(500);
    PORTB &= ~_BV(PORTB0);
    PORTB |= _BV(PORTB1);
    x_delay(500);
    PORTB &= ~_BV(PORTB1);
    PORTB |= _BV(PORTB2);
    x_delay(500);
  }
}

void USART_print() {
  x_usart_init(9600, 8, P_NONE, 1, false);

  while (1) {
    x_usart_puts("test\n");
    x_delay(2000);
  }
}

void USART_echo() {
  uint8_t maxlen = 20;
  char* buff = calloc(maxlen, sizeof(char));

  while (1) {
    if (x_usart_gets(maxlen, buff)) {
      x_usart_puts(buff);
    }
    x_yield();
  }
}

void LCD() {
  LCD_init();
  LCD_set_backlight(true);
  LCD_set_pos(3, 2);
  LCD_putc('x');
  LCD_putc('y');
  LCD_putc('z');

  while (1) {
    x_delay(2000);
  }
}

void esp8266() {
  if (!esp8266_init()) {
    x_usart_puts("ERR: esp init.\n");
    x_crash();
  }

  while (true) {
    
  }
}

int main(void) {
  DDRB |= _BV(DDB0) | _BV(DDB1) | _BV(DDB2);

  x_init();
  x_usart_init(9600, 8, P_NONE, 1, false);
  x_twi_init();
  
  x_new(1, flash, true);
  x_new(2, flashLED, true);
  // x_new(2, USART_echo, true);
  x_new(0, LCD, true);


  // while(1) {
  //   x_yield();
  // }
}