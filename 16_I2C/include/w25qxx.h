#ifndef W25QXX_H
#define W25QXX_H

#include "stm32f407.h"
#include "spi.h"

/*----------------------------------------------------------*/
/* W25Qxx Flash Commands                                    */
/*----------------------------------------------------------*/
#define W25Q_CMD_WRITE_ENABLE          0x06U
#define W25Q_CMD_WRITE_DISABLE         0x04U

#define W25Q_CMD_READ_STATUS_REG1      0x05U
#define W25Q_CMD_READ_STATUS_REG2      0x35U
#define W25Q_CMD_WRITE_STATUS_REG      0x01U

#define W25Q_CMD_READ_DATA             0x03U
#define W25Q_CMD_FAST_READ             0x0BU

#define W25Q_CMD_PAGE_PROGRAM          0x02U

#define W25Q_CMD_SECTOR_ERASE          0x20U      /* 4 KB */
#define W25Q_CMD_BLOCK_ERASE_32K       0x52U
#define W25Q_CMD_BLOCK_ERASE_64K       0xD8U
#define W25Q_CMD_CHIP_ERASE            0xC7U

#define W25Q_CMD_JEDEC_ID              0x9FU

#define W25Q_CMD_POWER_DOWN            0xB9U
#define W25Q_CMD_RELEASE_POWER_DOWN    0xABU

/*----------------------------------------------------------*/
/* Status Register 1 Bits                                   */
/*----------------------------------------------------------*/
#define W25Q_SR1_BUSY_Pos              0U
#define W25Q_SR1_WEL_Pos               1U

/*----------------------------------------------------------*/
/* Flash Memory Organization                                */
/*----------------------------------------------------------*/
#define W25Q_PAGE_SIZE                 256U
#define W25Q_SECTOR_SIZE               4096U
#define W25Q_BLOCK_SIZE_32K            32768U
#define W25Q_BLOCK_SIZE_64K            65536U

/*----------------------------------------------------------*/
/* Function Prototypes                                      */
/*----------------------------------------------------------*/
void SPI1_CS_Select(void);
void SPI1_CS_Deselect(void);

void W25Q_WriteEnable(void);
void W25Q_WriteDisable(void);

uint8_t W25Q_ReadStatusRegister1(void);
void W25Q_WaitWhileBusy(void);

void W25Q_ReadJEDECID(uint8_t *manufacturer, uint8_t *memory_type, uint8_t *capacity);

void W25Q_ReadData(uint32_t address, uint8_t *buffer, uint32_t length);

void W25Q_PageProgram(uint32_t address, const uint8_t *buffer, uint16_t length);

void W25Q_SectorErase(uint32_t address);

void W25Q_ChipErase(void);

#endif
