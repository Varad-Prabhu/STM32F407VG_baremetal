#ifndef ADC_H
#define ADC_H

#include "stm32f407.h"

void ADC_Init(void);
void ADC_StartConversion(void);
uint16_t ADC_Read(void);

void ADC_IRQHandler(void);

#endif
