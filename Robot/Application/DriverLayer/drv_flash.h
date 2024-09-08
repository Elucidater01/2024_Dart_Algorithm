#ifndef __DRV_FLASH_H
#define __DRV_FLASH_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
#define IMU_DATA_ADDR	0x080E0000
/* Exported functions --------------------------------------------------------*/
void Flash_ReadData(uint32_t addr, uint32_t *buf, uint16_t len);
void Flash_WriteByteData(uint32_t addr,uint8_t *data,uint16_t num);
void Flash_WriteHalfWordData(uint32_t addr,uint16_t *data,uint16_t num);
void Flash_WriteWordData(uint32_t addr,uint32_t *data,uint16_t num);
void Flash_WriteDoubleWordData(uint32_t addr,uint64_t *data,uint16_t num);
uint32_t Flash_EraseSector(uint32_t SectorNum);
uint32_t Flash_EraseSector11(void);

#endif
