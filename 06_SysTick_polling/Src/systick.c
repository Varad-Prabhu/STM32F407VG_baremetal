#include "systick.h"

void SysTick_Init(uint32_t ticks)
{
    SYSTICK->CTRL = 0U;                                     /* Disable SysTick Timer */
    if ((ticks == 0U) || (ticks > 0xFFFFFFU))
    {
        return;
    }
    /* SysTick counts from LOAD down to 0. Thus, LOAD value is one less than the desired delay */
    SYSTICK->LOAD = ticks - 1U;                             /* Set reload register */
    SYSTICK->VAL = 0U;                                      /* Reset the SysTick counter value */
}

void SysTick_Start(void)
{
    SYSTICK->CTRL |= (1U << SYSTICK_CTRL_CLKSOURCE_Pos);    /* Select clock source as processor clock (AHB) */
    SYSTICK->CTRL |= (1U << SYSTICK_CTRL_ENABLE_Pos);       /* Counter enabled */
}

void SysTick_Stop(void)
{
    SYSTICK->CTRL &= ~(1U << SYSTICK_CTRL_ENABLE_Pos);      /* Counter disabled */
}

void SysTick_DelayMs(uint32_t delay)
{
    /* Implementation for delay in milliseconds */
    while (delay--)
    {
        /* Clear current counter value */
        SYSTICK->VAL = 0U;

        /* Start SysTick */
        SysTick_Start();

        /* Wait until the COUNTFLAG is set */
        while ((SYSTICK->CTRL & (1U << SYSTICK_CTRL_COUNTFLAG_Pos)) == 0U)
        {
            /* Wait for COUNTFLAG */
        }

        /* Stop SysTick */
        SysTick_Stop();
    }
}