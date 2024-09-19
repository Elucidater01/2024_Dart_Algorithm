#ifndef __DRV_TIM_H
#define __DRV_TIM_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
void TIM1_Init(void);
void TIM4_Init(void);

/* Servo functions */
void Trigger_Open(void);
void Trigger_Close(void);
void Trigger_Sleep(void);

#endif
