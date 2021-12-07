#include "LED.h"

volatile uint8_t LED_R;
volatile uint8_t LED_G;
volatile uint8_t LED_B;

void LED_init () {
  DDRB |= _BV(DDB0) | _BV(DDB1) | _BV(DDB2);
  LED_R = LED_OFF;
  LED_G = LED_OFF;
  LED_B = LED_OFF;

  LED_setR(LED_FLASH);
}

void LED_setR (uint8_t mode) {
  LED_R = mode;
}

void LED_setG (uint8_t mode) {
  LED_G = mode;
}

void LED_setB (uint8_t mode) {
  LED_B = mode;
}

void LED_thread() {
  LED_init();
  
  while (1) {
    if (LED_G == LED_ON) {
      PORTB |= _BV(DDB0);
    } else if (LED_G == LED_FLASH) {
      PORTB ^= _BV(DDB0);
    } else {
      PORTB &= ~_BV(DDB0);
    }

    if (LED_R == LED_ON) {
      PORTB |= _BV(DDB1);
    } else if (LED_R == LED_FLASH) {
      PORTB ^= _BV(DDB1);
    } else {
      PORTB &= ~_BV(DDB1);
    }

    if (LED_B == LED_ON) {
      PORTB |= _BV(DDB2);
    } else if (LED_B == LED_FLASH) {
      PORTB ^= _BV(DDB2);
    } else {
      PORTB &= ~_BV(DDB2);
    }

    x_delay(500);
  }
}