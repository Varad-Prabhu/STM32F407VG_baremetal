#ifndef TIMER_H
#define TIMER_H

#include "stm32f407.h"

void TIM2_Init(uint32_t prescaler, uint32_t auto_reload);
void TIM2_Start(void);
void TIM2_Stop(void);
void TIM2_DelayMs(uint32_t delay_ms);

void TIM2_PWM_Init(uint32_t prescaler, uint32_t period, uint32_t duty_cycle);

#endif
