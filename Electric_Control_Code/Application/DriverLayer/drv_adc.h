#ifndef __DRV_ADC_H
#define __DRV_ADC_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Exported variables --------------------------------------------------------*/
#define ADC1_RX_BUF_LEN     2
#define FORCE_K 			(50000/4096)
#define FORCE_k1			1
#define FORCE_b1			0
#define FORCE_k2			1
#define FORCE_b2			0

void Data_Process(int32_t *Force);
extern int32_t Force_Value[ADC1_RX_BUF_LEN];
void ADC1_DMA_Start(void);
extern int32_t Force[2];

#endif
