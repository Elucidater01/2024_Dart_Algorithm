#ifndef __POWER_LIMIT_H
#define __POWER_LIMIT_H


#include "rp_config.h"
#include "rp_math.h"

#include "judge.h"

#define CHAS_SP_MAX_OUT 8000.f

void Chassis_Motor_Power_Limit(int16_t *data);

#endif

