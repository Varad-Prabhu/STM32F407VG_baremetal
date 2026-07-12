#include "iwdg.h"

void IWDG_Init(uint8_t prescaler, uint16_t reload)
{
    if (reload > IWDG_RELOAD_MAX)
    {
        reload = IWDG_RELOAD_MAX;
    }

    if (prescaler > IWDG_PRESCALER_256)
    {
        prescaler = IWDG_PRESCALER_256;
    }

    IWDG->KR = IWDG_KR_ENABLE;                                          /* Start the watchdog */

    IWDG->KR = IWDG_KR_WRITE_ENABLE;                                    /* Enable write access to prescaler and reload register */
    
    IWDG->PR = prescaler;                                               /* Set the prescaler */
    while (IWDG->SR & (1U << IWDG_SR_PVU_Pos));                         /* Wait until the prescaler update completes */

    IWDG->RLR = reload;                                                 /* Set the reload value */
    while (IWDG->SR & (1U << IWDG_SR_RVU_Pos));                         /* Wait until the reload update completes */

    IWDG->KR = IWDG_KR_RELOAD;                                          /* Reload the counter */
}

void IWDG_Refresh(void)
{
    IWDG->KR = IWDG_KR_RELOAD;                                          /* Reload watchdog counter */
}
