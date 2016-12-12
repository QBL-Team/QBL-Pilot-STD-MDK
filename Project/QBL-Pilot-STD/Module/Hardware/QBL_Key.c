#include "QBL_key.h"
#include "QBL_USART1.h"
#include <stdio.h>

void Key_GPIO_Init(void)
{
	GPIO_InitTypeDef gpio;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);

	gpio.GPIO_Pin=GPIO_Pin_0;
	gpio.GPIO_Mode=GPIO_Mode_IN;
	gpio.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_Init(GPIOE,&gpio);
}

void Key_INT_Init(void)
{
	NVIC_InitTypeDef nvic;
	EXTI_InitTypeDef exti;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);

	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE,EXTI_PinSource0);

	exti.EXTI_Line=EXTI_Line0;
	exti.EXTI_Mode=EXTI_Mode_Interrupt;
	exti.EXTI_Trigger=EXTI_Trigger_Rising;
	exti.EXTI_LineCmd=ENABLE;
	EXTI_Init(&exti);

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);
	nvic.NVIC_IRQChannel=EXTI0_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority=0x00;
	nvic.NVIC_IRQChannelSubPriority=0x02;
	nvic.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&nvic);
}

void Key_init(void)
{
	Key_GPIO_Init();
	Key_INT_Init();
}

static uint8_t flag_key = 0;
void EXTI0_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line0) != RESET){		
		flag_key = 1;
		EXTI_ClearITPendingBit(EXTI_Line0);
	}  
}

bool isKeyPressed(void)
{
    if(flag_key ==  1)
		{
        flag_key = 0;
		    return true;
		}

    return false;
}
