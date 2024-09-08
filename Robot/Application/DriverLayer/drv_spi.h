#ifndef __DRV_SPI_H
#define __DRV_SPI_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "imu_sensor.h"

#include "main.h"
/* Exported types ------------------------------------------------------------*/
extern SPI_HandleTypeDef hspi1;
extern SPI_HandleTypeDef hspi2;
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
void SPI2_Init(void);
void SPI1_Init(void);

#endif
