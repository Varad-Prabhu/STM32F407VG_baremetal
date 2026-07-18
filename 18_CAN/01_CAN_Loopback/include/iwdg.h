#ifndef IWDG_H
#define IWDG_H

#include "stm32f407.h"

void IWDG_Init(uint8_t prescaler, uint16_t reload);
void IWDG_Refresh(void);

#endif
