#ifndef EXTI_H
#define EXTI_H

#include "stm32f407.h"

void EXTI_EnableInterrupt(uint8_t line);
void EXTI_DisableInterrupt(uint8_t line);

void EXTI_EnableRisingTrigger(uint8_t line);
void EXTI_DisableRisingTrigger(uint8_t line);

void EXTI_EnableFallingTrigger(uint8_t line);
void EXTI_DisableFallingTrigger(uint8_t line);

void EXTI_ClearPending(uint8_t line);

void EXTI_ConfigGPIO(uint8_t line, uint8_t port);

uint8_t EXTI_IsPending(uint8_t line);

#endif