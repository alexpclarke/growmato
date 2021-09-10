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

    // Make sure byte is defined.
    #ifndef byte
      typedef uint8_t byte;
    #endif

    // Define the function/thread pointer.
    typedef void (*PTHREAD)(void);
    typedef union {
      PTHREAD pthread;
      byte addr[2];
    } PTU;

    // Structure for 
    typedef struct {
      byte* sp;
      byte* spBase;
      byte* spCanary;
    } STACK_CONTROL;

    // ACX function prototypes
    void x_init(void);
    void x_new(uint8_t, PTHREAD, bool);
    void x_yield(void);
    void x_schedule(void);
    void x_delay(uint16_t);
    void x_suspend(uint8_t);
    void x_resume(uint8_t);
    void x_disable(uint8_t);
    void x_enable(uint8_t);
    uint32_t x_gtime();
    void x_stack_overflow(int);

    // Helper functions.
    void init_System_Timer(void);
    uint8_t bit2mask8(uint8_t x);
  #endif
#endif