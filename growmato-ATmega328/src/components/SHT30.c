#include "SHT30.h"

void SHT30_thread() {
  SHT30_init();

  while (1) {
    if (SHT30_read()) {
      char buf[4];
      intToString((int)(myEnv.AirTempF), buf, 4);
      x_usart_puts(buf);
      x_usart_putc('\n');

      intToString((int)(myEnv.AirRH), buf, 4);
      x_usart_puts(buf);
      x_usart_putc('\n');
    }

    x_delay(60000);
  }
}

void SHT30_init() {
  myEnv.AirTempF = 0;
  myEnv.AirRH = 0;
}

bool SHT30_read() {
  uint8_t command[2] = {0x24, 0x0B};
  if (!x_twi_puts(SHT30_ADDRESS, command, 2, true)) {
    return false;
    // x_crash();
  }

  x_delay(6);

  uint8_t response[6];
  if (!x_twi_gets(SHT30_ADDRESS, response, 6, true)) {
    return false;
  }

  for (int i = 0; i < 6; i++) {
    char buf[16];
    intToString(response[i], buf, 16);
    if (!x_usart_puts(buf)) {
      return false;
    }
  }

  uint16_t response_temp = *((uint16_t*)(response));
  uint16_t response_rh = *((uint16_t*)(response + 3));

  if (SHT30_CRC8(response) == response[2]) {
    myEnv.AirTempF = -49 + 315 * ((float)response_temp / 0xFFFF);
  }

  if (SHT30_CRC8(response + 3) == response[5]) {
    myEnv.AirRH = 315 * ((float)response_rh / 0xFFFF);
  }

  return true;
}

uint8_t SHT30_CRC8(uint8_t vals[2]) {
  uint8_t generator = 0x31;
  uint8_t crc = 0xFF;

  for (int j = 2; j; --j){
    crc ^= *vals++;
    for (int i = 8; i; --i) {
      crc = crc & 0x80 ? (crc << 1) ^ generator : (crc << 1);
    }
  }
  return crc;
}
