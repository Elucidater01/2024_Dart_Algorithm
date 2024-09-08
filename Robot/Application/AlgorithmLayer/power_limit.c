#include "Power_Limit.h"


/*-祖传功率-*/
void Chassis_Motor_Power_Limit(int16_t *data)
{
	float buffer = judge.info->power_heat_data.chassis_power_buffer;
	float heat_rate, Limit_k, CHAS_LimitOutput, CHAS_TotalOutput;
	
	float OUT_MAX = 0.f;
	
	OUT_MAX = CHAS_SP_MAX_OUT * 4.f;
	
	if(buffer > 60.f)buffer = 60.f;//防止飞坡之后缓冲250J变为正增益系数
	
	Limit_k = buffer / 60.f;
	
	if(buffer < 25.f)
		Limit_k = Limit_k * Limit_k ;// * Limit_k; //3方
	else
		Limit_k = Limit_k;// * str->Limit_k; //平方
	
	if(buffer < 60.f)
		CHAS_LimitOutput = Limit_k * OUT_MAX;
	else 
		CHAS_LimitOutput = OUT_MAX;    
	
	CHAS_TotalOutput = abs(data[0]) + abs(data[1]) + abs(data[2]) + abs(data[3]) ;
	
	heat_rate = CHAS_LimitOutput / CHAS_TotalOutput;
	
  if(CHAS_TotalOutput >= CHAS_LimitOutput)
  {
		for(char i = 0 ; i < 4 ; i++)
		{	
			data[i] = (int16_t)(data[i] * heat_rate);	
		}
	}
}

