#ifndef STM32F407_H
#define STM32F407_H

#include <stdint.h>

/* Memory Base Addresses */
#define FLASH_BASE        (0x08000000U)
#define SRAM_BASE         (0x20000000U)
#define PERIPH_BASE       (0x40000000U)

/* Bus Base Addresses */
#define APB1PERIPH_BASE    (PERIPH_BASE)
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

/* APB2 Peripheral Base Addresses */
#define SYSCFG_BASE        (APB2PERIPH_BASE + 0x3800U)
#define EXTI_BASE          (APB2PERIPH_BASE + 0x3C00U)

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
    volatile uint32_t CR;          /* Offset: 0x00 */
    volatile uint32_t PLLCFGR;     /* Offset: 0x04 */
    volatile uint32_t CFGR;        /* Offset: 0x08 */
    volatile uint32_t CIR;         /* Offset: 0x0C */
    volatile uint32_t AHB1RSTR;    /* Offset: 0x10 */
    volatile uint32_t AHB2RSTR;    /* Offset: 0x14 */
    volatile uint32_t AHB3RSTR;    /* Offset: 0x18 */
    uint32_t RESERVED0;            /* Offset: 0x1C */
    volatile uint32_t APB1RSTR;    /* Offset: 0x20 */
    volatile uint32_t APB2RSTR;    /* Offset: 0x24 */
    uint32_t RESERVED1[2];         /* Offset: 0x28, 0x2C */
    volatile uint32_t AHB1ENR;     /* Offset: 0x30 */
    volatile uint32_t AHB2ENR;     /* Offset: 0x34 */
    volatile uint32_t AHB3ENR;     /* Offset: 0x38 */
    uint32_t RESERVED2;            /* Offset: 0x3C */
    volatile uint32_t APB1ENR;     /* Offset: 0x40 */
    volatile uint32_t APB2ENR;     /* Offset: 0x44 */
    uint32_t RESERVED3[2];         /* Offset: 0x48, 0x4C */
    volatile uint32_t AHB1LPENR;   /* Offset: 0x50 */
    volatile uint32_t AHB2LPENR;   /* Offset: 0x54 */
    volatile uint32_t AHB3LPENR;   /* Offset: 0x58 */
    uint32_t RESERVED4;            /* Offset: 0x5C */
    volatile uint32_t APB1LPENR;   /* Offset: 0x60 */
    volatile uint32_t APB2LPENR;   /* Offset: 0x64 */
    uint32_t RESERVED5[2];         /* Offset: 0x68, 0x6C */
    volatile uint32_t BDCR;        /* Offset: 0x70 */
    volatile uint32_t CSR;         /* Offset: 0x74 */
    uint32_t RESERVED6[2];         /* Offset: 0x78, 0x7C */
    volatile uint32_t SSCGR;       /* Offset: 0x80 */
    volatile uint32_t PLLI2SCFGR;  /* Offset: 0x84 */
} RCC_TypeDef;

/* RCC Pointer */
#define RCC ((RCC_TypeDef *)RCC_BASE)

/* RCC AHB1 Clock Enable Register Bits */
#define RCC_AHB1ENR_GPIOAEN_Pos    0U
#define RCC_AHB1ENR_GPIOBEN_Pos    1U
#define RCC_AHB1ENR_GPIOCEN_Pos    2U
#define RCC_AHB1ENR_GPIODEN_Pos    3U
#define RCC_AHB1ENR_GPIOEEN_Pos    4U

/* RCC APB2 Clock Enable Register Bits */
#define RCC_APB2ENR_SYSCFGEN_Pos  14U

/* RCC USART2 Clock Enable Register Bits */
#define RCC_APB1ENR_USART2EN_Pos  17U

/* SYSCFG Register Definition Structure */
typedef struct
{
    volatile uint32_t MEMRMP;       /* Offset: 0x00 */
    volatile uint32_t PMC;          /* Offset: 0x04 */
    volatile uint32_t EXTICR[4];    /* Offset: 0x08, 0x0C, 0x10, 0x14 */
    uint32_t RESERVED[2];           /* Offset: 0x18, 0x1C */
    volatile uint32_t CMPCR;        /* Offset: 0x20 */
} SYSCFG_TypeDef;

/* SYSCFG Pointer */
#define SYSCFG ((SYSCFG_TypeDef *)SYSCFG_BASE)

/* EXTI Register Definition Structure */
typedef struct
{
    volatile uint32_t IMR;      /* Offset: 0x00 */
    volatile uint32_t EMR;      /* Offset: 0x04 */
    volatile uint32_t RTSR;     /* Offset: 0x08 */
    volatile uint32_t FTSR;     /* Offset: 0x0C */
    volatile uint32_t SWIER;    /* Offset: 0x10 */
    volatile uint32_t PR;       /* Offset: 0x14 */
} EXTI_TypeDef;

/*EXTI Pointer */
#define EXTI ((EXTI_TypeDef *)EXTI_BASE)

/* NVIC is part of the ARM Cortex-M4 System Control Space */
#define NVIC_BASE 0xE000E100U

/* NVIC Register Definition Structure */
typedef struct
{
    volatile uint32_t ISER[8];
    uint32_t RESERVED0[24];

    volatile uint32_t ICER[8];
    uint32_t RESERVED1[24];
    
    volatile uint32_t ISPR[8];
    uint32_t RESERVED2[24];
    
    volatile uint32_t ICPR[8];
    uint32_t RESERVED3[24];
    
    volatile uint32_t IABR[8];
    uint32_t RESERVED4[56];
    
    volatile uint8_t  IPR[240];
} NVIC_TypeDef;

/* NVIC Pointer */
#define NVIC ((NVIC_TypeDef *)NVIC_BASE)

/* Interrupt Numbers */
#define EXTI0_IRQn 6U

/* GPIO port */
#define GPIO_PORT_A    0U
#define GPIO_PORT_B    1U
#define GPIO_PORT_C    2U
#define GPIO_PORT_D    3U
#define GPIO_PORT_E    4U

/* Flash interface register base */
#define FLASH_R_BASE (AHB1PERIPH_BASE + 0x3C00U)

/* Flash interface register definition structure */
typedef struct
{
    volatile uint32_t ACR;        /* Offset: 0x00 */
} FLASH_TypeDef;

/* Flash interface pointer */
#define FLASH ((FLASH_TypeDef *)FLASH_R_BASE)

/* USART2 peripheral base */
#define USART2_BASE (APB1PERIPH_BASE + 0x4400U)

/* USART register definition structure */
typedef struct
{
    volatile uint32_t SR;         /* Offset: 0x00 */
    volatile uint32_t DR;         /* Offset: 0x04 */
    volatile uint32_t BRR;        /* Offset: 0x08 */
    volatile uint32_t CR1;        /* Offset: 0x0C */
    volatile uint32_t CR2;        /* Offset: 0x10 */
    volatile uint32_t CR3;        /* Offset: 0x14 */
    volatile uint32_t GTPR;       /* Offset: 0x18 */
} USART_TypeDef;

/* USART2 pointer */
#define USART2 ((USART_TypeDef *)USART2_BASE)

/* USART status register bits */
#define USART_SR_PE_Pos      0U
#define USART_SR_FE_Pos      1U
#define USART_SR_NF_Pos      2U
#define USART_SR_ORE_Pos     3U
#define USART_SR_IDLE_Pos    4U
#define USART_SR_RXNE_Pos    5U
#define USART_SR_TC_Pos      6U
#define USART_SR_TXE_Pos     7U

/* USART control register 1 bits */
#define USART_CR1_SBK_Pos      0U
#define USART_CR1_RWU_Pos      1U
#define USART_CR1_RE_Pos       2U
#define USART_CR1_TE_Pos       3U
#define USART_CR1_IDLEIE_Pos   4U
#define USART_CR1_RXNEIE_Pos   5U
#define USART_CR1_TCIE_Pos     6U
#define USART_CR1_TXEIE_Pos    7U
#define USART_CR1_PEIE_Pos     8U
#define USART_CR1_PS_Pos       9U
#define USART_CR1_PCE_Pos     10U
#define USART_CR1_WAKE_Pos    11U
#define USART_CR1_M_Pos       12U
#define USART_CR1_UE_Pos      13U
#define USART_CR1_OVER8_Pos   15U

/* USART control register 2 bits */
#define USART_CR2_STOP_Pos    12U

/* USART control register 3 bits */
#define USART_CR3_RTSE_Pos    8U
#define USART_CR3_CTSE_Pos    9U

/* GPIO alternate function selection for USART2 */
#define GPIO_AF7_USART2    7U

/* Interrupt number for USART2 */
#define USART2_IRQn        38U

#endif
