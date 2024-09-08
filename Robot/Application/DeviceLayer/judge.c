
/* Includes ------------------------------------------------------------------*/
#include "judge.h"

/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
judge_info_t judge_info = {
	.offline_max_cnt = 500,
};

/* Exported variables --------------------------------------------------------*/
judge_t judge = 
{
	.info = &judge_info,
	.update = judge_update,
	.heart_beat = judge_heart_beat,
	.work_state = DEV_OFFLINE,
};

/* Private functions ---------------------------------------------------------*/
void judge_heart_beat(judge_t *jud_sen)
{
	judge_info_t *jud_info = jud_sen->info;

	jud_info->offline_cnt++;
	if(jud_info->offline_cnt > jud_info->offline_max_cnt) {
		jud_info->offline_cnt = jud_info->offline_max_cnt;
		jud_sen->work_state = DEV_OFFLINE;
	} 
	else {
		/* ÀëÏß->ÔÚÏß */
		if(jud_sen->work_state == DEV_OFFLINE)
		{
			jud_sen->work_state = DEV_ONLINE;
		}
	}
}


/* Exported functions --------------------------------------------------------*/

