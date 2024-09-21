#include "pid.h"
#include "rp_math.h"

void single_pid_ctrl(pid_ctrl_t *pid)
{
    // �������ֵ(��Ҫ���������м������)
	//pid->err = err;
    // ����
    pid->integral += pid->err;
    pid->integral = constrain(pid->integral, -pid->integral_max, +pid->integral_max);
    // p i d ��������
    pid->pout = pid->kp * pid->err;
    pid->iout = pid->ki * pid->integral;
    pid->dout = pid->kd * (pid->err - pid->last_err);
    // �ۼ�pid���ֵ
    pid->out = pid->pout + pid->iout + pid->dout;
    pid->out = constrain(pid->out, -pid->out_max, pid->out_max);
    // ��¼�ϴ����ֵ
    pid->last_err = pid->err;
}
