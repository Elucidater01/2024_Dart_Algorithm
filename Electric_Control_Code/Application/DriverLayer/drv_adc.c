/* Includes ------------------------------------------------------------------*/
#include "drv_adc.h"

/* Private variables ---------------------------------------------------------*/
extern ADC_HandleTypeDef hadc1;
uint16_t ADC1_DMA_RXBUF[ADC1_RX_BUF_LEN];

/* Exported variables --------------------------------------------------------*/
int32_t Force_Value[] = {0};
int32_t Force[2]={0};

/* Private function ----------------------------------------------------------*/
/**
  * @brief  数据处理，将ADC采集到的数据转换为拉力值
  * @param  无
  * @retval 无
  */
void Data_Process(int32_t *Force)
{
	/*for(int i=0;i<ADC1_RX_BUF_LEN;i++)
	{
		Force_Value[i] = ADC1_DMA_RXBUF[i];
		Force_Value[i] *= FORCE_K;
 	}*/
	Force_Value[0] = ADC1_DMA_RXBUF[0]*FORCE_K;
	Force_Value[1] = ADC1_DMA_RXBUF[1]*FORCE_K;
	Force[0] = Middle_Filter(Force_Value[0]);
	Force[0] = Force[0]*FORCE_k1 + FORCE_b1;
	Force[1] = Middle_Filter(Force_Value[1]);
	Force[1] = Force[1]*FORCE_k2 + FORCE_b2;
}

/**
  * @brief  ADC1转换完成回调函数
  * @param  hadc: ADC句柄
  * @retval 无
  */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	Data_Process(Force);
}

/* Exported functions --------------------------------------------------------*/
/**
  * @brief  ADC1开启DMA，放在任务中1ms采集一次即可
  * @param  无
  * @retval 无
  */
void ADC1_DMA_Start()
{
	HAL_ADC_Start_DMA(&hadc1, (uint32_t *)ADC1_DMA_RXBUF, sizeof(ADC1_DMA_RXBUF));
}
