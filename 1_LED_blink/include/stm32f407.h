#ifndef STM32F407_H
#define STM32F407_H

#include <stdint.h>

/* Memory Base Addresses */
#define FLASH_BASE        0x08000000U
#define SRAM_BASE         0x20000000U
#define PERIPH_BASE       0x40000000U

/* Bus Base Addresses */
#define APB1PERIPH_BASE    PERIPH_BASE
#define APB2PERIPH_BASE    (PERIPH_BASE + 0x00010000U)
#define AHB1PERIPH_BASE    (PERIPH_BASE + 0x00020000U)
#define AHB2PERIPH_BASE    (PERIPH_BASE + 0x10000000U)

/* AHB1 Peripheral Base Addresses */
#define GPIOA_BASE         (AHB1PERIPH_BASE + 0x0000U)
#define GPIOB_BASE         (AHB1PERIPH_BASE + 0x0400U)
#define GPIOC_BASE         (AHB1PERIPH_BASE + 0x0800U)
#define GPIOD_BASE         (AHB1PERIPH_BASE + 0x0C00U)
#define GPIOE_BASE         (AHB1PERIPH_BASE + 0x1000U)

#define RCC_BASE           (AHB1PERIPH_BASE + 0x3800U)

/* GPIO Register Definition Structure */

typedef struct
{
    volatile uint32_t MODER;      /* Offset: 0x00 */
    volatile uint32_t OTYPER;     /* Offset: 0x04 */
    volatile uint32_t OSPEEDR;    /* Offset: 0x08 */
    volatile uint32_t PUPDR;      /* Offset: 0x0C */
    volatile uint32_t IDR;        /* Offset: 0x10 */
    volatile uint32_t ODR;        /* Offset: 0x14 */
    volatile uint32_t BSRR;       /* Offset: 0x18 */
    volatile uint32_t LCKR;       /* Offset: 0x1C */
    volatile uint32_t AFRL;       /* Offset: 0x20 */
    volatile uint32_t AFRH;       /* Offset: 0x24 */
} GPIO_TypeDef;

/* Peripheral Pointers */
#define GPIOA ((GPIO_TypeDef *)GPIOA_BASE)
#define GPIOB ((GPIO_TypeDef *)GPIOB_BASE)
#define GPIOC ((GPIO_TypeDef *)GPIOC_BASE)
#define GPIOD ((GPIO_TypeDef *)GPIOD_BASE)
#define GPIOE ((GPIO_TypeDef *)GPIOE_BASE)

/* RCC structure */
typedef struct
{
    volatile uint32_t CR;          // 0x00
    volatile uint32_t PLLCFGR;     // 0x04
    volatile uint32_t CFGR;        // 0x08
    volatile uint32_t CIR;         // 0x0C
    volatile uint32_t AHB1RSTR;    // 0x10
    volatile uint32_t AHB2RSTR;    // 0x14
    volatile uint32_t AHB3RSTR;    // 0x18
    uint32_t RESERVED0;            // 0x1C
    volatile uint32_t APB1RSTR;    // 0x20
    volatile uint32_t APB2RSTR;    // 0x24
    uint32_t RESERVED1[2];         // 0x28, 0x2C
    volatile uint32_t AHB1ENR;     // 0x30
    volatile uint32_t AHB2ENR;     // 0x34
    volatile uint32_t AHB3ENR;     // 0x38
    uint32_t RESERVED2;            // 0x3C
    volatile uint32_t APB1ENR;     // 0x40
    volatile uint32_t APB2ENR;     // 0x44
} RCC_TypeDef;

/* RCC Pointer */
#define RCC ((RCC_TypeDef *)RCC_BASE)

/* RCC AHB1 Clock Enable Register Bits */
#define RCC_AHB1ENR_GPIOAEN_Pos    0U
#define RCC_AHB1ENR_GPIOBEN_Pos    1U
#define RCC_AHB1ENR_GPIOCEN_Pos    2U
#define RCC_AHB1ENR_GPIODEN_Pos    3U
#define RCC_AHB1ENR_GPIOEEN_Pos    4U

#endif
