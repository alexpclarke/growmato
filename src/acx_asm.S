#define __SFR_OFFSET 0
#include <avr/io.h>
#include "acx.h"

    .section .text
    .global x_yield
x_yield:
    ; --- Part 1: Save thread state ---

    ; Push values of the "callee-save" registers to the stack.
    PUSH r2
    PUSH r3
    PUSH r4
    PUSH r5
    PUSH r6
    PUSH r7
    PUSH r8
    PUSH r9
    PUSH r10
    PUSH r11
    PUSH r12
    PUSH r13
    PUSH r14
    PUSH r15
    PUSH r16
    PUSH r17
    PUSH r28
    PUSH r29

    ; Store the value of our thread ID for later use.
    ; r19:r18 <- x_thread_id
    LDS r18, x_thread_id
    LDI	r19, 0x00

    ; Check if the canary has been changed.

    ; r20 <- *(stack[x_thread_id].spCanary)
    ; or
    ; r20 <- *(&stack + (x_thread_id * 6) + 4)
    LDI r26, lo8(stack)
    LDI r27, hi8(stack)
    ADD r26, r18
    ADC r27, r19
    ADD r26, r18
    ADC r27, r19
    ADD r26, r18
    ADC r27, r19
    ADD r26, r18
    ADC r27, r19
    ADD r26, r18
    ADC r27, r19
    ADD r26, r18
    ADC r27, r19
    ADIW r26, 0x04
    LD r20, X+
    LD r27, X
    MOV r26, r20
    LD r20, X

    ; r21 <- STACK_CANARY
    LDI r21, STACK_CANARY

    ; If r20 != r21, the canary has been overwritten.
    SUB r21, r20
    BREQ 1f
    MOV r24, r18
    MOV r25, r19
    CALL x_stack_overflow

    ; Set the current thread's stack pointer to the system stack pointer.
    ; stack[x_thread_id].sp <- SP
    ; r21:r20 <- SP
1:  CLI
    IN r20, SPL
    IN r21, SPH
    SEI

    ; X <- stack
    LDI r26, lo8(stack)
    LDI	r27, hi8(stack)

    ; X <- X + (6 * r19:r18)
    ADD r26, r18
    ADC r27, r19
    ADD r26, r18
    ADC r27, r19
    ADD r26, r18
    ADC r27, r19
    ADD r26, r18
    ADC r27, r19
    ADD r26, r18
    ADC r27, r19
    ADD r26, r18
    ADC r27, r19

    ; (X) <- SP
    ST X+, r20
    ST X, r21


    ; --- Part 2: Schedule the next thread ---
    .global	x_schedule
x_schedule:
    ; r20 <- !(x_disable_status | x_suspend_status | x_delay_status)
    LDS r20, x_disable_status
    LDS r21, x_suspend_status
    OR r20, r21
    LDS r21, x_delay_status
    OR r20, r21
    COM r20

    ; r19:r18 <- x_thread_id
    LDS r18, x_thread_id
    LDI r19, 0x00

    ; Loop through all threads to find the next ready one.
    ; do {
    ;   r20 <- (r20 + 0x01) % MAX_THREADS
    ;   r21 <- 0x01 << r18
    ; } while (r21 & r20)
    LDI r21, MAX_THREADS
    DEC r21
1:  INC r18
    AND r18, r21
    MOV r23, r18
    LDI r22, 0x01
    RJMP 2f
3:  ADD r22, r22
2:  DEC r23
    BRPL 3b
    MOV r23, r22
    AND r23, r20
    BREQ 1b

    ; x_thread_id <- r18
    STS x_thread_id, r18

    ; x_thread_mask <- r22
    STS x_thread_mask, r22

    ; SP = stack[x_thread_id].sp
    ; X <- stack
    LDI r26, lo8(stack)
    LDI	r27, hi8(stack)

    ; X <- X + (6 * r19:r)
    ADD r26, r18
    ADC r27, r19
    ADD r26, r18
    ADC r27, r19
    ADD r26, r18
    ADC r27, r19
    ADD r26, r18
    ADC r27, r19
    ADD r26, r18
    ADC r27, r19
    ADD r26, r18
    ADC r27, r19

    ; r21:r20 <- stack[x_thread_id].sp
    LD r20, X+
    LD r21, X

    ; SP <- r21:r20
    CLI
    OUT SPL, r20
    OUT SPH, r21
    SEI

; --- Part 3: Restore context of next READY thread ---
restore:
    POP r29
    POP r28
    POP r17
    POP r16
    POP r15
    POP r14
    POP r13
    POP r12
    POP r11
    POP r10
    POP r9
    POP r8
    POP r7
    POP r6
    POP r5
    POP r4
    POP r3
    POP r2

    RET