#ifndef NVIC_H
#define NVIC_H

#include "stm32f407.h"

void NVIC_EnableIRQ(uint8_t IRQNumber);
void NVIC_DisableIRQ(uint8_t IRQNumber);
void NVIC_SetPriority(uint8_t IRQNumber, uint8_t Priority);

#endif