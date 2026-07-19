#include "wwdg.h"

void WWDG_Init(uint8_t prescaler, uint8_t window, uint8_t counter)
{
    if (window > WWDG_WINDOW_MAX)
    {
        window = WWDG_WINDOW_MAX;
    }

    if (window < WWDG_WINDOW_MIN)
    {
        window = WWDG_WINDOW_MIN;
    }

    if (counter > WWDG_COUNTER_MAX)
    {
        counter = WWDG_COUNTER_MAX;
    }

    if (counter < WWDG_COUNTER_MIN)
    {
        counter = WWDG_COUNTER_MIN;
    }

    if (window > counter)
    {
        window = counter;
    }

    if (prescaler > WWDG_PRESCALER_8)
    {
        prescaler = WWDG_PRESCALER_8;
    }

    RCC->APB1ENR |= (1U << RCC_APB1ENR_WWDGEN_Pos);                                 /* Enable WWDG peripheral clock */
    WWDG->CFR = (window << WWDG_CFR_W_Pos) | (prescaler << WWDG_CFR_WDGTB_Pos);     /* Configure window value and prescaler */
    WWDG->CR = (counter << WWDG_CR_T_Pos) | (1U << WWDG_CR_WDGA_Pos);               /* Load counter value and enable watchdog */
}

void WWDG_Refresh(uint8_t counter)
{
    if (counter > WWDG_COUNTER_MAX)
    {
        counter = WWDG_COUNTER_MAX;
    }

    if (counter < WWDG_COUNTER_MIN)
    {
        counter = WWDG_COUNTER_MIN;
    }

    WWDG->CR = (counter << WWDG_CR_T_Pos) | (1U << WWDG_CR_WDGA_Pos);               /* Load counter value and enable watchdog */
}
