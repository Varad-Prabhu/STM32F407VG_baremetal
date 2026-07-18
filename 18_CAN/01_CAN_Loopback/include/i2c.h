#ifndef I2C_H
#define I2C_H

#include "stm32f407.h"

/* I2C peripheral clock frequency */
#define I2C1_PCLK1_FREQ    42000000U

/* I2C Speed */
#define I2C_STANDARD_MODE        100000U        /* 100 kHz */
#define I2C_FAST_MODE            400000U        /* 400 kHz */

/* I2C Direction */
#define I2C_WRITE    0U
#define I2C_READ     1U

/* Initialization */
void I2C1_Init(uint32_t speed);

/* Generate START and STOP conditions */
void I2C1_Start(void);
void I2C1_Stop(void);

/* Address phase */
void I2C1_SendAddress(uint8_t address, uint8_t direction);

/* Data transfer */
void I2C1_WriteByte(uint8_t data);
uint8_t I2C1_ReadByte_ACK(void);
uint8_t I2C1_ReadByte_NACK(void);

/* Helper functions */
void I2C1_Write(uint8_t slave_address, uint8_t register_address, uint8_t data);

uint8_t I2C1_Read(uint8_t slave_address, uint8_t register_address);

void I2C1_ReadBuffer(uint8_t slave_address, uint8_t register_address, uint8_t *buffer, uint16_t length);

void I2C1_WriteBuffer(uint8_t slave_address, uint8_t register_address, const uint8_t *buffer, uint16_t length);

#endif
