#include "community_task.h"

void StartCommunityTask(void const * argument)
{
	uint16_t i = 0;
  for(;;)
  {
//	  ADC1_DMA_Start();
	  if (i == 60000)
	  {
		  i = 0;
	  }
	  
	  if(i%3==0)
	  {
		vision_send_data();
	  }
	  i++;
	  Tx_vision_info_update(&vision_sensor);
	  osDelay(1);
  }
}
