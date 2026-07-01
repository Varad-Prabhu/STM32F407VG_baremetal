#include "exti.h"

void EXTI_EnableInterrupt(uint8_t line)
{
    EXTI->IMR |= (1U << line);
}

void EXTI_DisableInterrupt(uint8_t line)
{
    EXTI->IMR &= ~(1U << line);
}

void EXTI_EnableRisingTrigger(uint8_t line)
{
    EXTI->RTSR |= (1U << line);
}

void EXTI_DisableRisingTrigger(uint8_t line)
{
    EXTI->RTSR &= ~(1U << line);
}

void EXTI_EnableFallingTrigger(uint8_t line)
{
    EXTI->FTSR |= (1U << line);
}

void EXTI_DisableFallingTrigger(uint8_t line)
{
    EXTI->FTSR &= ~(1U << line);
}

void EXTI_ClearPending(uint8_t line)
{
    EXTI->PR = (1U << line);
}

void EXTI_ConfigGPIO(uint8_t line, uint8_t port)
{
    uint32_t registerIndex = line / 4U;
    uint32_t bitPosition = (line % 4U) * 4U;

    RCC->APB2ENR |= (1U << RCC_APB2ENR_SYSCFGEN_Pos);               /* Enable SYSCFG clock */

    SYSCFG->EXTICR[registerIndex] &= ~(0xFU << bitPosition);        /* Clear existing port selection */

    SYSCFG->EXTICR[registerIndex] |= (port << bitPosition);         /* Configure GPIO port for EXTI line */
}

uint8_t EXTI_IsPending(uint8_t line)
{
    return ((EXTI->PR & (1U << line)) != 0U);
}