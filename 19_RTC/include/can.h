#ifndef CAN_H
#define CAN_H

#include "stm32f407.h"

void CAN_Init(void);

void CAN_Transmit(uint32_t id, uint8_t *data, uint8_t length);

uint8_t CAN_Receive(uint32_t *id, uint8_t *data);

#endif
