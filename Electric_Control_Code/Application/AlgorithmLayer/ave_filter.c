/**
  ******************************************************************************
  * @file           : ave_filter.c\h
  * @brief          : 
  * @note           : 2022-1-21 15:10:55
  ******************************************************************************
  */

#include "ave_filter.h"
#include "string.h"

void ave_fil_init(ave_filter_t *ave_fil)
{
	memset(ave_fil->value,0,ave_filter_times_max);
	ave_fil->value_ave = 0;
	ave_fil->index = 0;
	ave_fil->filter_times = 0;
}

float ave_fil_update(ave_filter_t *ave_fil, float value, uint16_t max)
{
	if(max > ave_filter_times_max)
	{
		max = ave_filter_times_max;
	}
	if(max != ave_fil->filter_times)
	{
		ave_fil_init(ave_fil);
		ave_fil->filter_times = max;
	}
	ave_fil->value_ave -= ave_fil->value[ave_fil->index] / (float)max;
	ave_fil->value[ave_fil->index] = value;
	ave_fil->value_ave += ave_fil->value[ave_fil->index] / (float)max;
	ave_fil->index++;
	ave_fil->index %= max;
	
	return ave_fil->value_ave;
}
