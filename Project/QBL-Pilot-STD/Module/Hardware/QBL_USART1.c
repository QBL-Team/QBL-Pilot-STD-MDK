#include "stm32f4xx_gpio.h"
#include "stm32f4xx_usart.h"
#include "stm32f4xx_dma.h"
#include "QBL_SysTick.h"
#include "QBL_USART1.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

//=====================================================
#if __GNUC__

#else
int fputc(int ch, FILE* f)
{
	USART1->DR = ch & 0x1FF;
	while (RESET == USART_GetFlagStatus(USART1, USART_FLAG_TC));
	USART_ClearFlag(USART1, USART_FLAG_TC);
	return ch;
}
#endif

#ifdef USE_DMA_USART1_TRANSMIT
#include <stdarg.h>
static uint8_t print_buffer[255];
void DMA_USART1_Printf(const char *format, ...)
{
	uint32_t length;

	va_list args;
	va_start(args, format);
	/* formate the data */
	length = vsnprintf((char*)print_buffer, sizeof(print_buffer), (char*)format, args);
	va_end(args);

	/* transmit the data */
	QBL_USART1_DMA_SendBuffer(print_buffer, length, 2);
}
#endif

//=======================================================
static void QBL_USART1_Config(void)
{
	{
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	}

	{
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_USART1);
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_USART1);
	}

	{
		GPIO_InitTypeDef io;

		io.GPIO_OType = GPIO_OType_PP;
		io.GPIO_PuPd = GPIO_PuPd_UP;
		io.GPIO_Mode = GPIO_Mode_AF;
		io.GPIO_Speed = GPIO_Speed_100MHz;

		io.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;

		GPIO_Init(GPIOB, &io);
	}

	{
		USART_InitTypeDef usart;

		usart.USART_BaudRate = 115200;
		usart.USART_WordLength = USART_WordLength_8b;
		usart.USART_StopBits = USART_StopBits_1;
		usart.USART_Parity = USART_Parity_No ;
		usart.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		usart.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
		USART_Init(USART1, &usart);
		USART_Cmd(USART1, ENABLE);
	}

	USART_ClearFlag(USART1, USART_FLAG_TC);
}

//=======================================================
#define RX_BUF_SIZE 1000
static uint8_t USART1_RxBuffer[RX_BUF_SIZE];
static void QBL_USART1_DMA_Receive_Config(void)
{
	{
		DMA_InitTypeDef dma;

		DMA_DeInit(DMA2_Stream5);
		dma.DMA_Channel = DMA_Channel_4;
		dma.DMA_PeripheralBaseAddr = (uint32_t)(&(USART1->DR));
		dma.DMA_Memory0BaseAddr = (uint32_t)USART1_RxBuffer;
		dma.DMA_DIR = DMA_DIR_PeripheralToMemory;
		dma.DMA_BufferSize = sizeof(USART1_RxBuffer) / 4;
		dma.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
		dma.DMA_MemoryInc = DMA_MemoryInc_Enable;
		dma.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
		dma.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
		dma.DMA_Mode = DMA_Mode_Normal;
		dma.DMA_Priority = DMA_Priority_VeryHigh;
		dma.DMA_FIFOMode = DMA_FIFOMode_Disable;
		dma.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
		dma.DMA_MemoryBurst = DMA_MemoryBurst_Single;
		dma.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
		DMA_Init(DMA2_Stream5, &dma);

		USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);
		DMA_Cmd(DMA2_Stream5, ENABLE);
	}

	{
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

		NVIC_InitTypeDef nvic;
		nvic.NVIC_IRQChannel = DMA2_Stream5_IRQn;
		nvic.NVIC_IRQChannelPreemptionPriority = 0;
		nvic.NVIC_IRQChannelSubPriority = 3;
		nvic.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&nvic);

		DMA_ITConfig(DMA2_Stream5, DMA_IT_TC, ENABLE);
	}
}

/* integral multiple of sizeof(USART1_RxBuffer)/4 */
static uint16_t WriteIndexDMA = 0;
static uint16_t WriteIndex = 0;
static uint16_t ReadIndex = 0;
void DMA2_Stream5_IRQHandler(void)
{
	/* USART1(receive) DMA carry finished */
	if (DMA_GetITStatus(DMA2_Stream5, DMA_IT_TCIF5))
	{
		DMA_ClearITPendingBit(DMA2_Stream5, DMA_IT_TCIF5);

		WriteIndexDMA = WriteIndexDMA + sizeof(USART1_RxBuffer) / 4;
		WriteIndexDMA = WriteIndexDMA % sizeof(USART1_RxBuffer);

		DMA_Cmd(DMA2_Stream5, DISABLE);
		DMA_MemoryTargetConfig(DMA2_Stream5, (uint32_t)(&USART1_RxBuffer[WriteIndexDMA]), DMA_Memory_0);
		DMA_Cmd(DMA2_Stream5, ENABLE);

		/* divide the RxBuffer to 4 parts */
		uint8_t ReadPartNum = ReadIndex / (sizeof(USART1_RxBuffer) / 4);
		uint8_t WritePartNum = WriteIndexDMA / (sizeof(USART1_RxBuffer) / 4);
		/* if WriteIndex will overstep ReadIndex, force to move backward the ReadIndex */
		if((ReadPartNum - WritePartNum + 4) % 4 == 0)
		{
			ReadIndex = (ReadPartNum + 1) % 4 * (sizeof(USART1_RxBuffer) / 4);
		}
	}
}

static uint16_t USART1_DataAvailable(void)
{
	WriteIndex = WriteIndexDMA + (sizeof(USART1_RxBuffer) / 4 - DMA_GetCurrDataCounter(DMA2_Stream5));
	return (WriteIndex - ReadIndex + sizeof(USART1_RxBuffer)) % sizeof(USART1_RxBuffer);
}

uint16_t Fetch_USART1_Data(char *data, uint16_t length, uint8_t ReadDirection, bool flag_moveReadIndex)
{
	uint16_t ValidDataCounts = USART1_DataAvailable();

	if(length > ValidDataCounts)
	{
		length = ValidDataCounts;
	}

	uint16_t i = 0;
	if(ReadDirection==0)/* read front-end valid data */
	{
		for(i=0; i<length; i++)
		{
			data[i] = USART1_RxBuffer[(ReadIndex+i)%sizeof(USART1_RxBuffer)];
		}
	}
	else/* read back-end valid data */
	{
		for(i=0; i<length; i++)
		{
			data[i] = USART1_RxBuffer[(WriteIndex-length+i)%sizeof(USART1_RxBuffer)];
		}
	}

	if(flag_moveReadIndex==true)
	{
		ReadIndex = (ReadIndex + length) % sizeof(USART1_RxBuffer);
	}

	return length;
}

//=======================================================
#ifdef USE_DMA_USART1_TRANSMIT
static void QBL_USART1_DMA_Transmit_Config(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);

	DMA_Cmd(DMA2_Stream7, DISABLE);
	USART_DMACmd(USART1, USART_DMAReq_Tx, DISABLE);

	DMA_InitTypeDef dma;

	DMA_DeInit(DMA2_Stream7);
	dma.DMA_Channel = DMA_Channel_4;
	dma.DMA_PeripheralBaseAddr = (uint32_t)(&(USART1->DR));
	dma.DMA_Memory0BaseAddr = (uint32_t)0;//DMA carry memory
	dma.DMA_DIR = DMA_DIR_MemoryToPeripheral;
	dma.DMA_BufferSize = 0;//DMA carry counts
	dma.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	dma.DMA_MemoryInc = DMA_MemoryInc_Enable;
	dma.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	dma.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	dma.DMA_Mode = DMA_Mode_Normal;
	dma.DMA_Priority = DMA_Priority_VeryHigh;
	dma.DMA_FIFOMode = DMA_FIFOMode_Disable;
	dma.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
	dma.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	dma.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_DeInit(DMA2_Stream7);
	DMA_Init(DMA2_Stream7, &dma);
}

bool QBL_USART1_DMA_SendBuffer(uint8_t* buffer, uint32_t sendSize,  uint16_t TimeOut)
{
	uint32_t max_time = TimeOut + QBL_GetTick();

	DMA_Cmd(DMA2_Stream7, DISABLE);
	DMA_SetCurrDataCounter(DMA2_Stream7, sendSize);
	DMA_MemoryTargetConfig(DMA2_Stream7, (uint32_t)buffer, DMA_Memory_0);

	USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
	DMA_Cmd(DMA2_Stream7, ENABLE);

	while ((DMA_GetCmdStatus(DMA2_Stream7)!= ENABLE)) {
		if (QBL_GetTick() >= max_time) {
			return false;
		}
	}

	/* USART1(transmit) DMA carry finished */
	while (DMA_GetFlagStatus(DMA2_Stream7, DMA_FLAG_TCIF7)==RESET) {
		if (QBL_GetTick() >= max_time) {
			return false;
		}
	}

	/* USART1 transmit finished */
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC)==RESET) {
		if (QBL_GetTick() >= max_time) {
			return false;
		}
	}

	DMA_Cmd(DMA2_Stream7, DISABLE);

	while ((DMA_GetCmdStatus(DMA2_Stream7)!= DISABLE)) {
		if (QBL_GetTick() >= max_time) {
			return false;
		}
	}

	USART_DMACmd(USART1, USART_DMAReq_Tx, DISABLE);

	DMA_ClearFlag(DMA2_Stream7, DMA_FLAG_TCIF7 | DMA_FLAG_FEIF7 | DMA_FLAG_DMEIF7 | \
								   DMA_FLAG_TEIF7 | DMA_FLAG_HTIF7);

	return true;
}
#endif

//=======================================================
void QBL_USART1_Init(void)
{
	QBL_USART1_Config();

#ifdef USE_DMA_USART1_TRANSMIT
	QBL_USART1_DMA_Transmit_Config();
#endif

	QBL_USART1_DMA_Receive_Config();
}
