#ifndef ACX_H_
  #define ACX_H_

  // General system info.
  #define CLOCK_HZ 16000000L
  #define SRAM_SIZE 0x800
  #define SRAM_OFFSET 0x100
  #define EEPROM_SIZE 0x400

  // Thread info.
  #define T_MAX 8
  #define T_SIZE 0x80

  // Macros for getting getting individual stacks.
  #define T_START (SRAM_SIZE + SRAM_OFFSET - (T_MAX * T_SIZE))
  #define TN_DELAY(n) _SFR_MEM16(T_START + (T_SIZE * n))
  #define TN_SP(n) _SFR_MEM16(T_START + (T_SIZE * n) + 2)
  #define TN_SPBASE(n) (uint8_t*)(T_START + (T_SIZE * n) + (T_SIZE - 1))

  // Store current thread and status flags in the GPIO registers.
  #define T_ID GPIOR0
  #define T_DISABLED_STATUS GPIOR1
  #define T_DELAYED_STATUS GPIOR2

  // Only include if being imported by a non-assembly file.
  #ifndef __ASSEMBLER__
    // Includes.
    #include <avr/io.h>
    #include <avr/interrupt.h>
    #include <util/atomic.h>
    #include <stdlib.h>
    #include <stdbool.h>

    // Define the function/thread pointer.
    typedef void (*PTHREAD)(void);
    typedef union {
      PTHREAD pthread;
      uint8_t addr[2];
    } PTU;

    // ACX function prototypes
    void x_init(void);
    void x_init_system_timer(void);
    void x_new(uint8_t, PTHREAD, bool);
    void x_yield(void);
    void x_schedule(void);
    void x_delay(uint16_t);
    void x_disable(uint8_t);
    void x_enable(uint8_t);
  #endif
#endif