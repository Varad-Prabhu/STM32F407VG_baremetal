#include "stm32f407.h"
#include "gpio.h"

void SystemClock_Config(void)
{
    /* Configure Flash wait states */
    FLASH->ACR &= ~(0xFU << 0U);                    /* Clear latency bits */
    FLASH->ACR |= (5U << 0U);                       /* 5 wait states for 168 MHz */

    while ((FLASH->ACR & 0xFU) != 5U);              /* Wait for latency to be configured*/

    FLASH->ACR |= (1U << 8U);                       /* Enable prefetch */

    FLASH->ACR |= (1U << 9U);                       /* Enable instruction cache */

    FLASH->ACR |= (1U << 10U);                      /* Enable data cache */

    /* Disable PLL */
    RCC->CR &= ~(1U << 24U);

    /* Wait until PLL is fully disabled */
    while (RCC->CR & (1U << 25U));

    /* Configure PLL */
    RCC->PLLCFGR &= ~(0x3FU << 0U);                 /* Clear PLLM */
    RCC->PLLCFGR |= (16U << 0U);                    /* PLLM = 16, PLL Input = HSI / PLLM, PLL Input = 16 MHz / 16 = 1 MHz */
    RCC->PLLCFGR &= ~(0x1FFU << 6U);                /* Clear PLLN */
    RCC->PLLCFGR |= (336U << 6U);                   /* PLLN = 336, VCO Output = PLL Input * PLLN = 1 MHz * 336 = 336 MHz */
    RCC->PLLCFGR &= ~(0x3U << 16U);                 /* Clear PLLP */
    RCC->PLLCFGR |= (0U << 16U);                    /* PLLP = 2, SYSCLK = VCO Output / PLLP = 336 MHz / 2 = 168 MHz */
    RCC->PLLCFGR &= ~(0xFU << 24U);                 /* Clear PLLQ */
    RCC->PLLCFGR |= (7U << 24U);                    /* PLLQ = 7, USB OTG FS, SDIO and RNG Clock = VCO Output / PLLQ = 336 MHz / 7 = 48 MHz */  

    RCC->PLLCFGR &= ~(1U << 22U);                   /* Clear PLLSRC */
    RCC->PLLCFGR |= (0U << 22U);                    /* PLL source = HSI */

    /* Enable PLL */
    RCC->CR |= (1U << 24U);                         /* Main PLL enable */

    /* Wait until PLL is ready */
    while (!(RCC->CR & (1U << 25U)));               /* Wait until PLL is locked or ready */

    /* Configure AHB prescaler */
    RCC->CFGR &= ~(0xFU << 4U);                     /* Clear AHB prescaler bits */
    RCC->CFGR |= (0U << 4U);                        /* AHB prescaler = 1, HCLK = SYSCLK / 1 = 168 MHz */

    /* Configure APB1 prescaler */
    RCC->CFGR &= ~(0x7U << 10U);                    /* Clear APB1 prescaler bits */
    RCC->CFGR |= (5U << 10U);                       /* APB1 prescaler = 4, PCLK1 = HCLK / 4 = 42 MHz */

    /* Configure APB2 prescaler */
    RCC->CFGR &= ~(0x7U << 13U);                    /* Clear APB2 prescaler bits */
    RCC->CFGR |= (4U << 13U);                       /* APB2 prescaler = 2, PCLK2 = HCLK / 2 = 84 MHz */

    /* Select PLL as system clock */
    RCC->CFGR &= ~(0x3U << 0U);                     /* Clear SW bits */
    RCC->CFGR |= (2U << 0U);                        /* System clock switch = PLL */

    /* Wait until PLL becomes system clock */
    while (((RCC->CFGR >> 2U) & 0x03U) != 0x02U);   /* Wait until PLL is used as system clock */
}

void EXTI0_IRQHandler(void)
{
    /* Check if pending bit for EXTI0 is set */
    if (EXTI->PR & (1U << 0U))
    {
        /* Clear the pending bit */
        EXTI->PR = (1U << 0U);

        /* Toggle LED*/
        GPIO_TogglePin(GPIOD, GPIO_PIN_12);
    }
}

int main(void)
{
    /* System clock configuration */
    SystemClock_Config();

    /* Enable clock for SYSCFG */
    RCC->APB2ENR |= (1U << RCC_APB2ENR_SYSCFGEN_Pos);
    
    GPIO_InitTypeDef GPIO_LED;
    GPIO_InitTypeDef GPIO_BUTTON;

    GPIO_LED.Port = GPIOD;
    GPIO_LED.Pin = GPIO_PIN_12;
    GPIO_LED.Mode = GPIO_MODE_OUTPUT;
    GPIO_LED.Pull = GPIO_NOPULL;
    GPIO_LED.Speed = GPIO_SPEED_MEDIUM;
    GPIO_LED.OutputType = GPIO_OTYPE_PP;

    GPIO_Init(&GPIO_LED);

    GPIO_BUTTON.Port = GPIOA;
    GPIO_BUTTON.Pin = GPIO_PIN_0;
    GPIO_BUTTON.Mode = GPIO_MODE_INPUT;
    GPIO_BUTTON.Pull = GPIO_PULLDOWN;
    GPIO_BUTTON.Speed = GPIO_SPEED_LOW;
    GPIO_BUTTON.OutputType = GPIO_OTYPE_PP;

    GPIO_Init(&GPIO_BUTTON);

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