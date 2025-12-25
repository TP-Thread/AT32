#ifndef DRV_EFLASH_H
#define DRV_EFLASH_H

#include "main.h"
#include <stdio.h>

#define FLASH_ID 0XEF4018 // W25Q128

#define FLASH_PageSize 256

#define W25X_WriteEnable 0x06
#define W25X_WriteDisable 0x04
#define W25X_ReadStatusReg 0x05
#define W25X_WriteStatusReg 0x01
#define W25X_ReadData 0x03
#define W25X_FastReadData 0x0B
#define W25X_FastReadDual 0x3B
#define W25X_PageProgram 0x02
#define W25X_BlockErase 0xD8
#define W25X_SectorErase 0x20
#define W25X_ChipErase 0xC7
#define W25X_PowerDown 0xB9
#define W25X_ReleasePowerDown 0xAB
#define W25X_DeviceID 0xAB
#define W25X_ManufactDeviceID 0x90
#define W25X_JedecDeviceID 0x9F

#define Dummy_Byte 0xFF

#define SPI1_NSS_H() HAL_GPIO_WritePin(SPI1_NSS_GPIO_Port, SPI1_NSS_Pin, GPIO_PIN_SET)
#define SPI1_NSS_L() HAL_GPIO_WritePin(SPI1_NSS_GPIO_Port, SPI1_NSS_Pin, GPIO_PIN_RESET)

uint8_t SPI_FLASH_Read_Write_Byte(uint8_t data_w);
uint32_t SPI_FLASH_ReadID(void);
uint8_t SPI_FLASH_ByteRead(uint32_t ReadAddr);
void SPI_FLASH_BufferRead(uint8_t *pBuffer, uint32_t ReadAddr, uint16_t BufferSize);

void SPI_FLASH_WaitForWriteEnd(void);
void SPI_FLASH_SectorErase(uint32_t SectorAddr);
void SPI_FLASH_PageWrite(uint8_t *pBuffer, uint32_t WriteAddr, uint16_t BufferSize);
void SPI_FLASH_BufferWrite(uint8_t *pBuffer, uint32_t WriteAddr, uint16_t BufferSize);

#endif /* DRV_EFLASH_H */
