#include "usart.h"
#include "gpio.h"

void USART2_Init(void)
{
    GPIO_InitTypeDef GPIO_USART;

    GPIO_USART.Port = GPIOA;
    GPIO_USART.Pin = GPIO_PIN_2 | GPIO_PIN_3;   /* PA2 -> USART2_Tx, PA3 -> USART2_Rx */
    GPIO_USART.Mode = GPIO_MODE_AF;
    GPIO_USART.Pull = GPIO_PULLUP;
    GPIO_USART.Speed = GPIO_SPEED_HIGH;
    GPIO_USART.OutputType = GPIO_OTYPE_PP;
    GPIO_USART.Alternate = GPIO_AF7_USART2;

    GPIO_Init(&GPIO_USART);

    RCC->APB1ENR |= (1U << RCC_APB1ENR_USART2EN_Pos);   /* Enable USART2 clock */

    USART2->CR1 &= ~(1U << USART_CR1_UE_Pos);       /* Disable USART2 before configuration */
    USART2->CR1 &= ~(1U << USART_CR1_M_Pos);        /* 8 data bits */
    USART2->CR1 &= ~(1U << USART_CR1_PCE_Pos);      /* No parity */
    USART2->CR1 &= ~(1U << USART_CR1_OVER8_Pos);    /* Oversampling by 16 */
    
    USART2->CR2 &= ~(3U << USART_CR2_STOP_Pos);     /* 1 stop bit */

    USART2->CR3 &= ~(1U << USART_CR3_CTSE_Pos);      /* Disable CTS */
    USART2->CR3 &= ~(1U << USART_CR3_RTSE_Pos);      /* Disable RTS */

    /* Configure USART2 BRR */
    /* USARTDIV = pclk1 / (16 * baudrate) */
    /* Here pclk1 = 42 MHz, Baudrate = 115200 */
    /* USARTDIV = 42000000 / (16 * 115200) = 22.786458 */
    /* DIV_MANTISSA = 22 = 0x16, DIV_FRACTION = 0.786458 * 16 = 12.583328 = 13 = 0xD */
    /* BRR = DIV_MANTISSA << 4 | DIV_FRACTION = 0x16D */
    USART2->BRR = 0x16D;                            /* Set baud rate to 115200 */
    USART2->CR1 |= (1U << USART_CR1_RE_Pos);        /* Enable Receiver */
    USART2->CR1 |= (1U << USART_CR1_TE_Pos);        /* Enable Transmitter */
    USART2->CR1 |= (1U << USART_CR1_UE_Pos);        /* Enable USART2 */
}

void USART2_WriteChar(char ch)
{
    while (!(USART2->SR & (1U << USART_SR_TXE_Pos)));   /* Wait until TXE is set */
    USART2->DR = (uint8_t)ch;                           /* Write character to data register */
}

void USART2_WriteString(const char *str)
{
    char ch;
    while (*str)
    {
        ch = *str++;
        USART2_WriteChar(ch);
    }
    while (!(USART2->SR & (1U << USART_SR_TC_Pos)));
}

char USART2_ReadChar(void)
{
    while (!(USART2->SR & (1U << USART_SR_RXNE_Pos)));      /* Wait until RXNE is set */
    return (char)(USART2->DR & 0xFFU);                      /* Read character from data register */
}

void USART2_SendHex(uint8_t value)
{
    char hex_chars[] = "0123456789ABCDEF";

    USART2_WriteChar(hex_chars[(value >> 4) & 0x0F]);    /* Send upper nibble */
    USART2_WriteChar(hex_chars[value & 0x0F]);           /* Send lower nibble */
}
