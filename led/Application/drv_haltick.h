/**
 * @file        drv_haltick.h
 * @author      RobotPilots
 * @Version     V1.1
 * @brief       Haltick driver
 * @update
 *              v1.0(15-September-2020)
 *              v1.1(10-October-2021)
 *                  1.ͨ����дHAL_IncTick()������Ҫ��������micros()�Ÿ���
 *                    HalTick��ֵ�޸ĳ����ж������Զ�����(��/1ms)����micros()
 *                    �����ڵ���ʱ��ȷ���㵱ǰʱ��(us)��
 */
#ifndef __DRV_HALTICK_H
#define __DRV_HALTICK_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Exported macro ------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
uint32_t micros(void);
void delay_us(uint32_t us);
void delay_ms(uint32_t ms);

#endif
