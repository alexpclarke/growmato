#ifndef ACX_H_
  #define ACX_H_

  /* ----- SYSTEM INFO ----- */

  #define CLOCK_HZ 16000000
  #define NUM_THREADS 8
  #define T_STACK_SIZE 128

  /* ----- SRAM DEFS ----- */
  #define T_BUFF_START 0x0400
  #define TN_DELAY(n) _SFR_MEM16(T_BUFF_START + (T_STACK_SIZE * n))
  #define TN_SP(n) _SFR_MEM16(T_BUFF_START + (T_STACK_SIZE * n) + 2)
  #define TN_SPBASE(n) (uint8_t*)(T_BUFF_START + (T_STACK_SIZE * n) + (T_STACK_SIZE - 1))

  #define T_ID GPIOR0
  #define T_DISABLED_STATUS GPIOR1
  #define T_DELAYED_STATUS GPIOR2

  // Only include if being imported by a non-assembly file.
  #ifndef __ASSEMBLER__
    #include <avr/io.h>
    #include <avr/interrupt.h>
    #include <util/atomic.h>
    #include <stdlib.h>
  
    // Make sure bool is defined.
    #ifndef bool
      #include <stdbool.h>
    #endif

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