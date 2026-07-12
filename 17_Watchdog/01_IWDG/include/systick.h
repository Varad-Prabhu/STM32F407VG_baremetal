#ifndef SYSTICK_H
#define SYSTICK_H

#include "stm32f407.h"

/** SysTick Base Address */
#define SYSTICK_BASE 0xE000E010U

/** SysTick Register Definitions */
typedef struct
{
    volatile uint32_t CTRL;       /* Offset: 0x00 */
    volatile uint32_t LOAD;       /* Offset: 0x04 */
    volatile uint32_t VAL;        /* Offset: 0x08 */
    volatile uint32_t CALIB;      /* Offset: 0x0C */
} SysTick_TypeDef;

/* SysTick Peripheral Pointer */
#define SYSTICK ((SysTick_TypeDef *)SYSTICK_BASE)

/** SysTick Control Register bit positions */
#define SYSTICK_CTRL_ENABLE_Pos         0U
#define SYSTICK_CTRL_TICKINT_Pos        1U
#define SYSTICK_CTRL_CLKSOURCE_Pos      2U
#define SYSTICK_CTRL_COUNTFLAG_Pos      16U

/** SysTick Functions */
void SysTick_Init(uint32_t ticks);

void SysTick_Start(void);

void SysTick_Stop(void);

uint32_t SysTick_GetTick(void);

void SysTick_Handler(void);

void SysTick_DelayMs(uint32_t delay_ms);

#endif /* SYSTICK_H */
