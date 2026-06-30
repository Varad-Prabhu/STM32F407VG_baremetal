#include "systick.h"

static volatile uint32_t g_sysTickCount = 0U;

void SysTick_Handler(void)
{
    g_sysTickCount++;
}

uint32_t SysTick_GetTick(void)
{
    return g_sysTickCount;
}

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
    SYSTICK->CTRL |= (1U << SYSTICK_CTRL_TICKINT_Pos);      /* Enable SysTick interrupt */
}

void SysTick_Stop(void)
{
    SYSTICK->CTRL &= ~(1U << SYSTICK_CTRL_ENABLE_Pos);      /* Counter disabled */
    SYSTICK->CTRL &= ~(1U << SYSTICK_CTRL_TICKINT_Pos);     /* Disable SysTick interrupt */
}
