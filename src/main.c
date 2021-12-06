/* ----- Includes ----- */
#include "acx/acx.h"
#include "components/LCD.h"
#include "components/LED.h"
#include "components/StemmaSoil.h"
#include "components/esp8266.h"
#include "MachineData.h"

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

void LCD_thread() {
  LCD_init();
  LCD_set_backlight(true);
  LCD_set_pos(3, 2);
  LCD_putc('x');

  while (1) {
    x_delay(2000);
  }
}

void StemmaSoil_thread() {
  if (!esp8266_init()) {
    x_usart_puts("ERR: esp init.\n");
    x_crash();
  } else {
    x_usart_puts("esp init.\n");
  }

  while (true) {
    x_yield();
  }
}

void soil_thread() {
  StemmaSoil_init();

  while(1) {
    float temp = getTemp();
    if (temp == 0) {
      x_usart_puts("0");
    } else {
      x_usart_puts("not 0");
    }
    x_delay(2000);
  } 
}
void esp8266_thread() {
  while (1) {

  }
};
// void SHT30_thread();

int main(void) {
  x_init();
  x_usart_init(9600, 8, P_NONE, 1, false);
  x_twi_init();

  // x_new(4, SHT30_thread, true);
  // x_new(3, StemmaSoil_thread, true);
  // x_new(2, LED_thread, true);
  x_new(0, LCD_thread, true);
  // x_new(0, esp8266_thread, true);
}


