.syntax unified
.cpu cortex-m4
.thumb

.global Reset_Handler
.global _estack

.extern main
.extern SysTick_Handler
.extern ADC_IRQHandler

.extern _sidata
.extern _sdata
.extern _edata
.extern _sbss
.extern _ebss

.section .isr_vector,"a",%progbits
.align 2

.word _estack                /* Initial Stack Pointer */
.word Reset_Handler          /* Reset */
.word Default_Handler        /* NMI */
.word Default_Handler        /* Hard Fault */
.word Default_Handler        /* MemManage */
.word Default_Handler        /* BusFault */
.word Default_Handler        /* UsageFault */
.word 0                      /* Reserved */
.word 0                      /* Reserved */
.word 0                      /* Reserved */
.word 0                      /* Reserved */
.word Default_Handler        /* SVCall */
.word Default_Handler        /* Debug Monitor */
.word 0                      /* Reserved */
.word Default_Handler        /* PendSV */
.word SysTick_Handler        /* SysTick */
/* External Interrupts */
.word Default_Handler        /* WWDG */
.word Default_Handler        /* PVD */
.word Default_Handler        /* TAMP_STAMP */
.word Default_Handler        /* RTC_WKUP */
.word Default_Handler        /* FLASH */
.word Default_Handler        /* RCC */
.word Default_Handler        /* EXTI0 */
.word Default_Handler        /* EXTI1 */
.word Default_Handler        /* EXTI2 */
.word Default_Handler        /* EXTI3 */
.word Default_Handler        /* EXTI4 */
.word Default_Handler        /* DMA1_Stream0 */
.word Default_Handler        /* DMA1_Stream1 */
.word Default_Handler        /* DMA1_Stream2 */
.word Default_Handler        /* DMA1_Stream3 */
.word Default_Handler        /* DMA1_Stream4 */
.word Default_Handler        /* DMA1_Stream5 */
.word Default_Handler        /* DMA1_Stream6 */
.word ADC_IRQHandler         /* ADC */
.word Default_Handler        /* CAN1_TX */
.word Default_Handler        /* CAN1_RX0 */
.word Default_Handler        /* CAN1_RX1 */
.word Default_Handler        /* CAN1_SCE */
.word Default_Handler        /* EXTI9_5 */
.word Default_Handler        /* TIM1_BRK_TIM9 */
.word Default_Handler        /* TIM1_UP_TIM10 */
.word Default_Handler        /* TIM1_TRG_COM_TIM11 */
.word Default_Handler        /* TIM1_CC */
.word Default_Handler        /* TIM2 */
.word Default_Handler        /* TIM3 */
.word Default_Handler        /* TIM4 */
.word Default_Handler        /* I2C1_EV */
.word Default_Handler        /* I2C1_ER */
.word Default_Handler        /* I2C2_EV */
.word Default_Handler        /* I2C2_ER */
.word Default_Handler        /* SPI1 */
.word Default_Handler        /* SPI2 */
.word Default_Handler        /* USART1 */
.word Default_Handler        /* USART2 */

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

