#ifndef SYSTICK_H
#define SYSTICK_H

#include "stm32f407.h"

/** SysTick Functions */
void SysTick_Init(uint32_t ticks);

void SysTick_Start(void);

void SysTick_Stop(void);

void SysTick_DelayMs(uint32_t delay);

#endif /* SYSTICK_H */