#include "spi.h"

void SPI1_Init(void)
{
    RCC->APB2ENR |= (1U << RCC_APB2ENR_SPI1EN_Pos);                 /* Enable SPI1 clock */

    SPI1->CR1 &= ~(1U << SPI_CR1_SPE_Pos);                          /* Disable SPI1 before configuration */
    
    SPI1->CR1 &= ~(1U << SPI_CR1_MSTR_Pos);                         /* Clear master/slave mode bit */
    SPI1->CR1 |= (SPI_MODE_MASTER << SPI_CR1_MSTR_Pos);             /* Set SPI1 to Master mode */

    SPI1->CR1 &= ~(7U << SPI_CR1_BR_Pos);                           /* Clear baud rate bits */
    SPI1->CR1 |= (SPI_BAUD_DIV16 << SPI_CR1_BR_Pos);                /* Set baud rate to fPCLK/16 */

    SPI1->CR1 &= ~(1U << SPI_CR1_CPOL_Pos);                         /* Set clock polarity to 0 */
    SPI1->CR1 &= ~(1U << SPI_CR1_CPHA_Pos);                         /* Set clock phase to 0 */

    SPI1->CR1 &= ~(1U << SPI_CR1_LSBFIRST_Pos);                     /* Set data order to MSB first */

    SPI1->CR1 &= ~(1U << SPI_CR1_DFF_Pos);                          /* Select 8-bit data frame */

    SPI1->CR1 &= ~(1U << SPI_CR1_RXONLY_Pos);                       /* Full-duplex mode */
    SPI1->CR1 &= ~(1U << SPI_CR1_BIDIMODE_Pos);                     /* 2-line unidirectional mode */

    SPI1->CR1 |= (1U << SPI_CR1_SSM_Pos);                           /* Enable software slave management */
    SPI1->CR1 |= (1U << SPI_CR1_SSI_Pos);                           /* Set internal slave select */

    SPI1->CR1 |= (1U << SPI_CR1_SPE_Pos);                           /* Enable SPI1 */
}

void SPI1_Transmit(uint8_t data)
{
    while (!(SPI1->SR & (1U << SPI_SR_TXE_Pos)));                   /* Wait until TX buffer is empty */
    SPI1->DR = data;                                                /* Transmit data */
    while (SPI1->SR & (1U << SPI_SR_BSY_Pos));                      /* Wait until SPI is not busy */
    while (!(SPI1->SR & (1U << SPI_SR_RXNE_Pos)));                  /* Wait until RX buffer is not empty */
    (void)SPI1->DR;                                                 /* Discard received data */
}

uint8_t SPI1_Receive(void)
{
    while (!(SPI1->SR & (1U << SPI_SR_TXE_Pos)));                   /* Wait until TX buffer is empty */
    SPI1->DR = 0xFF;                                                /* Dummy write to generate clock */
    while (!(SPI1->SR & (1U << SPI_SR_RXNE_Pos)));                  /* Wait until RX buffer is not empty */
    while (SPI1->SR & (1U << SPI_SR_BSY_Pos));                      /* Wait until SPI is not busy */
    return (uint8_t)(SPI1->DR);                                     /* Read received data */
}

uint8_t SPI1_TransmitReceive(uint8_t data)
{
    while (!(SPI1->SR & (1U << SPI_SR_TXE_Pos)));                   /* Wait until TX buffer is empty */
    SPI1->DR = data;                                                /* Send data */

    while (!(SPI1->SR & (1U << SPI_SR_RXNE_Pos)));                  /* Wait until data is received */
    while (SPI1->SR & (1U << SPI_SR_BSY_Pos));                      /* Wait until SPI is not busy */
    
    return (uint8_t)SPI1->DR;                                       /* Return received byte */
}
