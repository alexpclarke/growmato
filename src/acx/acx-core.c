#include "acx-core.h"

// Initializes the ACX kernel.
void x_init(void) {
  // Clear interupts so the init doesnt get interupted.
  cli();

  // Initialize thread status variables.
  T_ID = 0x00;                      // Set thread 0 to be the current thread.
  T_DISABLED_STATUS = 0b11111110;   // Set all but thread 0 to dissabled.
  T_DELAYED_STATUS = 0b00000000;    // Set all threads to not delayed.
  for (int i = 0; i < T_MAX; i++) {
    TN_DELAY(i) = 0x0000;           // Reset each thread's delay counter.
  }

  // Initialize Stack Pointers.
  for (int i = 0; i < T_MAX; i++) {
    TN_SP(i) = (uint16_t)TN_SPBASE(i);
  }

  // Initialize System Timer.
  x_init_system_timer();

  // Move the system stack to the T0 stack.
  for (int i = 2; i > 0; i--) {
    uint8_t val = *((uint8_t*)SP + i);
    *((uint8_t*)TN_SP(0)) = val;
    TN_SP(0)--;
  }

  // Update SP to the T0 stack.
  SP = TN_SP(0);

  // Re-enable interrupts.
  sei();

  // Return to caller (now thread 0).
  return;
}

// Set up the system counter on Timer0 in CTC mode that produces an interupt at
// a 1mHz resolution.
void x_init_system_timer(void) {
  // Clear registers.
  TCCR0A = 0x00;
  TCCR0B = 0x00;
  TCNT0 = 0x00;

  // Set mode to CTC.
  TCCR0A |= (0b00000001 << WGM01);

  // Set OCR0A to a 1msec tick.
  // OCR0A = (uint8_t)((CLOCK_HZ / 64) / 1000);
  OCR0A = 250;

  // Enable OCIE0A, turning on the interupt.
  TIMSK0 |= (0b00000001 << OCIE0A);

  // Set prescalar to 64.
  TCCR0B |= (0b00000011 << CS00);

  // Finished setting up the system timer.
  return;
}

// Creates a new thread by associating a function pointer with a specified
// thread ID and stack.
void x_new(uint8_t tid, PTHREAD pthread, bool isEnabled) {
  if (tid >= T_MAX) return;

  // Cast the pthread to a PTU for easier access.
  PTU pt = {.pthread = pthread};

  // Reset the sp of the current stack back to its spBase, clearing its stack.
  TN_SP(tid) = (uint16_t)TN_SPBASE(tid);

  // Copy the function pointer pthread onto tid's stack.
  for (int i = 0; i < 2; i++) {
    *((uint8_t*)TN_SP(tid)) = pt.addr[i];
    TN_SP(tid)--;
  }

  // Leave room for the 18 callee-saved registers on the tid stack.
  TN_SP(tid) -= 18;

  // Set or clear the enabled bit for the thread.
  if (isEnabled) {
    T_DISABLED_STATUS &= ~(0x01 << tid);
  } else {
    T_DISABLED_STATUS |= 0x01 << tid;
  }

  // If the current thread was overwritten, go to the scheduler.
  if (tid == T_ID) {
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
      TN_DELAY(T_ID) = ticks;

      // Set the x_delay_status bit corresponding to the calling thread's ID.
      T_DELAYED_STATUS |= (0x01 << T_ID);
    }
  }

  // Calls x_yield to initiate thread rescheduling.
  x_yield();
}

// Set specified thread's DISABLE status bit.
void x_disable(uint8_t tid) {
  T_DISABLED_STATUS |= (0x01 << tid);
  return;
}

// Clears specified thread's DISABLE status bit.
void x_enable(uint8_t tid) {
  T_DISABLED_STATUS &= ~(0x01 << tid);
  return;
}

// This interrupt is triggered every 1 msec based on TIMER0 COMPARE MATCH.
ISR(TIMER0_COMPA_vect) {
  // For each delayed thread, decrement its counter and if the counter is at
  // 0, turn the delay status off.
  for (int i = 0; i < T_MAX; i++) {
    if (T_DELAYED_STATUS & (0x01 << i)) {
      TN_DELAY(i)--;
      if (TN_DELAY(i) == 0) {
        T_DELAYED_STATUS &= ~(0x01 << i);
      }
    }
  }

  // Done with ISR, continue whatever the system was doing before the interupt.
  return;
}
