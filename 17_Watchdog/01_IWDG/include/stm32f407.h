#ifndef STM32F407_H
#define STM32F407_H

#include <stdint.h>
#include <stddef.h>

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
#define RCC_APB2ENR_ADC1EN_Pos    8U
#define RCC_APB2ENR_SPI1EN_Pos    12U

/* RCC APB1 Clock Enable Register Bits */
#define RCC_APB1ENR_TIM2EN_Pos    0U
#define RCC_APB1ENR_TIM3EN_Pos    1U
#define RCC_APB1ENR_TIM4EN_Pos    2U
#define RCC_APB1ENR_TIM5EN_Pos    3U
#define RCC_APB1ENR_USART2EN_Pos  17U
#define RCC_APB1ENR_SPI2EN_Pos    14U
#define RCC_APB1ENR_SPI3EN_Pos    15U
#define RCC_APB1ENR_I2C1EN_Pos    21U
#define RCC_APB1ENR_I2C2EN_Pos    22U
#define RCC_APB1ENR_I2C3EN_Pos    23U

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

/* TIM2 peripheral base */
#define TIM2_BASE (APB1PERIPH_BASE + 0x0000U)

/* TIM3 peripheral base */
#define TIM3_BASE (APB1PERIPH_BASE + 0x0400U)

/* TIM4 peripheral base */
#define TIM4_BASE (APB1PERIPH_BASE + 0x0800U)

/* TIM5 peripheral base */
#define TIM5_BASE (APB1PERIPH_BASE + 0x0C00U)

/* TIM peripheral register definition structure */
typedef struct
{
    volatile uint32_t CR1;        /* Offset: 0x00 */
    volatile uint32_t CR2;        /* Offset: 0x04 */
    volatile uint32_t SMCR;       /* Offset: 0x08 */
    volatile uint32_t DIER;       /* Offset: 0x0C */
    volatile uint32_t SR;         /* Offset: 0x10 */
    volatile uint32_t EGR;        /* Offset: 0x14 */
    volatile uint32_t CCMR1;      /* Offset: 0x18 */
    volatile uint32_t CCMR2;      /* Offset: 0x1C */
    volatile uint32_t CCER;       /* Offset: 0x20 */
    volatile uint32_t CNT;        /* Offset: 0x24 */
    volatile uint32_t PSC;        /* Offset: 0x28 */
    volatile uint32_t ARR;        /* Offset: 0x2C */
    uint32_t RESERVED1;           /* Offset: 0x30 */
    volatile uint32_t CCR1;       /* Offset: 0x34 */
    volatile uint32_t CCR2;       /* Offset: 0x38 */
    volatile uint32_t CCR3;       /* Offset: 0x3C */
    volatile uint32_t CCR4;       /* Offset: 0x40 */
    uint32_t RESERVED2;           /* Offset: 0x44 */
    volatile uint32_t DCR;        /* Offset: 0x48 */
    volatile uint32_t DMAR;       /* Offset: 0x4C */
} TIM_TypeDef;

/* TIM2 pointer */
#define TIM2 ((TIM_TypeDef *)TIM2_BASE)

/* TIM3 pointer */
#define TIM3 ((TIM_TypeDef *)TIM3_BASE)

/* TIM4 pointer */
#define TIM4 ((TIM_TypeDef *)TIM4_BASE)

/* TIM5 pointer */
#define TIM5 ((TIM_TypeDef *)TIM5_BASE)

/* TIM CR1 Register Bits */
#define TIM_CR1_CEN_Pos      0U
#define TIM_CR1_UDIS_Pos     1U
#define TIM_CR1_URS_Pos      2U
#define TIM_CR1_OPM_Pos      3U
#define TIM_CR1_DIR_Pos      4U
#define TIM_CR1_ARPE_Pos     7U

/* TIM SR Register Bits */
#define TIM_SR_UIF_Pos       0U

/* TIM EGR Register Bits */
#define TIM_EGR_UG_Pos       0U

/* TIM DIER Register Bits */
#define TIM_DIER_UIE_Pos     0U

/* Interrupt number for TIM2 */
#define TIM2_IRQn 28U

/* TIM CCMR1 Register Bits */
#define TIM_CCMR1_CC1S_Pos    0U
#define TIM_CCMR1_OC1FE_Pos   2U
#define TIM_CCMR1_OC1PE_Pos   3U
#define TIM_CCMR1_OC1M_Pos    4U

/* TIM CCER Register Bits */
#define TIM_CCER_CC1E_Pos     0U
#define TIM_CCER_CC1P_Pos     1U

/* PWM Modes */
#define TIM_OC_MODE_PWM1      6U
#define TIM_OC_MODE_PWM2      7U

/* Timer channels */
#define TIM_CHANNEL_1         1U
#define TIM_CHANNEL_2         2U
#define TIM_CHANNEL_3         3U
#define TIM_CHANNEL_4         4U

/* GPIO alternate function selection for TIM2 */
#define GPIO_AF1_TIM1_TIM2    1U

/* ADC1 peripheral base */
#define ADC1_BASE (APB2PERIPH_BASE + 0x2000U)

/* ADC register definition structure */
typedef struct
{
    volatile uint32_t SR;         /* Offset: 0x00 */
    volatile uint32_t CR1;        /* Offset: 0x04 */
    volatile uint32_t CR2;        /* Offset: 0x08 */
    volatile uint32_t SMPR1;      /* Offset: 0x0C */
    volatile uint32_t SMPR2;      /* Offset: 0x10 */
    volatile uint32_t JOFR[4];    /* Offset: 0x14, 0x18, 0x1C, 0x20 */
    volatile uint32_t HTR;        /* Offset: 0x24 */
    volatile uint32_t LTR;        /* Offset: 0x28 */
    volatile uint32_t SQR1;       /* Offset: 0x2C */
    volatile uint32_t SQR2;       /* Offset: 0x30 */
    volatile uint32_t SQR3;       /* Offset: 0x34 */
    volatile uint32_t JSQR;       /* Offset: 0x38 */
    volatile uint32_t JDR[4];     /* Offset: 0x3C, 0x40, 0x44, 0x48 */
    volatile uint32_t DR;         /* Offset: 0x4C */
} ADC_TypeDef;

/* ADC1 pointer */
#define ADC1 ((ADC_TypeDef *)ADC1_BASE)

/* ADC SR Register Bits */
#define ADC_SR_AWD_Pos      0U
#define ADC_SR_EOC_Pos      1U
#define ADC_SR_JEOC_Pos     2U
#define ADC_SR_JSTRT_Pos    3U
#define ADC_SR_STRT_Pos     4U
#define ADC_SR_OVR_Pos      5U

/* ADC CR1 Register Bits */
#define ADC_CR1_AWDCH_Pos    0U
#define ADC_CR1_EOCIE_Pos    5U
#define ADC_CR1_AWDIE_Pos    6U
#define ADC_CR1_JEOCIE_Pos   7U
#define ADC_CR1_SCAN_Pos     8U
#define ADC_CR1_AWDEN_Pos    23U
#define ADC_CR1_RES_Pos      24U
#define ADC_CR1_OVRIE_Pos    26U

/* ADC CR2 Register Bits */
#define ADC_CR2_ADON_Pos      0U
#define ADC_CR2_CONT_Pos      1U
#define ADC_CR2_DMA_Pos       8U
#define ADC_CR2_DDS_Pos       9U
#define ADC_CR2_EOCS_Pos      10U
#define ADC_CR2_ALIGN_Pos     11U
#define ADC_CR2_EXTSEL_Pos    24U
#define ADC_CR2_EXTEN_Pos     28U
#define ADC_CR2_SWSTART_Pos   30U

/* ADC sample time defines */
#define ADC_SAMPLE_3_CYCLES      0U
#define ADC_SAMPLE_15_CYCLES     1U
#define ADC_SAMPLE_28_CYCLES     2U
#define ADC_SAMPLE_56_CYCLES     3U
#define ADC_SAMPLE_84_CYCLES     4U
#define ADC_SAMPLE_112_CYCLES    5U
#define ADC_SAMPLE_144_CYCLES    6U
#define ADC_SAMPLE_480_CYCLES    7U

/* ADC resolution defines */
#define ADC_RES_12BIT    0U
#define ADC_RES_10BIT    1U
#define ADC_RES_8BIT     2U
#define ADC_RES_6BIT     3U

/* ADC channels */
/*----------------------------------------------------------
 * ADC Channel to GPIO Mapping (STM32F407VG)
 *----------------------------------------------------------
 *
 * ADC Channel    GPIO Pin
 * -----------    --------
 * CH0            PA0
 * CH1            PA1
 * CH2            PA2
 * CH3            PA3
 * CH4            PA4
 * CH5            PA5
 * CH6            PA6
 * CH7            PA7
 * CH8            PB0
 * CH9            PB1
 * CH10           PC0
 * CH11           PC1
 * CH12           PC2
 * CH13           PC3
 * CH14           PC4
 * CH15           PC5
 * CH16           Internal Temperature Sensor
 * CH17           Internal VREFINT (Reference Voltage)
 * CH18           VBAT (Battery Voltage)
 *
 * Notes:
 * - Channels 0-15 are external GPIO pins.
 * - Channel 16 is connected internally to the temperature sensor.
 * - Channel 17 is connected internally to VREFINT.
 * - Channel 18 is connected internally to VBAT
 *   (requires VBAT circuitry on the board).
 *----------------------------------------------------------*/
#define ADC_CHANNEL_0     0U
#define ADC_CHANNEL_1     1U
#define ADC_CHANNEL_2     2U
#define ADC_CHANNEL_3     3U
#define ADC_CHANNEL_4     4U
#define ADC_CHANNEL_5     5U
#define ADC_CHANNEL_6     6U
#define ADC_CHANNEL_7     7U
#define ADC_CHANNEL_8     8U
#define ADC_CHANNEL_9     9U
#define ADC_CHANNEL_10    10U
#define ADC_CHANNEL_11    11U
#define ADC_CHANNEL_12    12U
#define ADC_CHANNEL_13    13U
#define ADC_CHANNEL_14    14U
#define ADC_CHANNEL_15    15U
#define ADC_CHANNEL_16    16U
#define ADC_CHANNEL_17    17U
#define ADC_CHANNEL_18    18U

/* Interrupt number for ADC */
#define ADC_IRQn 18U

/* SPI peripheral base */
#define SPI1_BASE   (APB2PERIPH_BASE + 0x3000U)
#define SPI2_BASE   (APB1PERIPH_BASE + 0x3800U)
#define SPI3_BASE   (APB1PERIPH_BASE + 0x3C00U)

/* SPI register definition structure */
typedef struct {
    volatile uint32_t CR1;      /* Offset: 0x00 */
    volatile uint32_t CR2;      /* Offset: 0x04 */
    volatile uint32_t SR;       /* Offset: 0x08 */
    volatile uint32_t DR;       /* Offset: 0x0C */
    volatile uint32_t CRCPR;    /* Offset: 0x10 */
    volatile uint32_t RXCRCR;   /* Offset: 0x14 */
    volatile uint32_t TXCRCR;   /* Offset: 0x18 */
    volatile uint32_t I2SCFGR;  /* Offset: 0x1C */
    volatile uint32_t I2SPR;    /* Offset: 0x20 */
} SPI_TypeDef;

/* SPI pointer */
#define SPI1 ((SPI_TypeDef *)SPI1_BASE)
#define SPI2 ((SPI_TypeDef *)SPI2_BASE)
#define SPI3 ((SPI_TypeDef *)SPI3_BASE)

/* Alternate function selection for SPI */
#define GPIO_AF5_SPI1    5U
#define GPIO_AF5_SPI2    5U
#define GPIO_AF6_SPI3    6U

/* SPI CR1 Register Bits */
#define SPI_CR1_CPHA_Pos        0U
#define SPI_CR1_CPOL_Pos        1U
#define SPI_CR1_MSTR_Pos        2U
#define SPI_CR1_BR_Pos          3U
#define SPI_CR1_SPE_Pos         6U
#define SPI_CR1_LSBFIRST_Pos    7U
#define SPI_CR1_SSI_Pos         8U
#define SPI_CR1_SSM_Pos         9U
#define SPI_CR1_RXONLY_Pos      10U
#define SPI_CR1_DFF_Pos         11U
#define SPI_CR1_BIDIOE_Pos      14U
#define SPI_CR1_BIDIMODE_Pos    15U

/* SPI CR2 Register Bits */
#define SPI_CR2_RXDMAEN_Pos     0U
#define SPI_CR2_TXDMAEN_Pos     1U
#define SPI_CR2_SSOE_Pos        2U
#define SPI_CR2_FRF_Pos         4U
#define SPI_CR2_ERRIE_Pos       5U
#define SPI_CR2_RXNEIE_Pos      6U
#define SPI_CR2_TXEIE_Pos       7U

/* SPI SR Register Bits */
#define SPI_SR_RXNE_Pos         0U
#define SPI_SR_TXE_Pos          1U
#define SPI_SR_CHSIDE_Pos       2U
#define SPI_SR_UDR_Pos          3U
#define SPI_SR_CRCERR_Pos       4U
#define SPI_SR_MODF_Pos         5U
#define SPI_SR_OVR_Pos          6U
#define SPI_SR_BSY_Pos          7U

/* Interrupt number for SPI */
#define SPI1_IRQn 35U
#define SPI2_IRQn 36U
#define SPI3_IRQn 51U

/* SPI Modes */
#define SPI_MODE_SLAVE      0U
#define SPI_MODE_MASTER     1U

/* SPI Data Frame Format */
#define SPI_DFF_8BIT        0U
#define SPI_DFF_16BIT       1U

/* SPI Bit Order */
#define SPI_MSB_FIRST       0U
#define SPI_LSB_FIRST       1U

/* SPI Clock Modes */
#define SPI_MODE0   0U      /* CPOL=0 CPHA=0 */
#define SPI_MODE1   1U      /* CPOL=0 CPHA=1 */
#define SPI_MODE2   2U      /* CPOL=1 CPHA=0 */
#define SPI_MODE3   3U      /* CPOL=1 CPHA=1 */

/* SPI baud rate prescaler */
#define SPI_BAUD_DIV2      0U
#define SPI_BAUD_DIV4      1U
#define SPI_BAUD_DIV8      2U
#define SPI_BAUD_DIV16     3U
#define SPI_BAUD_DIV32     4U
#define SPI_BAUD_DIV64     5U
#define SPI_BAUD_DIV128    6U
#define SPI_BAUD_DIV256    7U

/* Alternate function selection for I2C1 */
#define GPIO_AF4_I2C1    4U
#define GPIO_AF4_I2C2    4U
#define GPIO_AF4_I2C3    4U

/* I2C peripheral base addresses */
#define I2C1_BASE    (APB1PERIPH_BASE + 0x5400U)
#define I2C2_BASE    (APB1PERIPH_BASE + 0x5800U)
#define I2C3_BASE    (APB1PERIPH_BASE + 0x5C00U)

/* I2C register definition structure */
typedef struct
{
    volatile uint32_t CR1;      /* Offset: 0x00 */
    volatile uint32_t CR2;      /* Offset: 0x04 */
    volatile uint32_t OAR1;     /* Offset: 0x08 */
    volatile uint32_t OAR2;     /* Offset: 0x0C */
    volatile uint32_t DR;       /* Offset: 0x10 */
    volatile uint32_t SR1;      /* Offset: 0x14 */
    volatile uint32_t SR2;      /* Offset: 0x18 */
    volatile uint32_t CCR;      /* Offset: 0x1C */
    volatile uint32_t TRISE;    /* Offset: 0x20 */
    volatile uint32_t FLTR;     /* Offset: 0x24 */
} I2C_TypeDef;

/* I2C pointers */
#define I2C1    ((I2C_TypeDef *)I2C1_BASE)
#define I2C2    ((I2C_TypeDef *)I2C2_BASE)
#define I2C3    ((I2C_TypeDef *)I2C3_BASE)

/* I2C CR1 Register Bits */
#define I2C_CR1_PE_Pos         0U
#define I2C_CR1_START_Pos      8U
#define I2C_CR1_STOP_Pos       9U
#define I2C_CR1_ACK_Pos        10U
#define I2C_CR1_POS_Pos        11U
#define I2C_CR1_SWRST_Pos      15U

/* I2C CR2 Register Bits */
#define I2C_CR2_FREQ_Pos       0U
#define I2C_CR2_ITERREN_Pos    8U
#define I2C_CR2_ITEVTEN_Pos    9U
#define I2C_CR2_ITBUFEN_Pos    10U

/* I2C OAR1 Register Bits */
#define I2C_OAR1_ADD0_Pos      0U
#define I2C_OAR1_ADD1_7_Pos    1U
#define I2C_OAR1_ADD8_9_Pos    8U
#define I2C_OAR1_ADDMODE_Pos   15U

/* I2C SR1 Register Bits */
#define I2C_SR1_SB_Pos         0U
#define I2C_SR1_ADDR_Pos       1U
#define I2C_SR1_BTF_Pos        2U
#define I2C_SR1_ADD10_Pos      3U
#define I2C_SR1_STOPF_Pos      4U
#define I2C_SR1_RXNE_Pos       6U
#define I2C_SR1_TXE_Pos        7U
#define I2C_SR1_BERR_Pos       8U
#define I2C_SR1_ARLO_Pos       9U
#define I2C_SR1_AF_Pos         10U
#define I2C_SR1_OVR_Pos        11U
#define I2C_SR1_TIMEOUT_Pos    14U

/* I2C SR2 Register Bits */
#define I2C_SR2_MSL_Pos        0U
#define I2C_SR2_BUSY_Pos       1U
#define I2C_SR2_TRA_Pos        2U

/* I2C CCR Register Bits */
#define I2C_CCR_CCR_Pos        0U
#define I2C_CCR_DUTY_Pos       14U
#define I2C_CCR_FS_Pos         15U

/* Interrupt number for I2C1 */
#define I2C1_EV_IRQn    31U
#define I2C1_ER_IRQn    32U

/* I2C acknowledge defines */
#define I2C_ACK_ENABLE    1U
#define I2C_ACK_DISABLE   0U

/* I2C addressing mode */
#define I2C_ADDRESS_7BIT    0U
#define I2C_ADDRESS_10BIT   1U

/* IWDG peripheral base address */
#define IWDG_BASE    (APB1PERIPH_BASE + 0x3000U)

/* IWDG register definition structure */
typedef struct
{
    volatile uint32_t KR;       /* Offset: 0x00 */
    volatile uint32_t PR;       /* Offset: 0x04 */
    volatile uint32_t RLR;      /* Offset: 0x08 */
    volatile uint32_t SR;       /* Offset: 0x0C */
} IWDG_TypeDef;

/* IWDG pointer */
#define IWDG    ((IWDG_TypeDef *)IWDG_BASE)

/* IWDG SR Register Bits */
#define IWDG_SR_PVU_Pos        0U
#define IWDG_SR_RVU_Pos        1U

/* Key Register values */
#define IWDG_KR_ENABLE          0xCCCCU
#define IWDG_KR_RELOAD          0xAAAAU
#define IWDG_KR_WRITE_ENABLE    0x5555U

/* Prescaler values */
#define IWDG_PRESCALER_4        0U
#define IWDG_PRESCALER_8        1U
#define IWDG_PRESCALER_16       2U
#define IWDG_PRESCALER_32       3U
#define IWDG_PRESCALER_64       4U
#define IWDG_PRESCALER_128      5U
#define IWDG_PRESCALER_256      6U

/* Maximum reload value */
#define IWDG_RELOAD_MAX         0x0FFFU

/* RCC watchdog flag */
#define RCC_CSR_RMVF_Pos        24U
#define RCC_CSR_IWDGRSTF_Pos    29U
#define RCC_CSR_WWDGRSTF_Pos    30U

#endif
