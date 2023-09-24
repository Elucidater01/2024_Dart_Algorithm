/**
  * @author  Liu heng
  * �������˲�������RoboMaster��̳  
  */
  
#ifndef _KALMAN_H
#define _KALMAN_H

#include "rp_config.h"
typedef struct {
    float X_last; //��һʱ�̵����Ž��  X(k-|k-1)
    float X_mid;  //��ǰʱ�̵�Ԥ����  X(k|k-1)
    float X_now;  //��ǰʱ�̵����Ž��  X(k|k)
    float P_mid;  //��ǰʱ��Ԥ������Э����  P(k|k-1)
    float P_now;  //��ǰʱ�����Ž����Э����  P(k|k)
    float P_last; //��һʱ�����Ž����Э����  P(k-1|k-1)
    float kg;     //kalman����
    float A;      //ϵͳ����
		float B;
    float Q;
    float R;
    float H;
}extKalman_t;

typedef struct {
	extKalman_t Stand_speed_kal;
	extKalman_t Locat_speed_kal;
}kalman_pid_t;

typedef struct kalman_pid_struct{
	kalman_pid_t *info;
	kalman_mode_cnt_t id;
 
	void        (*update)(struct kalman_pid_struct *self,extKalman_t *data);
	void        (*init)(struct kalman_pid_struct * self);
}kalman_pid_struct_t;//���յ�



void KalmanCreate(extKalman_t *p,float T_Q,float T_R);
float KalmanFilter(extKalman_t* p,float dat);
extern kalman_pid_struct_t KAL_Pid[KAL_CNT];
#endif
