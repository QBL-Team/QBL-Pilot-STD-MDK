#include "QBL.h"
#include "LED.h"

void test_LED_main(void)
{
	SysTick_Init();
	LED_Init();
	
	while(1)
	{
		LED_Show(LED_COLOR_BLUE);
		QBL_Delay(500);
		LED_Show(LED_COLOR_OFF);
		QBL_Delay(500);	
	}

}
