#ifndef __DRIVER_H
#define __DRIVER_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "stdbool.h"

#include "drv_can.h"
#include "drv_flash.h"
#include "drv_gpio.h"
#include "drv_spi.h"
#include "drv_tick.h"
#include "drv_tim.h"
#include "drv_uart.h"

/* Exported macro ------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
void DRIVER_Init(void);

#endif
