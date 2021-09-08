// Author(s):	Alexander Clarke
// Date:    	2021-04-20

/* ----- Includes ----- */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include <stdlib.h>
#include "acx.h"

/* ----- Global Variables ----- */

// Exec State Variables.
byte x_thread_id;
byte x_thread_mask;
byte x_disable_status;
byte x_suspend_status;
byte x_delay_status;

uint16_t TN_STACK_SIZE[MAX_THREADS] = {
  T0_STACK_SIZE, T1_STACK_SIZE, T2_STACK_SIZE, T3_STACK_SIZE
};

// Stack Control.
STACK_CONTROL stack[MAX_THREADS];

// Stack Memory.
byte x_thread_stacks[STACK_MEM_SIZE];

// Thread Delay Counters.
volatile uint16_t x_thread_delay[MAX_THREADS];
volatile uint32_t x_system_counter;

/* ----- ACX Functions ----- */

// Initializes the ACX kernel.
void x_init(void) {
  // Clear interupts so the init doesnt get interupted.
  cli();

  // Initialize thread status variables.
  x_disable_status = 0b11111110;    // Set all but thread 0 to dissabled.
  x_suspend_status = 0b00000000;    // Set all threads to not suspended.
  x_delay_status = 0b00000000;      // Set all threads to not delayed.
  x_system_counter = 0x00000000;    // Reset the system counter.
  x_thread_id = 0x00;               // Set thread 0 to be the current thread.
  x_thread_mask = 0x01;             // Set the thred mask to 0.
  for (int i = 0; i < MAX_THREADS; i++)
    x_thread_delay[i] = 0x0000;     // Set each thread's delay to 0.

  // Initialize Stack Control.
  for (int i = 0; i < MAX_THREADS; i++) {
    // Get the address of the previous base.
    uint8_t* prevBase = i == 0 ? x_thread_stacks - 1 : stack[i - 1].spBase;

    // Set the base, sp and canary for each stack control block.
    stack[i].spBase = prevBase + T_DEFAUNT_STACK_SIZE;
    stack[i].sp = stack[i].spBase;
    stack[i].spCanary = prevBase + 1;
  }

  // Put our canary values at each stack boundary (low end) to allow detection
  // of stack overflow.
  for (int i = 0; i < MAX_THREADS; i++) {
    *(stack[i].spCanary) = STACK_CANARY;
  }

  // Initialize System Timer.
  init_System_Timer();

  // Move the system stack to the T0 stack.
  for (int i = 1; i >= 0; i--) {
    // Get the value from the system stack and put it into the T0 stack.
    byte val = *((byte*)SP + i);
    *(stack[0].sp) = val;

    // Update sp.
    stack[0].sp--;
  }

  // Update hardware SP to the T0 stack.
  SP = (int)stack[0].sp;

  // Enable interrupts.
  sei();

  // Return to caller (now thread 0).
  return;
}

// Creates a new thread by associating a function pointer with a specified
// thread ID and stack.
void x_new(uint8_t tid, PTHREAD pthread, bool isEnabled) {
  // Cast the pthread to a PTU for easier access.
  PTU pt = {.pthread = pthread};

  // Reset the sp of the current stack back to its spBase, clearing its stack.
  stack[tid].sp = stack[tid].spBase;

  // Copy the function pointer pthread onto tid's stack.
  for (int i = 0; i < 2; i++) {
    // Get the value from the pthread and put it into the tid stack.
    *(stack[tid].sp) = pt.addr[i];

    // Update sp.
    stack[tid].sp--;
  }

  // Leave room for the 18 callee-saved registers on the tid stack.
  stack[tid].sp -= 18;

  // Set or clear the enabled bit for the thread.
  if (isEnabled) {
    x_disable_status &= ~(0x01 << tid);
  } else {
    x_disable_status |= 0x01 << tid;
  }

  // If the current thread was overwritten, go to the scheduler.
  if (tid == x_thread_id) {
    x_schedule();
  } else {
    return;
  }
}

// Delays the calling thread by the specified number of system ticks.
void x_delay(uint16_t ticks) {
  // Ticks must be greater than 0.
  if (ticks > 0) {
    ATOMIC_BLOCK (ATOMIC_RESTORESTATE) {
      // Copy the delay value into the calling thread's delay counter.
      x_thread_delay[x_thread_id] = ticks;

      // Set the x_delay_status bit corresponding to the calling thread's ID.
      x_delay_status |= (0x01 << x_thread_id);
    }
  }

  // Calls x_yield to initiate thread rescheduling.
  x_yield();
}

// Set specified thread's suspend status bit.
void x_suspend(uint8_t tid) {
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
    x_suspend_status |= (0x01 << tid);
  }
}

// Clears specified thread's suspend status bit.
void x_resume(uint8_t tid) {
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
    x_suspend_status &= ~(0x01 << tid);
  }
}

// Set specified thread's DISABLE status bit.
void x_disable(uint8_t tid) {
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
    x_disable_status |= (0x01 << tid);
  }
}

// Clears specified thread's DISABLE status bit.
void x_enable(uint8_t tid) {
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
    x_disable_status &= ~(0x01 << tid);
  }
}

// Returns current value of the system tick counter.
unsigned long x_gtime(){
  uint32_t x_system_counter_;
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
    x_system_counter_ = x_system_counter;
  }
  return x_system_counter_;
}

/* -----  Inerupt Service Routines ----- */

// This interrupt is triggered every 1 msec based on TIMER0 COMPARE MATCH.
ISR(TIMER0_COMPA_vect) {
  // Increment the system clock.
  x_system_counter++;

  // For each delayed thread, decrement its counter and if the counter is at
  // 0, turn the delay status off.
  for (int i = 0; i < MAX_THREADS; i++) {
    if (x_delay_status & (0x01 << i)) {
      x_thread_delay[i]--;
      if (x_thread_delay[i] == 0) {
        x_delay_status &= ~(0x01 << i);
      }
    }
  }
}

/* ----- Helper Functions ----- */

// Set up the system counter on Timer0 in CTC mode that produces an interupt at
// a 1mHz resolution.
void init_System_Timer(void) {
  // Clear registers.
  TCCR0A = 0x00;
  TCCR0B = 0x00;
  TCNT0 = 0x00;

  // Set mode to CTC.
  TCCR0A |= (0b1 << WGM01);

  // Set OCR0A to a 1msec tick.
  // OCR0A = (uint8_t)((CLOCK_HZ / 64) / 1000);
  OCR0A = 250;

  // Enable OCIE0A, turning on the interupt.
  TIMSK0 |= (0b1 << OCIE0A);

  // Set prescalar to 64.
  TCCR0B |= (0b011 << CS00);

  // Successfully set the system timer.
  return;
}

// Entered when canary check indicates that a thread exceeded its stack
// allocation. Does not return. Flashes the on-board LED with SOS pattern.
void x_stack_overflow(int thread_id) {
  // DDRB |= 0b00100000;
  // while(1) {
    // PORTB ^= 0b00100000;
    // _delay_ms(100000);
  // }
}