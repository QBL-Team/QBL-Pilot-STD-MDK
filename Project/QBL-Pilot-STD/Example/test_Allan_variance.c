#include "stm32f4xx.h"
#include "QBL_Type.h"
#include "QBL_SysTick.h"
#include "QBL_USART1.h"
#include "QBL_I2C.h"
#include "QBL_SPI.h"
#include "QBL_SD.h"

#include "MPU6050.h"
#include "LED.h"

#include "SystemParams.h"

#include "SDcard.h"
#include "ff.h"
#include "diskio.h"

#include <string.h>
#include <stdio.h>

static volatile unsigned char flag_5ms = 0;
static float MPU6050_axis[7];

static char filebuffer[100] = "\n";

static uint32_t nowtime = 0;

#define test_allan

#if defined (SYSTEM_MODE_Debug) && defined (test_allan)
void TIM7_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM7,TIM_IT_Update) == SET){
		
		TIM_ClearITPendingBit(TIM7,TIM_IT_Update); 

		flag_5ms = 1;

		nowtime = QBL_GetTick();

		MPU6050_Read(MPU6050_axis);
	}
}
#endif

static void TIM7_Int_Init(void)
{
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7,ENABLE);

		TIM_TimeBaseInitTypeDef tim;
	
		tim.TIM_Period = 250; //存在两倍的关系
		tim.TIM_Prescaler = 1680;
		tim.TIM_CounterMode = TIM_CounterMode_Up;
		tim.TIM_ClockDivision = TIM_CKD_DIV1;

		TIM_TimeBaseInit(TIM7, &tim);

		TIM_ITConfig(TIM7,TIM_IT_Update,ENABLE);
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

static FIL fdst;
static void SD_Write_init(void)
{
	{
		FRESULT fresult;
		// Mount a logical drive
		static FATFS fs;
		fresult = f_mount(&fs, "", 0);
		if (fresult != FR_OK) {
			printf("mount fail!\n");
		}
	}

	//if file not exist, new file and open
	f_open(&fdst, "Test.txt", FA_WRITE | FA_OPEN_ALWAYS);
}

static uint8_t SD_Write(int count, char FileBuffer[])
{
	UINT bw;

	f_lseek(&fdst, f_size(&fdst));
	f_write(&fdst , FileBuffer, count, &bw);
	f_sync(&fdst);

	return 1;
}

void test_Allan_var_main(void)
{
	SysTick_Init();
	LED_Init();
	LED_Show(LED_COLOR_OFF);
	QBL_USART1_Init();
	QBL_SDIO_Init();
	QBL_I2C_Init();	
	QBL_SPI_Init();
	MPU6050_Init();
    
	SD_Init();
    
	TIM7_Int_Init();
      
	SD_Write_init();
    
	while(1)
	{
		if(flag_5ms == 1){

			flag_5ms = 0;		

			printf("%d\n", QBL_GetTick());

			memset(filebuffer, 0, sizeof(filebuffer));

			sprintf(filebuffer, "%10d,%0.2f,%0.2f,%0.2f,%0.2f,%0.2f,%0.2f\n",
				nowtime,MPU6050_axis[0], MPU6050_axis[1], MPU6050_axis[2], MPU6050_axis[3], MPU6050_axis[4], MPU6050_axis[5]);

			SD_Write(strlen((char *)filebuffer),filebuffer);
		}
	}
}
