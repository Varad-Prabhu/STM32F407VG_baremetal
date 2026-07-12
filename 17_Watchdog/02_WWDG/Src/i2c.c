#include "i2c.h"

void I2C1_Init(uint32_t speed)
{
    RCC->APB1ENR |= (1U << RCC_APB1ENR_I2C1EN_Pos);                 /* Enable I2C1 clock */

    I2C1->CR1 &= ~(1U << I2C_CR1_PE_Pos);                           /* Disable I2C1 before configuration */

    I2C1->CR2 &= ~(0x3FU << I2C_CR2_FREQ_Pos);                      /* Clear FREQ bits */
    I2C1->CR2 |= (42U << I2C_CR2_FREQ_Pos);                         /* Set FREQ bits to 42 MHz (PCLK1 frequency) */

    I2C1->CCR = 0U;                                                 /* Clear CCR register */
    if (speed == I2C_STANDARD_MODE)
    {
        I2C1->CCR &= ~(1U << I2C_CCR_FS_Pos);                       /* Standard mode (100 kHz) */

        /* I2C1_PCLK1_FREQ = 42 MHz, I2C Speed = 100 kHz*/
        /* CCR = I2C1_PCLK1_FREQ / (2 * I2C Speed) */
        /* CCR = 42000000 / (2 * 100000) */
        /* CCR = 210 */

        /* TRISE = FREQ + 1 */
        /* TRISE = 42 + 1 = 43 */
        I2C1->CCR |= (I2C1_PCLK1_FREQ / (2U * speed));
        I2C1->TRISE = 43U;
    }
    else if (speed == I2C_FAST_MODE)
    {
        I2C1->CCR |= (1U << I2C_CCR_FS_Pos);                        /* Fast mode (400 kHz) */
        I2C1->CCR &= ~(1U << I2C_CCR_DUTY_Pos);                     /* Duty cycle = 2 (t_low/t_high = 2) */

        /* I2C1_PCLK1_FREQ = 42 MHz, I2C Speed = 400 kHz */
        /* CCR = I2C1_PCLK1_FREQ / (3 * I2C Speed) */
        /* CCR = 42000000 / (3 * 400000) */
        /* CCR = 35 */

        /* TRISE = (FREQ * 300ns) + 1 */
        /* TRISE = (42 * 0.3) + 1 = 13.6 ~ 14 */
        I2C1->CCR |= (I2C1_PCLK1_FREQ / (3U * speed));
        I2C1->TRISE = 14U;
    }
    else
    {
        return;    /* Invalid speed */
    }

    I2C1->CR1 |= (1U << I2C_CR1_PE_Pos);                            /* Enable I2C1 */
}

void I2C1_Start(void)
{
    I2C1->CR1 |= (1U << I2C_CR1_START_Pos);                             /* Generate START condition */
    while (!(I2C1->SR1 & (1U << I2C_SR1_SB_Pos)));                      /* Wait until START condition is generated */
}

void I2C1_Stop(void)
{
    I2C1->CR1 |= (1U << I2C_CR1_STOP_Pos);                              /* Generate STOP condition */
}

void I2C1_SendAddress(uint8_t address, uint8_t direction)
{
    volatile uint32_t temp;

    I2C1->DR = (address << 1U) | direction;                             /* Send address with read/write bit */
    while (!(I2C1->SR1 & (1U << I2C_SR1_ADDR_Pos)));                    /* Wait until address is acknowledged */

    /* Read SR1 followed by SR2 to clear ADDR flag*/
    temp = I2C1->SR1;
    temp = I2C1->SR2;

    (void)temp;
}

void I2C1_WriteByte(uint8_t data)
{
    while (!(I2C1->SR1 & (1U << I2C_SR1_TXE_Pos)));                     /* Wait until data register is empty */
    I2C1->DR = data;                                                    /* Send data byte */
    while (!(I2C1->SR1 & (1U << I2C_SR1_BTF_Pos)));                     /* Wait until byte transfer is finished */
}

uint8_t I2C1_ReadByte_ACK(void)
{
    I2C1->CR1 |= (1U << I2C_CR1_ACK_Pos);                               /* Enable ACK for next byte */
    while (!(I2C1->SR1 & (1U << I2C_SR1_RXNE_Pos)));                    /* Wait until data is received */
    return (uint8_t)I2C1->DR;                                           /* Read received byte */
}

uint8_t I2C1_ReadByte_NACK(void)
{
    uint8_t data;
    I2C1->CR1 &= ~(1U << I2C_CR1_ACK_Pos);                              /* Disable ACK for next byte */
    while (!(I2C1->SR1 & (1U << I2C_SR1_RXNE_Pos)));                    /* Wait until data is received */
    data = (uint8_t)I2C1->DR;                                           /* Read received byte */
    I2C1->CR1 |= (1U << I2C_CR1_ACK_Pos);                               /* Re-enable ACK for subsequent reads */
    return data;
}

void I2C1_Write(uint8_t slave_address, uint8_t register_address, uint8_t data)
{
    I2C1_Start();                                                       /* Generate START condition */
    I2C1_SendAddress(slave_address, I2C_WRITE);                         /* Send slave address with write bit */
    I2C1_WriteByte(register_address);                                   /* Send register address */
    I2C1_WriteByte(data);                                               /* Send data byte */
    I2C1_Stop();                                                        /* Generate STOP condition */
}

uint8_t I2C1_Read(uint8_t slave_address, uint8_t register_address)
{
    uint8_t data;

    I2C1_Start();                                                       /* Generate START condition */
    I2C1_SendAddress(slave_address, I2C_WRITE);                         /* Send slave address with write bit */
    I2C1_WriteByte(register_address);                                   /* Send register address */

    /* Generate repeated START without releasing the bus */
    I2C1_Start();                                                       /* Generate START condition */
    I2C1_SendAddress(slave_address, I2C_READ);                          /* Send slave address with read bit */
    data = I2C1_ReadByte_NACK();                                        /* Read received byte */
    I2C1_Stop();                                                        /* Generate STOP condition */

    return data;
}

void I2C1_ReadBuffer(uint8_t slave_address, uint8_t register_address, uint8_t *buffer, uint16_t length)
{
    uint16_t i;

    if ((buffer == NULL) || (length == 0U))
    {
        return;
    }

    I2C1_Start();                                                       /* Generate START condition */
    I2C1_SendAddress(slave_address, I2C_WRITE);                         /* Send slave address with write bit */
    I2C1_WriteByte(register_address);                                   /* Send register address */

    /* Repeat START condition */
    I2C1_Start();                                                       /* Generate START condition */
    I2C1_SendAddress(slave_address, I2C_READ);                          /* Send slave address with read bit */

    for (i = 0U; i < (length - 1); i++)
    {
        buffer[i] = I2C1_ReadByte_ACK();                                /* Read received byte with ACK */
    }

    buffer[length - 1] = I2C1_ReadByte_NACK();                          /* Read last byte with NACK */
    
    I2C1_Stop();                                                        /* Generate STOP condition */
}

void I2C1_WriteBuffer(uint8_t slave_address, uint8_t register_address, const uint8_t *buffer, uint16_t length)
{
    uint16_t i;

    if ((buffer == NULL) || (length == 0U))
    {
        return;
    }

    I2C1_Start();                                                       /* Generate START condition */
    I2C1_SendAddress(slave_address, I2C_WRITE);                         /* Send slave address with write bit */
    I2C1_WriteByte(register_address);                                   /* Send register address */

    for (i = 0U; i < length; i++)
    {
        I2C1_WriteByte(buffer[i]);                                      /* Send data byte */
    }

    I2C1_Stop();                                                        /* Generate STOP condition */
}
