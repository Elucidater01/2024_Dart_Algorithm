/**
 * @file        drv_haltick.h
 * @author      RobotPilots
 * @Version     V1.1
 * @brief       Haltick driver
 * @update
 *              v1.0(15-September-2020)
 *              v1.1(10-October-2021)
 *                  1.通过重写HAL_IncTick()，将需要主动调用micros()才更新
 *                    HalTick数值修改成在中断里面自动更新(次/1ms)，而micros()
 *                    则能在调用时精确计算当前时间(us)。
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
