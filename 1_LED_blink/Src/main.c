#include "stm32f407.h"

void delay(void)
{
    for (volatile uint32_t i = 0; i < 1000000; i++);
}

int main(void)
{
    /* Enable clock for GPIOD */
    RCC->AHB1ENR |= (1U << RCC_AHB1ENR_GPIODEN_Pos);

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
        /* LED ON */
        GPIOD->BSRR = (1U << 12U);

        delay();

        /* LED OFF */
        GPIOD->BSRR = (1U << (12U + 16U));

        delay();
    }
    return 0;
}