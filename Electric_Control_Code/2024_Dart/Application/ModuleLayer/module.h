/**
 ******************************************************************************
 * @file        module.h
 * @brief       Module Status Control
 * @update
 ******************************************************************************
 */
#ifndef __MODULE_H
#define __MODULE_H

/* Includes ------------------------------------------------------------------*/
#include "device.h"
#include "launcher.h"
#include "axis.h"
/* Exported macro ------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/

/**
 * @brief  模块总控制结构体
 */
typedef struct
{
	launcher_t *launcher;
	axis_t     *axis;

} module_t;

/* Exported variables --------------------------------------------------------*/
extern module_t module;

/* Exported functions --------------------------------------------------------*/
void State_Update(referee_data_t *referee_data, launcher_t *launcher);
void Tx_vision_info_update(vision_sensor_t *vision_sensor);
void mode_select(module_t *module);

#endif
