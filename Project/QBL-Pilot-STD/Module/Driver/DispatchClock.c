#include "stm32f4xx.h"
#include "DispatchClock.h"
#include "Tasks.h"
#include "SystemParams.h"

#ifdef SYSTEM_MODE_Release
void TIM7_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM7, TIM_IT_Update) == SET){
		
		TIM_ClearITPendingBit(TIM7, TIM_IT_Update);

		static uint64_t freequency = 0;

		Timer_callback_200hz();

		if(freequency%2 == 0)
		{
			Timer_callback_100hz();
		}

		if(freequency%4 == 0)
		{
			Timer_callback_50hz();
		}

		freequency ++;
	}
}
#endif

void DispatchClockInit_TIM7(void)
{
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7,ENABLE);
		
		TIM_TimeBaseInitTypeDef tim;

		//There are two times of relationship, 5ms
		tim.TIM_Period = 250;
		tim.TIM_Prescaler = 1680;
		tim.TIM_CounterMode = TIM_CounterMode_Up;
		tim.TIM_ClockDivision = TIM_CKD_DIV1;

		TIM_TimeBaseInit(TIM7, &tim);

		TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE);
		TIM_Cmd(TIM7,ENABLE);
	}
	{
		NVIC_InitTypeDef nvic;

		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);

		nvic.NVIC_IRQChannel = TIM7_IRQn;
		nvic.NVIC_IRQChannelPreemptionPriority = 0x01;
		nvic.NVIC_IRQChannelSubPriority = 0x03;
		nvic.NVIC_IRQChannelCmd=ENABLE;
		NVIC_Init(&nvic);
	}
}

