/**
 *\file QBL.c
 *\author no1wudi
 */

#include "QBL.h"


void QBL_Init(void)
{
	RCC_ClocksTypeDef RCC_Clocks_test;
	RCC_GetClocksFreq(&RCC_Clocks_test);
	if(RCC_Clocks_test.SYSCLK_Frequency != 168000000)
	{
		while(1);
	}

	SysTick_Init();

	QBL_SDIO_Init();
	QBL_I2C_Init();
	QBL_SPI_Init();
	QBL_USART1_Init();
}

