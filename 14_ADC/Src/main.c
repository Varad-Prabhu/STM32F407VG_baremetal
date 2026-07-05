#include "stm32f407.h"
#include "gpio.h"
#include "systick.h"
#include "nvic.h"
#include "timer.h"
#include "adc.h"

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

    /*----------------------------------------------------------*/
    /* Hardware Connections (ADC)                               */
    /*----------------------------------------------------------*/
    /* PA1 --------------------> Potentiometer Wiper            */
    /* Potentiometer End 1 ----> 3.3V                           */
    /* Potentiometer End 2 ----> GND                            */
    /* ADC1 Channel 1                                           */
    /*----------------------------------------------------------*/
    GPIO_InitTypeDef GPIO_ADC = {0};

    GPIO_ADC.Port = GPIOA;
    GPIO_ADC.Pin = GPIO_PIN_1;
    GPIO_ADC.Mode = GPIO_MODE_ANALOG;
    GPIO_ADC.Pull = GPIO_NOPULL;
    
    GPIO_Init(&GPIO_ADC);

    /*----------------------------------------------------------*/
    /* Hardware Connections (PWM)                               */
    /*                                                          */
    /* STM32F407VG                                              */
    /* ---------------------------------------------------------*/
    /* PA0 (TIM2_CH1, AF1) ----> 220 Ω Resistor ----> LED Anode */
    /* LED Cathode -------------> GND                           */
    /*                                                          */
    /* PWM Output:                                              */
    /* Timer : TIM2 Channel 1                                   */
    /* GPIO  : PA0 (Alternate Function AF1)                     */
    /*----------------------------------------------------------*/
    GPIO_InitTypeDef GPIO_PWM = {0};

    GPIO_PWM.Port = GPIOA;
    GPIO_PWM.Pin = GPIO_PIN_0;
    GPIO_PWM.Mode = GPIO_MODE_AF;
    GPIO_PWM.Pull = GPIO_NOPULL;
    GPIO_PWM.Speed = GPIO_SPEED_HIGH;
    GPIO_PWM.OutputType = GPIO_OTYPE_PP;
    GPIO_PWM.Alternate = GPIO_AF1_TIM1_TIM2;

    GPIO_Init(&GPIO_PWM);

    ADC_Init();                         /* Initialize ADC1 */
    NVIC_EnableIRQ(ADC_IRQn);           /* Enable ADC interrupt */

    /* Timer clock = 84 MHz */
    /* Timer tick = 84 MHz / (83 + 1) = 1 MHz */
    /* PWM frequency = 1 MHz / (999 + 1) = 1 kHz */
    /* CCR1 = 100 -> Duty cycle ≈ 10% */
    /* Initial duty cycle = 10%, duty cycle updated continuously by ADC interrupt */
    TIM2_PWM_Init(83, 999, 100);            /* Prescaler = 83, Auto-reload = 999, Duty cycle = 10% */

    ADC_StartConversion();                  /* Start ADC conversion */

    while (1)
    {
        /* ADC interrupt continuously updates PWM duty cycle */
    }

    return 0;
}
