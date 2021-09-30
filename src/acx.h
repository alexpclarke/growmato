#ifndef ACX_H_
  #define ACX_H_

  #include "acx-srf_defs.h"

  /* ----- SYSTEM INFO ----- */

  #define CLOCK_HZ 16000000
  #define NUM_THREADS 8
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