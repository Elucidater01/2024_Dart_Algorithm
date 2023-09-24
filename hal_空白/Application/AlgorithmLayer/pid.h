#ifndef __PID_H
#define __PID_H

/* Includes ------------------------------------------------------------------*/
#include "rp_config.h"

/* Exported macro ------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
void pid_val_init(pid_ctrl_t *pid);
void single_pid_ctrl(pid_ctrl_t *pid);
void cascade_pid_ctrl(pid_ctrl_t *pid);

void balan_locat_ctrl(pid_ctrl_t *pid);//��λ��ר��
void balan_turn_ctrl(pid_ctrl_t *pid);//ת��ר��
#endif
