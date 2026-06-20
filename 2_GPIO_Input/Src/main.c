#include "stm32f407.h"

int main(void)
{
    /* Enable clock for GPIOA */
    RCC->AHB1ENR |= (1U << RCC_AHB1ENR_GPIOAEN_Pos);

    /* Enable clock for GPIOD */
    RCC->AHB1ENR |= (1U << RCC_AHB1ENR_GPIODEN_Pos);

    /* Configure PA0 as input */
    GPIOA->MODER &= ~(3U << (0U * 2U));
    GPIOA->MODER |=  (0U << (0U * 2U));

    /* Configure PA0 with pull-down resistor */
    GPIOA->PUPDR &= ~(3U << (0U * 2U));
    GPIOA->PUPDR |=  (2U << (0U * 2U));

    /* Configure PD12 as output */
    GPIOD->MODER &= ~(3U << (12U * 2U));
    GPIOD->MODER |=  (1U << (12U * 2U));

    /* Configure PD12 as Push-Pull */
    GPIOD->OTYPER &= ~(1U << 12U);

    /* Configure PD12 output speed as Medium */
    GPIOD->OSPEEDR &= ~(3U << (12U * 2U));
    GPIOD->OSPEEDR |=  (1U << (12U * 2U));

    while (1)
    {
        if ((GPIOA->IDR & (1U << 0U)) != 0U)
        {
            /* LED ON */
            GPIOD->BSRR = (1U << 12U);
        }
        else
        {
            /* LED OFF */
            GPIOD->BSRR = (1U << (12U + 16U));
        }
    }
    return 0;
}