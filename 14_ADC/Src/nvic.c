#include "nvic.h"

void NVIC_EnableIRQ(uint8_t IRQNumber)
{
    uint32_t registerIndex;
    uint32_t bitPosition;
 
    registerIndex = IRQNumber / 32U;    /* Each ISER register controls 32 interrupts */
    bitPosition = IRQNumber % 32U;      /* Bit position within the register */

    NVIC->ISER[registerIndex] |= (1U << bitPosition);
}

void NVIC_DisableIRQ(uint8_t IRQNumber)
{
    uint32_t registerIndex;
    uint32_t bitPosition;
 
    registerIndex = IRQNumber / 32U;    /* Each ISER register controls 32 interrupts */
    bitPosition = IRQNumber % 32U;      /* Bit position within the register */

    NVIC->ICER[registerIndex] |= (1U << bitPosition);
}

void NVIC_SetPriority(uint8_t IRQNumber, uint8_t Priority)
{
    NVIC->IPR[IRQNumber] = (Priority << 4U);  /* Each priority field is 8 bits, but only the upper 4 bits are used */
}
