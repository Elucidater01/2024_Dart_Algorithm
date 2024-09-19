/**
  ******************************************************************************
  * @file    motor.h
  * @brief   This file contains all the function prototypes for
  *          the motor.c file
  ******************************************************************************
  * @attention
  ******************************************************************************
  */
#ifndef __MOTOR_H
#define __MOTOR_H

/* Includes ------------------------------------------------------------------*/
#include "rp_config.h"
#include "can_protocol.h"
#include "rm_motor.h"
#include "rm_protocol.h"

/* Exported variables --------------------------------------------------------*/
extern motor_pid_t motor_pid[MOTOR_LIST];
extern rm_motor_t rm_motor[MOTOR_LIST];

/* Exported functions --------------------------------------------------------*/
void motor_list_init(void);
void motor_list_heart_beat(void);
uint8_t motor_list_workstate(void);

#endif
