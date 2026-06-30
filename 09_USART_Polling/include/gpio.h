#ifndef GPIO_H
#define GPIO_H

#include "stm32f407.h"

/* GPIO Pin */
#define GPIO_PIN_0                  (1U << 0U)
#define GPIO_PIN_1                  (1U << 1U)
#define GPIO_PIN_2                  (1U << 2U)
#define GPIO_PIN_3                  (1U << 3U)
#define GPIO_PIN_4                  (1U << 4U)
#define GPIO_PIN_5                  (1U << 5U)
#define GPIO_PIN_6                  (1U << 6U)
#define GPIO_PIN_7                  (1U << 7U)
#define GPIO_PIN_8                  (1U << 8U)
#define GPIO_PIN_9                  (1U << 9U)
#define GPIO_PIN_10                 (1U << 10U)
#define GPIO_PIN_11                 (1U << 11U)
#define GPIO_PIN_12                 (1U << 12U)
#define GPIO_PIN_13                 (1U << 13U)
#define GPIO_PIN_14                 (1U << 14U)
#define GPIO_PIN_15                 (1U << 15U)  

/* GPIO Pin State */
#define GPIO_PIN_SET                1U
#define GPIO_PIN_RESET              0U

/* GPIO Mode */
#define GPIO_MODE_INPUT             0U
#define GPIO_MODE_OUTPUT            1U
#define GPIO_MODE_AF                2U
#define GPIO_MODE_ANALOG            3U

/* GPIO Output Type */
#define GPIO_OTYPE_PP               0U
#define GPIO_OTYPE_OD               1U

/* GPIO pull up/down */
#define GPIO_NOPULL                 0U
#define GPIO_PULLUP                 1U
#define GPIO_PULLDOWN               2U

/* GPIO Output Speed */
#define GPIO_SPEED_LOW              0U
#define GPIO_SPEED_MEDIUM           1U
#define GPIO_SPEED_FAST             2U
#define GPIO_SPEED_HIGH             3U

/* GPIO Initialization Type */
typedef struct
{
    GPIO_TypeDef *Port;
    uint32_t Pin;
    uint32_t Mode;
    uint32_t Pull;
    uint32_t Speed;
    uint32_t OutputType;
    uint32_t Alternate;
} GPIO_InitTypeDef;

/* GPIO Functions */
void GPIO_Init(const GPIO_InitTypeDef *GPIO_InitStruct);
void GPIO_WritePin(GPIO_TypeDef *GPIOx, uint32_t Pin, uint8_t PinState);
void GPIO_TogglePin(GPIO_TypeDef *GPIOx, uint32_t Pin);
uint8_t GPIO_ReadPin(GPIO_TypeDef *GPIOx, uint32_t Pin);

#endif