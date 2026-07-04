#include "timer.h"
#include "gpio.h"

static volatile uint16_t counter = 0;

void TIM2_Init(uint32_t prescaler, uint32_t auto_reload)
{
    RCC->APB1ENR |= (1U << RCC_APB1ENR_TIM2EN_Pos);     /* Enable TIM2 clock */

    TIM2->PSC = prescaler;                              /* Set prescaler */
    TIM2->ARR = auto_reload;                            /* Set auto-reload value */
    TIM2->CNT = 0;                                      /* Reset counter */

    TIM2->EGR |= (1U << TIM_EGR_UG_Pos);                /* Generate an update event to load the prescaler value */

    TIM2->SR &= ~(1U << TIM_SR_UIF_Pos);                /* Clear update interrupt flag */

    TIM2->DIER |= (1U << TIM_DIER_UIE_Pos);             /* Enable update interrupt */
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

void TIM2_PWM_Init(uint32_t prescaler, uint32_t period, uint32_t duty_cycle)
{
    RCC->APB1ENR |= (1U << RCC_APB1ENR_TIM2EN_Pos);                     /* Enable TIM2 clock */

    TIM2->PSC = prescaler;                                              /* Set prescaler */
    TIM2->ARR = period;                                                 /* Set auto-reload value */
    TIM2->CCR1 = duty_cycle;                                            /* Set compare value for channel 1 */
    
    TIM2->CCMR1 &= ~(3U << TIM_CCMR1_CC1S_Pos);                         /* Clear CC1S bits */
    TIM2->CCMR1 &= ~(7U << TIM_CCMR1_OC1M_Pos);                         /* Clear OC1M bits */
    TIM2->CCMR1 |= (TIM_OC_MODE_PWM1 << TIM_CCMR1_OC1M_Pos);            /* Set PWM mode 1 for channel 1 */
    TIM2->CCMR1 |= (1U << TIM_CCMR1_OC1PE_Pos);                         /* Enable CCR1 preload */
    
    TIM2->CCER |= (1U << TIM_CCER_CC1E_Pos);                            /* Enable output for channel 1 */

    TIM2->CR1 |= (1U << TIM_CR1_ARPE_Pos);                              /* Enable auto-reload preload */

    TIM2->EGR |= (1U << TIM_EGR_UG_Pos);                                /* Generate an update event to load the prescaler and auto-reload values */

    TIM2_Start();                                                       /* Start TIM2 counter */
}

void TIM2_PWM_Start(void)
{
    TIM2->CCER |= (1U << TIM_CCER_CC1E_Pos);                            /* Enable Channel 1 output */

    TIM2_Start();                                                       /* Start timer */
}

void TIM2_PWM_Stop(void)
{
    TIM2->CCER &= ~(1U << TIM_CCER_CC1E_Pos);                           /* Disable output for channel 1 */

    TIM2_Stop();                                                        /* Stop timer */
}

void TIM2_PWM_SetDutyCycle(uint32_t duty_cycle)
{
    TIM2->CCR1 = duty_cycle;                                            /* Set compare value for channel 1 */
}
