// Authors:   Alex Clarke
// Date:      2021-09-10

#ifndef ACX_H_
  #define ACX_H_

  /* ----- SYSTEM INFO ----- */

  #define CLOCK_HZ 16000000

  // Memory locations.
  #define T_STACK_BUFF _SFR_MEM8(0x0100)
  #define T_DELAY_BUFF _SFR_MEM16(0x0500)
  #define T_DISABLED_STATUS _SFR_MEM8(0x0510)
  #define T_DELAYED_STATUS _SFR_MEM8(0x0511)
  #define T_ID _SFR_MEM8(0x0512)

  /* ----- THREAD INFO ----- */

  // Maximum number of threads. Must be a power of two.
  #define NUM_THREADS 8

  // The default size of a thread stack.
  #define T_STACK_SIZE 128

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

    // Structure for 
    typedef struct {
      uint8_t* sp;
      uint8_t* spBase;
    } STACK_CONTROL;

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