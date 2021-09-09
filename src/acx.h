#ifndef ACX_H_
  #define ACX_H_

  /* ----- SYSTEM INFO ----- */

  #define CLOCK_HZ 16000000
  #define	STACK_CANARY 0xAA

  /* ----- THREAD INFO ----- */

  // Maximum number of threads. Must be a power of two.
  #define MAX_THREADS 4

  // Number of registers saved during context switch.
  #define T_CONTEXT_SIZE 18

  // The default size of a thread stack.
  #define T_DEFAUNT_STACK_SIZE 256

  // Size of each thread stack in case the default is not appropriate.
  #define T0_STACK_SIZE T_DEFAUNT_STACK_SIZE
  #define T1_STACK_SIZE T_DEFAUNT_STACK_SIZE
  #define T2_STACK_SIZE T_DEFAUNT_STACK_SIZE
  #define T3_STACK_SIZE T_DEFAUNT_STACK_SIZE

  // The total stack memory size.
  #define STACK_MEM_SIZE (T0_STACK_SIZE + T1_STACK_SIZE + T2_STACK_SIZE + \
  T3_STACK_SIZE)

  // Only include if being imported by a non-assembly file.
  #ifndef __ASSEMBLER__
    // Make sure bool is defined.
    #ifndef bool
      #include <stdbool.h>
    #endif

    // Make sure NULL is defined for pointers.
    #ifndef NULL
      #define NULL ((void*)0)
    #endif

    // Make sure byte is defined.
    #ifndef byte
      typedef uint8_t byte;
    #endif

    extern byte x_thread_id;
    #define x_getTID()	x_thread_id

    // typedef void(* PTHREAD)(int, char * );
    typedef void (*PTHREAD)(void);
    typedef union {
      PTHREAD pthread;
      byte addr[3];
    } PTU;

    //--------------------------------------------------------------------------
    // This type is used for entries in the stack control table
    //--------------------------------------------------------------------------
    typedef struct {
      byte* sp;
      byte* spBase;
      byte* spCanary;
    } STACK_CONTROL;

    // ACX Function prototypes
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
    void init_System_Timer(void);
  #endif
#endif