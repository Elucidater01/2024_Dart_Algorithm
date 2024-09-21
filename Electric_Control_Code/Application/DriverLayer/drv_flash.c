/* Includes ------------------------------------------------------------------*/
#include "drv_flash.h"

#include "string.h"
/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/**
  * @brief  读取flash数据
  * @param  addr  flash中被读取数据的起始地址
  * @param  buf	  存储数据的起始地址
  * @param  len		读取数据长度,以字节为单位
  */
void Flash_ReadData(uint32_t addr, uint32_t *buf, uint16_t len)
{
	memcpy(buf, (void*)addr, len);
}

/**
  * @brief  将数据写入flash,以字节为单位
  * @param  addr  flash中写入数据的起始地址
  * @param  data  被写入数据的起始地址
  * @param  num		写入数据数量
  */
void Flash_WriteByteData(uint32_t addr,uint8_t *data,uint16_t num)
{
	HAL_FLASH_Unlock();
	for(uint16_t i=0;i<num;i++)
	{
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, addr+i, data[i]);
	}
	HAL_FLASH_Lock();
}

/**
  * @brief  将数据写入flash,以半字为单位
  * @param  addr  flash中写入数据的起始地址
  * @param  data  被写入数据的起始地址
  * @param  num		写入数据数量
  */
void Flash_WriteHalfWordData(uint32_t addr,uint16_t *data,uint16_t num)
{
	HAL_FLASH_Unlock();
	for(uint16_t i=0;i<num;i++)
	{
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, addr+2*i, data[i]);
	}
	HAL_FLASH_Lock();
}

/**
  * @brief  将数据写入flash,以字为单位
  * @param  addr  flash中写入数据的起始地址
  * @param  data  被写入数据的起始地址
  * @param  num		写入数据数量
  */
void Flash_WriteWordData(uint32_t addr,uint32_t *data,uint16_t num)
{
	HAL_FLASH_Unlock();
	for(uint16_t i=0;i<num;i++)
	{
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, addr+4*i, data[i]);
	}
	HAL_FLASH_Lock();
}

/**
  * @brief  将数据写入flash,以双字为单位
  * @param  addr  flash中写入数据的起始地址
  * @param  data  被写入数据的起始地址
  * @param  num		写入数据数量
  */
void Flash_WriteDoubleWordData(uint32_t addr,uint64_t *data,uint16_t num)
{
	HAL_FLASH_Unlock();
	for(uint16_t i=0;i<num;i++)
	{
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, addr+8*i, data[i]);
	}
	HAL_FLASH_Lock();
}

/**
  * @brief  擦除扇区(将数据写入flash中原先已有数据的扇区前需要擦除扇区)
  * @param  SectorNum  需要擦除的目标扇区,取值范围为0至11(注意不要占用已经被程序占用了的扇区)
  * @retval uint32_t	sectorError
	*					如果本次flash擦除产生了错误，则发生擦除错误的页面号存储在SectorError中
  */
uint32_t Flash_EraseSector(uint32_t SectorNum)
{
	FLASH_EraseInitTypeDef FLASH_Erase;
	uint32_t sectorError = 0;
	
	HAL_FLASH_Unlock();
	FLASH_Erase.TypeErase = FLASH_TYPEERASE_SECTORS;
	FLASH_Erase.Banks = FLASH_BANK_1;
	FLASH_Erase.Sector = SectorNum;
	FLASH_Erase.NbSectors = 1;
	FLASH_Erase.VoltageRange = FLASH_VOLTAGE_RANGE_3;
	HAL_FLASHEx_Erase(&FLASH_Erase, &sectorError);
	HAL_FLASH_Lock();
	
	return sectorError;
}

/**
  * @brief  擦除扇区11
  */
uint32_t Flash_EraseSector11(void)
{
	uint32_t sectorError;
	
	sectorError = Flash_EraseSector(11);
	
	return sectorError;
}

