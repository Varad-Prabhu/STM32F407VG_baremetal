#include "w25qxx.h"
#include "gpio.h"

void SPI1_CS_Select(void)
{
    GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);           /* Set CS low to select the W25Qxx flash */    
}

void SPI1_CS_Deselect(void)
{
    GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);             /* Set CS high to deselect the W25Qxx flash */    
}

void W25Q_WriteEnable(void)
{
    SPI1_CS_Select();                                           /* Select the W25Qxx flash */
    SPI1_Transmit(W25Q_CMD_WRITE_ENABLE);                       /* Send Write Enable command */
    SPI1_CS_Deselect();                                         /* Deselect the W25Qxx flash */
}

void W25Q_WriteDisable(void)
{
    SPI1_CS_Select();                                           /* Select the W25Qxx flash */
    SPI1_Transmit(W25Q_CMD_WRITE_DISABLE);                      /* Send Write Disable command */
    SPI1_CS_Deselect();                                         /* Deselect the W25Qxx flash */
}

uint8_t W25Q_ReadStatusRegister1(void)
{
    uint8_t status;

    SPI1_CS_Select();                                           /* Select the W25Qxx flash */
    SPI1_Transmit(W25Q_CMD_READ_STATUS_REG1);                   /* Send Read Status Register 1 command */
    status = SPI1_Receive();                                    /* Receive the status register value */
    SPI1_CS_Deselect();                                         /* Deselect the W25Qxx flash */

    return status;
}

void W25Q_WaitWhileBusy(void)
{
    while (W25Q_ReadStatusRegister1() & (1U << W25Q_SR1_BUSY_Pos))
    {
        /* Wait until the W25Qxx flash is busy */
    }
}

void W25Q_ReadJEDECID(uint8_t *manufacturer, uint8_t *memory_type, uint8_t *capacity)
{
    SPI1_CS_Select();                                           /* Select the W25Qxx flash */
    SPI1_Transmit(W25Q_CMD_JEDEC_ID);                           /* Send JEDEC ID command */
    *manufacturer = SPI1_TransmitReceive(0xFFU);                /* Receive manufacturer ID */
    *memory_type = SPI1_TransmitReceive(0xFFU);                 /* Receive memory type */
    *capacity = SPI1_TransmitReceive(0xFFU);                    /* Receive capacity */
    SPI1_CS_Deselect();                                         /* Deselect the W25Qxx flash */
}

void W25Q_ReadData(uint32_t address, uint8_t *buffer, uint32_t length)
{
    SPI1_CS_Select();                                           /* Select the W25Qxx flash */
    SPI1_Transmit(W25Q_CMD_READ_DATA);                          /* Send Read Data command */
    SPI1_Transmit((address >> 16) & 0xFFU);                     /* Send address byte 1 */
    SPI1_Transmit((address >> 8) & 0xFFU);                      /* Send address byte 2 */
    SPI1_Transmit(address & 0xFFU);                             /* Send address byte 3 */
    for (uint32_t i = 0; i < length; i++)
    {
        buffer[i] = SPI1_TransmitReceive(0xFFU);                /* Receive data byte */
    }
    SPI1_CS_Deselect();                                         /* Deselect the W25Qxx flash */
}

void W25Q_PageProgram(uint32_t address, const uint8_t *buffer, uint16_t length)
{
    if (length > 256U)
    {
        length = 256U;
    }
    W25Q_WriteEnable();                                         /* Enable write operation */
    SPI1_CS_Select();                                           /* Select the W25Qxx flash */
    SPI1_Transmit(W25Q_CMD_PAGE_PROGRAM);                       /* Send Page Program command */
    SPI1_Transmit((address >> 16) & 0xFFU);                     /* Send address byte 1 */
    SPI1_Transmit((address >> 8) & 0xFFU);                      /* Send address byte 2 */
    SPI1_Transmit(address & 0xFFU);                             /* Send address byte 3 */
    for (uint16_t i = 0; i < length; i++)
    {
        SPI1_Transmit(buffer[i]);                               /* Send data byte */
    }
    SPI1_CS_Deselect();                                         /* Deselect the W25Qxx flash */
    W25Q_WaitWhileBusy();                                       /* Wait until the write operation is complete */
}

void W25Q_SectorErase(uint32_t address)
{
    W25Q_WriteEnable();                                         /* Enable write operation */
    SPI1_CS_Select();                                           /* Select the W25Qxx flash */
    SPI1_Transmit(W25Q_CMD_SECTOR_ERASE);                       /* Send Sector Erase command */
    SPI1_Transmit((address >> 16) & 0xFFU);                     /* Send address byte 1 */
    SPI1_Transmit((address >> 8) & 0xFFU);                      /* Send address byte 2 */
    SPI1_Transmit(address & 0xFFU);                             /* Send address byte 3 */
    SPI1_CS_Deselect();                                         /* Deselect the W25Qxx flash */
    W25Q_WaitWhileBusy();                                       /* Wait until the erase operation is complete */
}

void W25Q_ChipErase(void)
{
    W25Q_WriteEnable();                                         /* Enable write operation */
    SPI1_CS_Select();                                           /* Select the W25Qxx flash */
    SPI1_Transmit(W25Q_CMD_CHIP_ERASE);                         /* Send Chip Erase command */
    SPI1_CS_Deselect();                                         /* Deselect the W25Qxx flash */
    W25Q_WaitWhileBusy();                                       /* Wait until the erase operation is complete */
}
