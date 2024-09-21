/**
  ******************************************************************************
  * @file           : ave_filter.c\h
  * @brief          : 
  * @note           : 2022-1-21 15:10:59
  ******************************************************************************
  */
	
#ifndef __AVE_FILTER_H
#define __AVE_FILTER_H

#include "stm32f4xx_hal.h"

#define ave_filter_times_max 30

typedef struct
{
	int16_t index;
	float value[ave_filter_times_max];
	float value_ave;
	float filter_times;
}ave_filter_t;

void ave_fil_init(ave_filter_t *ave_fil);
float ave_fil_update(ave_filter_t *ave_fil, float value, uint16_t max);

#endif
