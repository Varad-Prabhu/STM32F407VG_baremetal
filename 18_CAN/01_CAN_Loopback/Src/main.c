#include "stm32f407.h"
#include "gpio.h"
#include "systick.h"
#include "can.h"

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
    uint32_t id;
    uint8_t rxData[8];
    uint8_t txData[8] =
    {
        0x11,
        0x22,
        0x33,
        0x44,
        0x55,
        0x66,
        0x77,
        0x88
    };
    uint8_t length;
    uint8_t i;
    uint8_t match;

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

    /*----------------------------------------------------------*/
    /* CAN Loopback Configuration                               */
    /*----------------------------------------------------------*/
    /* PB9 -> CAN1_TX (AF9)                                     */
    /* PB8 -> CAN1_RX (AF9)                                     */
    /*                                                          */
    /* Internal Loopback mode is enabled.                       */
    /* - Frames transmitted by CAN1 are internally received.    */
    /* - External CAN hardware is not required for this example.*/
    /*----------------------------------------------------------*/
    /*----------------------------------------------------------*/
    /* Hardware Connections (SN65HVD230 CAN Transceiver)        */
    /* (Required only for Normal Mode, not Loopback Mode)       */
    /*                                                          */
    /* STM32F407VG               SN65HVD230                     */
    /*----------------------------------------------------------*/
    /* PB9 (CAN1_TX, AF9) ------> CTX                           */
    /* PB8 (CAN1_RX, AF9) <------ CRX                           */
    /* 3.3V ---------------------> 3V3                          */
    /* GND ----------------------> GND                          */
    /* CANH ---------------------- CANH (CAN Bus High)          */
    /* CANL ---------------------- CANL (CAN Bus Low)           */
    /*----------------------------------------------------------*/
    /* - CTX = CAN Transmit input to transceiver.               */
    /* - CRX = CAN Receive output from transceiver.             */
    /* - CANH/CANL connect to the CAN bus.                      */
    /* - 120 Ω termination resistor at each end of the CAN bus. */
    /* - Another CAN node is required for communication.        */
    /*----------------------------------------------------------*/
    CAN_Init();                                 /* Initialize CAN */

    while (1)
    {
        CAN_Transmit(0x123, txData, sizeof(txData));
        length = CAN_Receive(&id, rxData);
        if (length == sizeof(txData))
        {
            if (id == 0x123U)
            {
                GPIO_TogglePin(GPIOD, GPIO_PIN_12);     /* ID matched */

                match = 1U;

                for (i = 0U; i < length; i++)
                {
                    if (rxData[i] != txData[i])
                    {
                        match = 0U;
                        break;
                    }
                }

                if (match)
                {
                    GPIO_TogglePin(GPIOD, GPIO_PIN_13); /* Data matched */
                }
            }
        }
        SysTick_DelayMs(1000);
    }

    return 0;
}
