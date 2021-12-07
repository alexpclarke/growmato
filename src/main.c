/* ----- Includes ----- */
#include "acx/acx.h"
#include "components/LCD.h"
#include "components/LED.h"
#include "components/StemmaSoil.h"
#include "components/esp8266.h"
#include "components/SHT30.h"
#include "MachineData.h"

void LCD_thread() {
  LCD_init();
  LCD_set_backlight(true);
  LCD_set_pos(1, 0);
  LCD_puts("- Growmato  v1.1 -");

  LCD_set_pos(0, 1);
  LCD_puts("Temp: XXXF");
  LCD_set_pos(0, 2);
  LCD_puts("RH:   000%");

  while (1) {
    x_delay(2000);
    LCD_set_pos(6, 1);
    LCD_puts("420");
    LCD_set_pos(6, 2);
    LCD_puts(" 69");
  }
}

void esp8266_thread() {
  while (1) {
    x_yield();
  }
};

int main(void) {
  x_init();
  x_usart_init(9600, 8, P_NONE, 1, false);
  x_twi_init();

  x_new(4, SHT30_thread, true);
  // x_new(3, StemmaSoil_thread, true);
  x_new(2, LED_thread, true);
  x_new(1, LCD_thread, true);
  x_new(0, esp8266_thread, true);
}

void floatToInt();