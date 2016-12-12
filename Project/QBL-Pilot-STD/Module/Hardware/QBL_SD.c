#include "QBL_SD.h"
#include "stm32f4xx_gpio.h"

void QBL_SDIO_Init(void)
{
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_SDIO, ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
	}

	{
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	}

	{
		//SDIO
		GPIO_InitTypeDef io;

		io.GPIO_Mode = GPIO_Mode_AF;
		io.GPIO_OType = GPIO_OType_PP;
		io.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12;
		io.GPIO_PuPd = GPIO_PuPd_NOPULL;
		io.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_Init(GPIOC, &io);

		io.GPIO_Pin = GPIO_Pin_2;
		GPIO_Init(GPIOD, &io);

		GPIO_PinAFConfig(GPIOC, GPIO_PinSource8, GPIO_AF_SDIO);
		GPIO_PinAFConfig(GPIOC, GPIO_PinSource9, GPIO_AF_SDIO);
		GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_SDIO);
		GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_SDIO);
		GPIO_PinAFConfig(GPIOC, GPIO_PinSource12, GPIO_AF_SDIO);
		GPIO_PinAFConfig(GPIOD, GPIO_PinSource2, GPIO_AF_SDIO);
	}
}
