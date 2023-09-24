/**
 * @file        judge_task.c
 * @author      Clumsy_Bird@2021
 * @Version     V1.0
 * @date        27-October-2020
 * @brief       Judgement interact messages send task
 */

/* Includes ------------------------------------------------------------------*/
#include "judge_task.h"
#include "judge_sensor.h"
#include "judge_infantrypotocol.h"
#include "rc_sensor.h"
#include "device.h"
#include "main.h"
#include "cmsis_os.h"
#include "vision_sensor.h"
/* Exported functions --------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
#include "string.h"
//
static char table_char[] = {'0','1','2','3','4','5','6','7','8','9'};
float qqt;
char str_try[7];
void char_type_try(char *str,float number,uint8_t g,uint8_t l)//g代表整数位数
{
	uint8_t i;
	int temp;
	float t2 = 0.0;
	for(i = 0;i<=g+l+2;i++)
	{
		*(str+i) = '0';
	}
	if(number >=0)
	{
		*(str) = '+';
		temp = number/1;
		t2 = number;
	}
	else if(number <0)
	{
		*(str) = '-';
		temp = -number/1;
		t2 = -number;
	}
	for(i=1;i<=g;i++)
	{
		if(temp == 0)
			str[g-i+1] = table_char[0];
		else
			str[g-i+1] = table_char[temp%10];
		temp = temp/10;
	}
	
	*(str+g+1) = '.';
	temp = 0;
	for(i=1;i<=l;i++)
	{
		temp = t2*10;
		str[g+1+i] = table_char[temp%10];
		t2 = t2*10;
	}
	*(str+g+l+2) = '\0';
}
extern char engineer_task_updateflag,engineer_bool_updateflag,engineer_condition_updateflag;
void Startjudge_task(void const * argument)
{
	judge_sensor.init(&judge_sensor);
	vision_sensor.init(&vision_sensor);
	for(;;)
	{
		static int i;
		i++;
		if(i%8 == 1)
		{
//			Client_string_taskstate();
//			engineer_task_updateflag = MODIFY;
		}
		else if(i%8 == 4)
		{

//			Client_engineer_Redgreen();
//				engineer_bool_updateflag = MODIFY;
			
		}
//		 else if(i%8 == 3)
//		{
//			static int j;
//			j++;
//			Client_string_taskstate();
//			if(j%2 == 0)
//				engineer_task_updateflag = MODIFY;
//			else if(j%2 == 1)
//				engineer_task_updateflag = ADD;	
//		}
//		else if(i%8 == 4)
//		{
//				Client_2engineer_Redgreen();
//		}
		if(i%8 == 5)
		{
			radar_runing();
		}
		if(i%30 == 0)
		{
			engineer_bool_updateflag = ADD;
			engineer_task_updateflag = ADD;
		}
		osDelay(30);
	}
}



