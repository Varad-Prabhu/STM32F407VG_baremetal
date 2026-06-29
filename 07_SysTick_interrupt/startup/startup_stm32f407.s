.syntax unified
.cpu cortex-m4
.thumb

.global Reset_Handler
.global _estack

.extern main
.extern SysTick_Handler

.extern _sidata
.extern _sdata
.extern _edata
.extern _sbss
.extern _ebss

.section .isr_vector,"a",%progbits
.align 2

.word _estack
.word Reset_Handler
.word Default_Handler
.word Default_Handler
.word Default_Handler
.word Default_Handler
.word Default_Handler
.word Default_Handler
.word Default_Handler
.word Default_Handler
.word Default_Handler
.word Default_Handler
.word Default_Handler
.word Default_Handler
.word Default_Handler
.word SysTick_Handler
.word Default_Handler
.word Default_Handler
.word Default_Handler
.word Default_Handler
.word Default_Handler
.word Default_Handler
.word Default_Handler

.text
.align 2

.thumb_func
Reset_Handler:

    ldr r0, =_sidata
    ldr r1, =_sdata
    ldr r2, =_edata

CopyData:

    cmp r1, r2
    bge ZeroBSS

    ldr r3, [r0], #4
    str r3, [r1], #4

    b CopyData

ZeroBSS:

    ldr r0, =_sbss
    ldr r1, =_ebss

    movs r2, #0

ClearLoop:

    cmp r0, r1
    bge CallMain

    str r2, [r0], #4

    b ClearLoop

CallMain:

    bl main

Hang:

    b Hang

.thumb_func
Default_Handler:

    b Default_Handler

