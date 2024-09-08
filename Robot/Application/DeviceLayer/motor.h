#ifndef __MOTOR_H
#define __MOTOR_H

#include "rp_config.h"
#include "can_protocol.h"
#include "rm_motor.h"
#include "rm_protocol.h"

extern motor_pid_t motor_pid[MOTOR_LIST];
extern rm_motor_t rm_motor[MOTOR_LIST];

#endif
