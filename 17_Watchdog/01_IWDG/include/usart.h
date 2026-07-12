#ifndef USART_H
#define USART_H

#include "stm32f407.h"

void USART2_Init(void);

void USART2_WriteChar(char ch);

void USART2_WriteString(const char *str);

char USART2_ReadChar(void);

void USART2_SendHex(uint8_t value);

#endif
