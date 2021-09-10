// Authors:   Alex Clarke
// Date:      2021-09-09

#ifndef ACX_H_
  #define ACX_H_

  /* ----- SYSTEM INFO ----- */

  #define CLOCK_HZ 16000000
  #define	STACK_CANARY 0xAA

  /* ----- THREAD INFO ----- */

  // Maximum number of threads. Must be a power of two.
  #define MAX_THREADS 4
  // The default size of a thread stack.
  #define T_DEFAUNT_STACK_SIZE 256

  // Only include if being imported by a non-assembly file.
  #ifndef __ASSEMBLER__
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
    void x_new(uint8_t, PTHREAD, bool);
    void x_yield(void);
    void x_schedule(void);
    void x_delay(uint16_t);
    void x_disable(uint8_t);
    void x_enable(uint8_t);
    uint32_t x_gtime();

    // Helper functions.
    void init_System_Timer(void);
  #endif
#endif