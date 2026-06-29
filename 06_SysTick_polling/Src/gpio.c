#include "gpio.h"

void GPIO_Init(const GPIO_InitTypeDef *GPIO_InitStruct)
{
    GPIO_TypeDef *GPIOx = GPIO_InitStruct->Port;

    if (GPIOx == GPIOA)
    {
        RCC->AHB1ENR |= (1U << RCC_AHB1ENR_GPIOAEN_Pos);
    }
    else if (GPIOx == GPIOB)
    {
        RCC->AHB1ENR |= (1U << RCC_AHB1ENR_GPIOBEN_Pos);
    }
    else if (GPIOx == GPIOC)
    {
        RCC->AHB1ENR |= (1U << RCC_AHB1ENR_GPIOCEN_Pos);
    }
    else if (GPIOx == GPIOD)
    {
        RCC->AHB1ENR |= (1U << RCC_AHB1ENR_GPIODEN_Pos);
    }
    else if (GPIOx == GPIOE)
    {
        RCC->AHB1ENR |= (1U << RCC_AHB1ENR_GPIOEEN_Pos);
    }

    for (uint32_t pin = 0U; pin < 16U; pin++)
    {
        if (GPIO_InitStruct->Pin & (1U << pin))
        {
            /* Configure GPIO mode */
            GPIOx->MODER &= ~(3U << (pin * 2U));
            GPIOx->MODER |= (GPIO_InitStruct->Mode << (pin * 2U));

            /* Configure pull-up/pull-down */
            GPIOx->PUPDR &= ~(3U << (pin * 2U));
            GPIOx->PUPDR |= (GPIO_InitStruct->Pull << (pin * 2U));

            /* Configure output type
               Applicable for Output and Alternate Function modes */
            if ((GPIO_InitStruct->Mode == GPIO_MODE_OUTPUT) ||
                (GPIO_InitStruct->Mode == GPIO_MODE_AF))
            {
                GPIOx->OTYPER &= ~(1U << pin);
                GPIOx->OTYPER |= (GPIO_InitStruct->OutputType << pin);
            }

            /* Configure output speed
               Applicable for Output and Alternate Function modes */
            if ((GPIO_InitStruct->Mode == GPIO_MODE_OUTPUT) ||
                (GPIO_InitStruct->Mode == GPIO_MODE_AF))
            {
                GPIOx->OSPEEDR &= ~(3U << (pin * 2U));
                GPIOx->OSPEEDR |= (GPIO_InitStruct->Speed << (pin * 2U));
            }
        }
    }
}

void GPIO_WritePin(GPIO_TypeDef *GPIOx, uint32_t Pin, uint8_t PinState)
{
    if (PinState == GPIO_PIN_SET)
    {
        GPIOx->BSRR = Pin;
    }
    else
    {
        GPIOx->BSRR = (Pin << 16U);
    }
}

void GPIO_TogglePin(GPIO_TypeDef *GPIOx, uint32_t Pin)
{
    GPIOx->ODR ^= Pin;
}

uint8_t GPIO_ReadPin(GPIO_TypeDef *GPIOx, uint32_t Pin)
{
    if (GPIOx->IDR & Pin)
    {
        return GPIO_PIN_SET;
    }

    return GPIO_PIN_RESET;
}

