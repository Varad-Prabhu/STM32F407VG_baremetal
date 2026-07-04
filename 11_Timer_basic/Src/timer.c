#include "timer.h"

void TIM2_Init(uint32_t prescaler, uint32_t auto_reload)
{
    RCC->APB1ENR |= (1U << RCC_APB1ENR_TIM2EN_Pos);     /* Enable TIM2 clock */

    TIM2->PSC = prescaler;                              /* Set prescaler */
    TIM2->ARR = auto_reload;                            /* Set auto-reload value */
    TIM2->CNT = 0;                                      /* Reset counter */

    TIM2->EGR |= (1U << TIM_EGR_UG_Pos);                /* Generate an update event to load the prescaler value */

    TIM2->SR &= ~(1U << TIM_SR_UIF_Pos);                /* Clear update interrupt flag */
}

void TIM2_Start(void)
{
    TIM2->CR1 |= (1U << TIM_CR1_CEN_Pos);               /* Enable TIM2 counter */
}

void TIM2_Stop(void)
{
    TIM2->CR1 &= ~(1U << TIM_CR1_CEN_Pos);              /* Disable TIM2 counter */
}

void TIM2_DelayMs(uint32_t delay_ms)
{
    while (delay_ms--)
    {
        TIM2->CNT = 0;                                  /* Reset counter */
        TIM2->SR &= ~(1U << TIM_SR_UIF_Pos);            /* Clear update interrupt flag */
        TIM2_Start();                                   /* Start TIM2 counter */

        while (!(TIM2->SR & (1U << TIM_SR_UIF_Pos)));   /* Wait until the update interrupt flag is set */
        
        TIM2->SR &= ~(1U << TIM_SR_UIF_Pos);            /* Clear update interrupt flag */

        TIM2_Stop();                                    /* Stop TIM2 counter */
    }
}
