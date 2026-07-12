#include "stm32f407.h"
#include "gpio.h"
#include "systick.h"
#include "wwdg.h"

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

int main(void)
{
    /* System clock configuration */
    SystemClock_Config();

    SysTick_Init(168000);
    SysTick_Start();


    GPIO_InitTypeDef GPIO_LED;
 
    GPIO_LED.Port = GPIOD;
    GPIO_LED.Pin = GPIO_PIN_12 | GPIO_PIN_13;
    GPIO_LED.Mode = GPIO_MODE_OUTPUT;
    GPIO_LED.Pull = GPIO_NOPULL;
    GPIO_LED.Speed = GPIO_SPEED_MEDIUM;
    GPIO_LED.OutputType = GPIO_OTYPE_PP;

    GPIO_Init(&GPIO_LED);

    if (RCC->CSR & (1U << RCC_CSR_WWDGRSTF_Pos))
    {
        /* Previous reset was caused by the Window Watchdog (WWDG) */
        GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_SET);                    /* Turn on the LED to indicate that the previous reset was caused by the WWDG */
    }
    RCC->CSR |= (1U << RCC_CSR_RMVF_Pos);                                   /* Clear the reset flags */

    /* PCLK1 = 42 MHz */
    /* WWDG clock = PCLK1 / 4096 / Prescaler */
    /* Window = 0x50 */
    /* Counter = 0x7F */
    /* Refresh after the counter falls below 0x50 but before it decrements to 0x3F (reset occurs at 0x3F) */
    /* WWDG clock = 42,000,000 / 4096 / 8 */
    /* WWDG clock = 1281.7 Hz */
    /* One counter decrement = 1 / 1281.7 = 0.78 ms */
    /* Time to reach window = 47 x 0.780 = 36.7 ms */
    /* Refresh after ~40 ms */
    WWDG_Init(WWDG_PRESCALER_8, 0x50, 0x7F);

    while (1)
    {
        GPIO_TogglePin(GPIOD, GPIO_PIN_12);                                 /* Toggle the LED */
        /* Normal operation */
        SysTick_DelayMs(40);                                                /* Delay for 40 ms */
        
        /* Uncomment ONE of the following instead of the above delay to demonstrate WWDG behavior */

        // SysTick_DelayMs(20);                                             /* Early refresh -> reset */
        // SysTick_DelayMs(60);                                             /* Late refresh -> reset */

        /* Reload the watchdog counter back to its maximum value (0x7F) */
        WWDG_Refresh(0x7F);                                                 /* Refresh watchdog counter */
    }

    return 0;
}
