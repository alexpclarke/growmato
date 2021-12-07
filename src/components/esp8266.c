#include "esp8266.h"

uint32_t ipAddress;

void esp8266_thread() {
  // esp8266_init();

  while (1) {
    x_yield();
  }
};

bool esp8266_init(void) {
  // Start flashing the blue LED
  LED_setB(LED_FLASH);

  // Send the init command code.
  if (!x_twi_putc(ESP8266_ADDRESS, ESP8266_CMD_INIT, false)) {
    LED_setB(LED_OFF);
    x_usart_puts("1\n");
    return false;
  }

  // Send the MachineData.
  if (!x_twi_puts(ESP8266_ADDRESS, (uint8_t*)(&myMachine), sizeof(MachineSettings), true)) {
    LED_setB(LED_OFF);
    x_usart_puts("2\n");
    return false;
  }

  // Retrieve the IP address as a response.
  uint8_t buff[4];
  if(!x_twi_gets(ESP8266_ADDRESS, buff, 4, true)) {
    LED_setB(LED_OFF);
    x_usart_puts("3\n");
    return false;
  }
  ipAddress = *(uint32_t*)buff;

  // Determine if successful.
  if (ipAddress) {
    LED_setB(LED_OFF);
    return true;
  } else {
    LED_setB(LED_OFF);
    return false;
  }
}

// Send an updates environment to the ESP8266.
bool esp8266_update_env() {
  // Send the command code.
  x_twi_putc(ESP8266_ADDRESS, ESP8266_CMD_UPDATE_ENV, false);

  // Send the MachineData.
  if (!x_twi_puts(ESP8266_ADDRESS, (uint8_t*)(&(myMachine.env)), sizeof(MachineSettings), true)) {
    x_crash();
  }
  return true;
}

bool esp8266_readUpdates() {
  // Send the init command code.
  x_twi_putc(ESP8266_ADDRESS, ESP8266_CMD_READ_UPDATE, false);

  return false;
}