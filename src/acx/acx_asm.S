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
    ; *(T_BUFF_START + (T_STACK_SIZE * T_ID) + 2) <- SP

    ; X <- T_BUFF_START
    CLI
    LDI r26, lo8(T_START)
    LDI	r27, hi8(T_START)
    SEI

    ; X <- X + 2
    ADIW X, 2

    ; X <- X + (T_STACK_SIZE * T_ID)
    IN r18, T_ID
    LDI r20, lo8(T_SIZE)
    LDI r21, hi8(T_SIZE)
1:  TST r18
    BREQ 2f
    ADD r26, r20
    ADC r27, r21
    DEC r18
    JMP 1b

    ; r21:r20 <- SP
2:  CLI
    IN r20, SPL
    IN r21, SPH
    SEI

    ; (X) <- SP
    ST X+, r20
    ST X, r21

    ; --- Part 2: Schedule the next thread ---
    .global	x_schedule
x_schedule:
    ; Z <- T_ID
    IN r30, T_ID
    LDI r31, 0x00

    ; Loop through all threads to find the next ready one.
    ; do {
    ;   r18 <- !(T_DISABLED_STATUS | T_DELAYED_STATUS)
    ;   r30 <- (r30 + 0x01) | 0b00000111
    ;   r19 <- 0b00000001 << r30
    ; } while (r18 != r19)
1:  IN r18, T_DISABLED_STATUS
    IN r19, T_DELAYED_STATUS
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

    ; T_ID <- r30
    OUT T_ID, r30

    ; X <- T_BUFF_START
    CLI
    LDI r26, lo8(T_START)
    LDI	r27, hi8(T_START)
    SEI

    ; X <- X + 2
    ADIW X, 2

    ; X <- X + (T_SIZE * T_ID)
    IN r18, T_ID
    LDI r20, lo8(T_SIZE)
    LDI r21, hi8(T_SIZE)
1:  TST r18
    BREQ 2f
    ADD r26, r20
    ADC r27, r21
    DEC r18
    JMP 1b

2:  ; r19:r18 <- x_stack_control[T_ID].sp
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