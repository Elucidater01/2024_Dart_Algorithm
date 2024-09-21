/**
 * @file        rp_math.h
 * @author      RobotPilots
 * @Version     v1.1
 * @brief       RobotPilots Robots' Math Libaray.
 * @update
 *              v1.0(11-September-2020)
 *              v1.1(13-November-2021)
 *                  1.增加位操作函数
 */

#ifndef __RP_MATH_H
#define __RP_MATH_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Exported macro ------------------------------------------------------------*/
#define ANGLE_TO_RAD 0.01745f

typedef enum jugde_logical_e    //逻辑判断
{
	Flase = 0,
	True  = 1,
		
}jugde_logical_e;
/* Exported types ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/* 位操作函数 */
#define SET_EVENT(EVENT, FLAG)      ((EVENT) |= FLAG)     
#define CLEAR_EVENT(EVENT, FLAG)    ((EVENT) &= ~(FLAG))
#define GET_EVENT(EVENT, FLAG)      ((EVENT) & (FLAG))
/* 数值函数 */
#define constrain(x, min, max)	((x>max)?max:(x<min?min:x))
#define max(a,b)                ((a)>(b) ? (a):(b))
#define min(a,b)                ((a)<(b) ? (a):(b))
#define abs(x) 					((x)>0? (x):(-(x)))
#define one(x)					((x)>0? (1):(-1))
#define sgn(x) 					(((x)>0)?1:((x)<0?-1:0))
#define within_or_not(x, min, max)     (x>max)?Flase:((x<min)?Flase:True)  //在规定范围外返回0

#define angle2rad(ANGLE)         ((ANGLE) * ANGLE_TO_RAD)


/* 斜坡函数 */
int16_t RampInt(int16_t final, int16_t now, int16_t ramp);
float RampFloat(float final, float now, float ramp);
/* 死区函数 */
float DeathZoom(float input, float center, float death);
/* 低通滤波 */
float lowpass(float X_last, float X_new, float K);

#endif

