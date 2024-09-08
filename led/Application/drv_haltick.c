/**
 * @file        drv_haltick.c
 * @author      RobotPilots@2020
 * @Version     V1.0
 * @date        15-September-2020
 * @brief       Haltick driver
 * @update
 *              v1.1(10-October-2021)
 *                  1.通过重写HAL_IncTick()，将需要主动调用micros()才更新
 *                    HalTick数值修改成在中断里面自动更新(次/1ms)，而micros()
 *                    则能在调用时精确计算当前时间(us)。
 */

/**
 * @HalTick TimeBase(TIM2)
 *      选用TIM2作为Hal时基的源
 *      频率:1MHz => 1Tick = 1us
 *      周期:1ms
 */
/* Includes ------------------------------------------------------------------*/
#include "drv_haltick.h"

/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
// Hal时间基准(us)
volatile uint32_t HalTick = 0;
// Hal库全局时间变量(默认1ms更新一次)
extern __IO uint32_t uwTick;
extern HAL_TickFreqTypeDef uwTickFreq;
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/**
 *  @brief 
 *      重写stm32f4xx_hal.c中的HAL_IncTick函数
 *  @note  
 *      使用cubemx生成FREERTOS后会建议将SYS的时基切换成除SysTick之外的定时器
 *	    从而系统会存在两套时基，①用于RTOS的SysTick ②用于HAL的HalTick
 *	    SysTick 使用cortex-m4内核的SysTick (SysTick->VAL会在启动任务调度器之后才更新)
 *	    HalTick 在本工程里面使用TIM2 (TIM2->CNT可提供微妙级延时)
 *	    # delay_us 和 delay_ms 不会引起任务调度(阻塞型)
 */
void HAL_IncTick(void)
{
    // 更新Hal库全局时间变量(ms)
    uwTick += uwTickFreq;
    // 更新当前的Hal时间基准(us)
    HalTick = uwTick*1000 + TIM2->CNT;
}

uint32_t micros(void)
{
    // 更新当前的Hal时间基准(us)
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

