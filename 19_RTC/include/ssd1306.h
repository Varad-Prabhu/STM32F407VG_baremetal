#ifndef SSD1306_H
#define SSD1306_H

#include "stm32f407.h"
#include "i2c.h"

/* SSD1306 I2C Address (7-bit) */
#define SSD1306_I2C_ADDRESS      0x3CU

/* SSD1306 control bytes */
#define SSD1306_COMMAND          0x00U
#define SSD1306_DATA             0x40U

/* Initialization */
void SSD1306_Init(void);

/* Command/Data transfer */
void SSD1306_WriteCommand(uint8_t cmd);
void SSD1306_WriteData(uint8_t data);
void SSD1306_WriteBuffer(const uint8_t *buffer, uint16_t length);

/* Display operations */
void SSD1306_Clear(void);
void SSD1306_SetCursor(uint8_t page, uint8_t column);
void SSD1306_WriteChar(char ch);
void SSD1306_WriteString(const char *str);

#endif
