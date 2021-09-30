; Authors:  Alex Clarke
; Date:     2021-09-10

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

    ; Set the current thread's stack pointer to the system stack pointer.
    ; stack[x_thread_id].sp <- SP

    ; r19:r18 <- x_thread_id
    LDS r18, T_ID
    LDI	r19, 0x00

    ; r21:r20 <- SP
    CLI
    IN r20, SPL
    IN r21, SPH
    SEI

    ; X <- &stack[0]
    CLI
    LDI r26, lo8(stack)
    LDI	r27, hi8(stack)
    SEI

    ; X <- X + (4 * r19:r18)
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
    ; Z <- x_thread_id
    LDS r30, T_ID
    LDI r31, 0x00

    ; Loop through all threads to find the next ready one.
    ; do {
    ;   r18 <- !(x_disable_status | x_delay_status)
    ;   r30 <- (r30 + 0x01) | 0b00000111
    ;   r19 <- 0b00000001 << r30
    ; } while (r18 != r19)
1:  LDS r18, T_DISABLED_STATUS
    LDS r19, T_DELAYED_STATUS
    OR r18, r19
    LDI r19, 0b11111111
    EOR r18, r19
    INC r30
    ANDI r30, 0b00000111
    LDI r19, 0b00000001
    MOV r20, r30
2:  TST r20
    BREQ 3f
    ADD r19, r19
    DEC r20
    JMP 2b
3:  AND r19, r18
    BREQ 1b

    ; x_thread_id <- r30
    STS T_ID, r30

    ; SP = stack[x_thread_id].sp
    ; X <- stack
    CLI
    LDI r26, lo8(stack)
    LDI	r27, hi8(stack)
    SEI

    ; X <- X + (4 * x_thread_id)
    ADD r26, r30
    ADC r27, r31
    ADD r26, r30
    ADC r27, r31
    ADD r26, r30
    ADC r27, r31
    ADD r26, r30
    ADC r27, r31

    ; r19:r18 <- stack[x_thread_id].sp
    LD r18, X+
    LD r19, X

    ; SP <- r19:r18
    CLI
    OUT SPL, r18
    OUT SPH, r19
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