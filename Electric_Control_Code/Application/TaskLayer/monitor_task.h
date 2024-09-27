#ifndef __MONITOR_TASK
#define __MONITOR_TASK

#include "cmsis_os.h"
#include "main.h"
#include "module.h"

extern IWDG_HandleTypeDef hiwdg;

void StartMonitorTask(void const * argument);

#endif
