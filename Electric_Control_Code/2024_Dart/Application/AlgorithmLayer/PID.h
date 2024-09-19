#ifndef __PID_H
#define __PID_H
//#include "pid_conf.h"
typedef struct pid_ctrl {
	float	target;
	float	measure;
	float 	err;
	float 	last_err;
	float	kp;
	float 	ki;
	float 	kd;
	float 	pout;
	float 	iout;
	float 	dout;
	float 	out;
	float	integral;
	float 	integral_max;
	float 	out_max;
} pid_ctrl_t;
void single_pid_ctrl(pid_ctrl_t *pid);

#endif
