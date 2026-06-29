#include "stm32f407.h"

void EXTI0_IRQHandler(void)
{
    /* Check if pending bit for EXTI0 is set */
    if (EXTI->PR & (1U << 0U))
    {
        /* Clear the pending bit */
        EXTI->PR = (1U << 0U);

        /* Toggle LED*/
        GPIOD->ODR ^= (1U << 12U);
    }
}

int main(void)
{
    /* Enable clock for GPIOA */
    RCC->AHB1ENR |= (1U << RCC_AHB1ENR_GPIOAEN_Pos);

    /* Enable clock for GPIOD */
    RCC->AHB1ENR |= (1U << RCC_AHB1ENR_GPIODEN_Pos);

    /* Enable clock for SYSCFG */
    RCC->APB2ENR |= (1U << RCC_APB2ENR_SYSCFGEN_Pos);
    
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

    /* Configure EXTI line 0 to be connected to GPIOA */
    SYSCFG->EXTICR[0] &= ~(0x0FU << (0U * 4U));
    SYSCFG->EXTICR[0] |=  (0x00U << (0U * 4U));

    /* Enable interrupt for EXTI line 0 */
    EXTI->IMR |= (1U << 0U);

    /* Enable rising trigger for EXTI line 0 */
    EXTI->RTSR |= (1U << 0U);

    /* Disable falling trigger for EXTI line 0 */
    EXTI->FTSR &= ~(1U << 0U);

    /* Enable EXTI0 interrupt in NVIC */
    NVIC->ISER[0] |= (1U << EXTI0_IRQn);

    while (1)
    {
        /* LED toggle is done in ISR */
    }
    return 0;
}