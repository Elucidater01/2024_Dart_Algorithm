/**
 * @file        drv_haltick.c
 * @author      RobotPilots@2020
 * @Version     V1.0
 * @date        15-September-2020
 * @brief       Haltick driver
 * @update
 *              v1.1(10-October-2021)
 *                  1.ͨ����дHAL_IncTick()������Ҫ��������micros()�Ÿ���
 *                    HalTick��ֵ�޸ĳ����ж������Զ�����(��/1ms)����micros()
 *                    �����ڵ���ʱ��ȷ���㵱ǰʱ��(us)��
 */

/**
 * @HalTick TimeBase(TIM2)
 *      ѡ��TIM2��ΪHalʱ����Դ
 *      Ƶ��:1MHz => 1Tick = 1us
 *      ����:1ms
 */
/* Includes ------------------------------------------------------------------*/
#include "drv_haltick.h"

/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
// Halʱ���׼(us)
volatile uint32_t HalTick = 0;
// Hal��ȫ��ʱ�����(Ĭ��1ms����һ��)
extern __IO uint32_t uwTick;
extern HAL_TickFreqTypeDef uwTickFreq;
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/**
 *  @brief 
 *      ��дstm32f4xx_hal.c�е�HAL_IncTick����
 *  @note  
 *      ʹ��cubemx����FREERTOS��Ὠ�齫SYS��ʱ���л��ɳ�SysTick֮��Ķ�ʱ��
 *	    �Ӷ�ϵͳ���������ʱ����������RTOS��SysTick ������HAL��HalTick
 *	    SysTick ʹ��cortex-m4�ں˵�SysTick (SysTick->VAL�����������������֮��Ÿ���)
 *	    HalTick �ڱ���������ʹ��TIM2 (TIM2->CNT���ṩ΢���ʱ)
 *	    # delay_us �� delay_ms ���������������(������)
 */
void HAL_IncTick(void)
{
    // ����Hal��ȫ��ʱ�����(ms)
    uwTick += uwTickFreq;
    // ���µ�ǰ��Halʱ���׼(us)
    HalTick = uwTick*1000 + TIM2->CNT;
}

uint32_t micros(void)
{
    // ���µ�ǰ��Halʱ���׼(us)
    HalTick = uwTick*1000 + TIM2->CNT;
	return HalTick;
}

void delay_us(uint32_t us)
{
	uint32_t now = micros();
	
	while((micros() - now) < us);
}

void delay_ms(uint32_t ms)
{
	while(ms--)
		delay_us(1000);
}

