#include "adc.h"
#include "timer.h"

void ADC_Init(void)
{
    RCC->APB2ENR |= (1U << RCC_APB2ENR_ADC1EN_Pos);                 /* Enable ADC1 clock */

    ADC1->CR1 &= ~(0x3U << ADC_CR1_RES_Pos);                        /* Clear RES bits */
    ADC1->CR1 |= (ADC_RES_12BIT << ADC_CR1_RES_Pos);                /* Set resolution to 12 bits */

    ADC1->SMPR2 &= ~(0x7U << (3U * 1U));                            /* Clear SMP1 bits */
    ADC1->SMPR2 |= (ADC_SAMPLE_84_CYCLES << (3U * 1U));             /* Set sample time for channel 1 to 84 cycles */

    ADC1->SQR1 &= ~(0xFU << 20U);                                   /* Regular sequence length = 0 which corresponds to 1 conversion */

    ADC1->SQR3 &= ~(0x1FU << 0U);                                   /* Clear SQ1 bits */
    ADC1->SQR3 |= (ADC_CHANNEL_1 << 0U);                            /* Set channel 1 as the first conversion in the regular sequence */

    ADC1->CR1 |= (1U << ADC_CR1_EOCIE_Pos);                         /* Enable end of conversion interrupt */

    ADC1->CR2 |= (1U << ADC_CR2_EOCS_Pos);                          /* Enable end of conversion selection */

    ADC1->CR2 |= (1U << ADC_CR2_CONT_Pos);                          /* Enable continuous conversion mode */

    ADC1->CR2 |= (1U << ADC_CR2_ADON_Pos);                          /* Enable ADC1 */
}

void ADC_StartConversion(void)
{
    ADC1->CR2 |= (1U << ADC_CR2_SWSTART_Pos);                       /* Start conversion */
}

uint16_t ADC_Read(void)
{
    return (uint16_t)(ADC1->DR & 0xFFFFU);                          /* Read ADC data register and return the result */
}

void ADC_IRQHandler(void)
{
    if (ADC1->SR & (1U << ADC_SR_EOC_Pos))                          /* Check if end of conversion flag is set */
    {
        uint16_t adc_value = ADC_Read();                            /* Read the ADC value */
        /* Reading DR automatically clears the EOC flag */
        
        uint32_t compare_value = ((uint32_t)adc_value * (uint32_t)TIM2->ARR) / 4095U;

        TIM2_PWM_SetCompareValue(compare_value);                    /* Update PWM duty cycle based on ADC value */
    }
}
