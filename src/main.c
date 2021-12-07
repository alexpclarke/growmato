/* ----- Includes ----- */
#include "acx/acx.h"
#include "components/LCD.h"
#include "components/LED.h"
#include "components/StemmaSoil.h"
#include "components/esp8266.h"
#include "components/SHT30.h"
#include "MachineData.h"

int main(void) {
  x_init();
  x_usart_init(9600, 8, P_NONE, 1, false);
  x_twi_init();

  x_new(3, SHT30_thread, true);
  x_new(2, LED_thread, true);
  x_new(1, LCD_thread, true);
  x_new(0, esp8266_thread, true);
}

void floatToInt();