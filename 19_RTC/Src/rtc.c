#include "rtc.h"

static void RTC_BackupAccessEnable(void)
{
    RCC->APB1ENR |= (1U << RCC_APB1ENR_PWREN_Pos);                                  /* Enable clock for the PWR peripheral */
    PWR->CR |= (1U << PWR_CR_DBP_Pos);                                              /* Enable write access to the backup domain */
    while (!(PWR->CR & (1U << PWR_CR_DBP_Pos)));                                    /* Wait until write access to the backup domain is enabled */
}

static void RTC_EnableLSI(void)
{
    RCC->CSR |= (1U << RCC_CSR_LSION_Pos);                                          /* Enable the LSI oscillator */
    while (!(RCC->CSR & (1U << RCC_CSR_LSIRDY_Pos)));                               /* Wait until the LSI oscillator becomes stable */
}

static void RTC_SelectClock(void)
{
    RCC->BDCR &= ~(3U << RCC_BDCR_RTCSEL_Pos);                                      /* Clear the RTC clock source selection bits */
    RCC->BDCR |= (2U << RCC_BDCR_RTCSEL_Pos);                                       /* Select LSI as the RTC clock source */
    RCC->BDCR |= (1U << RCC_BDCR_RTCEN_Pos);                                        /* Enable the RTC peripheral */
}

static void RTC_WriteProtectionEnable(void)
{
    RTC->WPR = RTC_WPR_ENABLE_KEY;                                                  /* Enable write protection for RTC registers */
}

static void RTC_WriteProtectionDisable(void)
{
    RTC->WPR = RTC_WPR_DISABLE_KEY1;                                                /* Write first key to disable write protection */
    RTC->WPR = RTC_WPR_DISABLE_KEY2;                                                /* Write second key to disable write protection */
}

static void RTC_EnterInitMode(void)
{
    RTC->ISR |= (1U << RTC_ISR_INIT_Pos);                                           /* Request initialization mode */
    while (!(RTC->ISR & (1U << RTC_ISR_INITF_Pos)));                                /* Wait until initialization mode is entered */                   
}

static void RTC_ExitInitMode(void)
{
    RTC->ISR &= ~(1U << RTC_ISR_INIT_Pos);                                          /* Exit initialization mode */
    while (RTC->ISR & (1U << RTC_ISR_INITF_Pos));                                   /* Wait until the RTC exits initialization mode (INITF cleared by hardware) */
}

static uint8_t DecimalToBCD(uint8_t value)
{
    return ((value / 10U) << 4U) | (value % 10U);                                   /* Convert decimal digits into BCD format */
}

static uint8_t BCDToDecimal(uint8_t value)
{
    return (((value >> 4U) * 10U) + (value & 0x0FU));                               /* Convert BCD digits into decimal format */
}

void RTC_Init(void)
{
    RTC_BackupAccessEnable();                                                       /* Enable access to the backup domain */
    RCC->BDCR |= (1U << RCC_BDCR_BDRST_Pos);                                        /* Reset the backup domain to allow RTC clock source reconfiguration */
    RCC->BDCR &= ~(1U << RCC_BDCR_BDRST_Pos);                                       /* Release the backup domain reset */
    RTC_EnableLSI();                                                                /* Start the LSI oscillator */
    RTC_SelectClock();                                                              /* Select LSI as the RTC clock source */
    RTC->ISR &= ~(1U << RTC_ISR_RSF_Pos);                                           /* Clear the registers synchronized flag */
    while (!(RTC->ISR & (1U << RTC_ISR_RSF_Pos)));                                  /* Wait until the RTC shadow registers are synchronized */
    RTC_WriteProtectionDisable();                                                   /* Allow RTC register modification */
    RTC_EnterInitMode();                                                            /* Enter RTC initialization mode */

    /* Assuming LSI clock frequency = 32 kHz */
    /* RTC clock = LSI / ((PREDIV_A + 1) × (PREDIV_S + 1)) */
    /* RTC clock = 32000 / ((127 + 1) × (249 + 1)) = 1 Hz */
    RTC->PRER = (127U << RTC_PRER_PREDIV_A_Pos) | (249U << RTC_PRER_PREDIV_S_Pos);  /* Configure RTC prescaler */
    RTC->CR &= ~(1U << RTC_CR_FMT_Pos);                                             /* Configure 24-hour format */
    
    RTC_ExitInitMode();                                                             /* Exit initialization mode */
    RTC_WriteProtectionEnable();                                                    /* Protect RTC registers from accidental writes */
}

void RTC_SetTime(RTC_TimeTypeDef *time)
{
    uint8_t seconds, minutes, hours;
    uint32_t tr = 0;
    if ((time->hours > 23U) || (time->minutes > 59U) || (time->seconds > 59U))      /* Validate the input time values */
    {
        return;
    }
    RTC_WriteProtectionDisable();                                                   /* Allow RTC register modification */
    RTC_EnterInitMode();                                                            /* Enter initialization mode */

    seconds = DecimalToBCD(time->seconds);                                          /* Convert seconds from decimal to BCD format */
    minutes = DecimalToBCD(time->minutes);                                          /* Convert minutes from decimal to BCD format */
    hours   = DecimalToBCD(time->hours);                                            /* Convert hours from decimal to BCD format */
    
    tr |= ((seconds & 0x0F) << RTC_TR_SU_Pos);                                      /* Write seconds units in BCD format */
    tr |= (((seconds >> 4) & 0x07) << RTC_TR_ST_Pos);                               /* Write seconds tens in BCD format */

    tr |= ((minutes & 0x0F) << RTC_TR_MNU_Pos);                                     /* Write minutes units in BCD format */
    tr |= (((minutes >> 4) & 0x07) << RTC_TR_MNT_Pos);                              /* Write minutes tens in BCD format */

    tr |= ((hours & 0x0F) << RTC_TR_HU_Pos);                                        /* Write hours units in BCD format */
    tr |= (((hours >> 4) & 0x03) << RTC_TR_HT_Pos);                                 /* Write hours tens in BCD format */

    tr &= ~(1U << RTC_TR_PM_Pos);                                                   /* Configure 24-hour format (AM/PM bit cleared) */

    RTC->TR = tr;

    RTC_ExitInitMode();                                                             /* Exit initialization mode */

    RTC_WriteProtectionEnable();                                                    /* Protect RTC registers */
}

void RTC_GetTime(RTC_TimeTypeDef *time)
{
    uint32_t tr;
    uint8_t seconds, minutes, hours;

    tr = RTC->TR;                                                                   /* Read RTC time register */

    seconds = ((tr >> RTC_TR_SU_Pos) & 0x0FU) | 
            (((tr >> RTC_TR_ST_Pos) & 0x07U) << 4U);                                /* Extract seconds in BCD format */

    minutes = ((tr >> RTC_TR_MNU_Pos) & 0x0FU) |
            (((tr >> RTC_TR_MNT_Pos) & 0x07U) << 4U);                               /* Extract minutes in BCD format */

    hours = ((tr >> RTC_TR_HU_Pos) & 0x0FU) |
            (((tr >> RTC_TR_HT_Pos) & 0x03U) << 4U);                                /* Extract hours in BCD format */

    time->seconds = BCDToDecimal(seconds);                                          /* Convert seconds to decimal format */
    time->minutes = BCDToDecimal(minutes);                                          /* Convert minutes to decimal format */
    time->hours   = BCDToDecimal(hours);                                            /* Convert hours to decimal format */
}

void RTC_SetDate(RTC_DateTypeDef *date)
{
    uint8_t day, month, year;
    uint32_t dr = 0;

    /* Validate the input date values */
    if ((date->date < 1U) || (date->date > 31U) ||
        (date->month < 1U) || (date->month > 12U) ||
        (date->year > 99U) ||
        (date->weekday < 1U) || (date->weekday > 7U))
    {
        return;
    }

    RTC_WriteProtectionDisable();                                                   /* Allow RTC register modification */
    RTC_EnterInitMode();                                                            /* Enter initialization mode */

    day   = DecimalToBCD(date->date);                                               /* Convert date from decimal to BCD format */
    month = DecimalToBCD(date->month);                                              /* Convert month from decimal to BCD format */
    year  = DecimalToBCD(date->year);                                               /* Convert year from decimal to BCD format */

    dr |= ((day & 0x0F) << RTC_DR_DU_Pos);                                          /* Write date units in BCD format */
    dr |= (((day >> 4) & 0x03) << RTC_DR_DT_Pos);                                   /* Write date tens in BCD format */

    dr |= ((month & 0x0F) << RTC_DR_MU_Pos);                                        /* Write month units in BCD format */
    dr |= (((month >> 4) & 0x01) << RTC_DR_MT_Pos);                                 /* Write month tens in BCD format */

    dr |= ((date->weekday & 0x07) << RTC_DR_WDU_Pos);                               /* Write weekday value */

    dr |= ((year & 0x0F) << RTC_DR_YU_Pos);                                         /* Write year units in BCD format */
    dr |= (((year >> 4) & 0x0F) << RTC_DR_YT_Pos);                                  /* Write year tens in BCD format */

    RTC->DR = dr;

    RTC_ExitInitMode();                                                             /* Exit initialization mode */
    RTC_WriteProtectionEnable();                                                    /* Protect RTC registers */
}

void RTC_GetDate(RTC_DateTypeDef *date)
{
    uint8_t day, month, year;

    day = (((RTC->DR) >> RTC_DR_DU_Pos) & 0x0FU) |                                  /* Read date units from BCD format */
          ((((RTC->DR) >> RTC_DR_DT_Pos) & 0x03U) << 4U);                           /* Read date tens from BCD format */

    month = (((RTC->DR) >> RTC_DR_MU_Pos) & 0x0FU) |                                /* Read month units from BCD format */
            ((((RTC->DR) >> RTC_DR_MT_Pos) & 0x01U) << 4U);                         /* Read month tens from BCD format */

    year = (((RTC->DR) >> RTC_DR_YU_Pos) & 0x0FU) |                                 /* Read year units from BCD format */
           ((((RTC->DR) >> RTC_DR_YT_Pos) & 0x0FU) << 4U);                          /* Read year tens from BCD format */

    date->date = BCDToDecimal(day);                                                 /* Convert date from BCD to decimal */
    date->month = BCDToDecimal(month);                                              /* Convert month from BCD to decimal */
    date->year = BCDToDecimal(year);                                                /* Convert year from BCD to decimal */

    date->weekday = (RTC->DR >> RTC_DR_WDU_Pos) & 0x07U;                            /* Read weekday value */
}
