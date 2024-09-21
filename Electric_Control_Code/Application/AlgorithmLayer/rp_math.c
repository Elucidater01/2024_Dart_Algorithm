/**
 * @file        rp_math.c
 * @author      RobotPilots
 * @Version     v1.1
 * @brief       RobotPilots Robots' Math Libaray.
 * @update
 */
 
/* Includes ------------------------------------------------------------------*/
#include "rp_math.h"

/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/**
  * @brief  一阶低通滤波
  * @param  
  * @retval 
  */
float lowpass(float X_last, float X_new, float K)
{
	return (X_last + (X_new - X_last) * K);
}

/**
  * @brief  
  * @param  final: 目标值
  * @param  now: 当前值
  * @param  ramp: 限幅
  * @retval int
  */
int16_t RampInt(int16_t final, int16_t now, int16_t ramp)
{
	int32_t buffer = 0;
	
	buffer = final - now;
	if (buffer > 0)
	{
		if (buffer > ramp)
			now += ramp;
		else
			now += buffer;
	}
	else
	{
		if (buffer < -ramp)
			now += -ramp;
		else
			now += buffer;
	}

	return now;
}

/**
  * @brief  
  * @param  final: 目标值
  * @param  now: 当前值
  * @param  ramp: 限幅
  * @retval float
  */
float RampFloat(float final, float now, float ramp)
{
	float buffer = 0;
	
	buffer = final - now;
	if (buffer > 0)
	{
		if (buffer > ramp)
			now += ramp;
		else
			now += buffer;
	}
	else
	{
		if (buffer < -ramp)
			now += -ramp;
		else
			now += buffer;
	}

	return now;	
}

/**
  * @brief  死区处理
  * @param  input: 输入值
  * @param  center: 中心值
  * @param  death: 死区范围
  * @retval 
  */
float DeathZoom(float input, float center, float death)
{
	if(abs(input - center) < death)
	{
		return center;
	}
	else
	{
		return input;
	}
}
