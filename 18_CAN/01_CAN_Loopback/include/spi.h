#ifndef SPI_H
#define SPI_H

#include "stm32f407.h"

/* SPI initialization */
void SPI1_Init(void);

/* SPI data transfer */
void SPI1_Transmit(uint8_t data);
uint8_t SPI1_Receive(void);
uint8_t SPI1_TransmitReceive(uint8_t data);

#endif
