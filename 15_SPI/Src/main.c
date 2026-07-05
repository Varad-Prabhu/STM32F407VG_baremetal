#include "stm32f407.h"
#include "gpio.h"
#include "systick.h"
#include "nvic.h"
#include "timer.h"
#include "spi.h"
#include "usart.h"
#include "w25qxx.h"

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
    uint8_t manufacturer;
    uint8_t memory_type;
    uint8_t capacity;

    /* System clock configuration */
    SystemClock_Config();

    /*----------------------------------------------------------*/
    /* Hardware Connections (W25Qxx SPI Flash Memory)           */
    /*                                                          */
    /* STM32F407VG                  W25Qxx                      */
    /*----------------------------------------------------------*/
    /* PA4  (GPIO Output) ---------> CS (/CS)                   */
    /* PA5  (SPI1_SCK, AF5) -------> CLK                        */
    /* PA6  (SPI1_MISO, AF5) <------ DO (MISO)                  */
    /* PA7  (SPI1_MOSI, AF5) -------> DI (MOSI)                 */
    /* 3.3V ------------------------> VCC                       */
    /* GND -------------------------> GND                       */
    /*----------------------------------------------------------*/
    GPIO_InitTypeDef GPIO_SPI = {0};

    GPIO_SPI.Port = GPIOA;
    GPIO_SPI.Pin = GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
    GPIO_SPI.Mode = GPIO_MODE_AF;
    GPIO_SPI.Pull = GPIO_NOPULL;
    GPIO_SPI.Speed = GPIO_SPEED_HIGH;
    GPIO_SPI.OutputType = GPIO_OTYPE_PP;
    GPIO_SPI.Alternate = GPIO_AF5_SPI1;

    GPIO_Init(&GPIO_SPI);

    GPIO_InitTypeDef GPIO_CS = {0};

    GPIO_CS.Port = GPIOA;
    GPIO_CS.Pin = GPIO_PIN_4;
    GPIO_CS.Mode = GPIO_MODE_OUTPUT;
    GPIO_CS.Pull = GPIO_NOPULL;
    GPIO_CS.Speed = GPIO_SPEED_HIGH;
    GPIO_CS.OutputType = GPIO_OTYPE_PP;

    GPIO_Init(&GPIO_CS);

    /*----------------------------------------------------------*/
    /* Hardware Connections (USART2)                            */
    /*                                                          */
    /* STM32F407VG                USB-to-UART Module            */
    /* ---------------------------------------------------------*/
    /* PA2 (USART2_TX, AF7) ----> RXD                           */
    /* PA3 (USART2_RX, AF7) <---- TXD                           */
    /* GND ---------------------> GND                           */
    /*                                                          */
    /* USB-to-UART connected to PC                              */
    /* Baud Rate : 115200                                       */
    /*----------------------------------------------------------*/
    USART2_Init();                          /* Initialize USART2 for debugging */
    SPI1_CS_Deselect();                     /* W25Qxx deselected */
    SPI1_Init();                            /* Initialize SPI1 */

    W25Q_ReadJEDECID(&manufacturer, &memory_type, &capacity);
    
    USART2_WriteString("W25Qxx JEDEC ID:\r\n");
    USART2_WriteString("Manufacturer ID: 0x");
    USART2_SendHex(manufacturer);

    USART2_WriteString("\r\nMemory Type: 0x");
    USART2_SendHex(memory_type);

    USART2_WriteString("\r\nCapacity: 0x");
    USART2_SendHex(capacity);

    USART2_WriteString("\r\n");

    while (1)
    {
        /* Main loop */
    }

    return 0;
}
