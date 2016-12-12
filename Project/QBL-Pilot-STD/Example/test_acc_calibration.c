#include "QBL.h"
#include "QBL_Key.h"
#include "QBL_SD.h"
#include "QBL_USART1.h"
#include "MPU6050.h"
#include "HMC5883.h"
#include "MS5611.h"
#include "SDcard.h"
#include "LED.h"

#include "ff.h"
#include "diskio.h"
#include "SystemParams.h"

#include <string.h>

static volatile unsigned char flag_5ms = 0;
static float MPU6050_axis[7];
static float HMC5883_axis[3];
static float MS5611[2];

static char filebuffer[100] = "\n";

static uint32_t nowtime = 0;

//#define test_acc_cali

#if defined (SYSTEM_MODE_Debug) && defined (test_acc_cali)
void TIM7_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM7,TIM_IT_Update) == SET){
		
		TIM_ClearITPendingBit(TIM7,TIM_IT_Update); 

		flag_5ms = 1;
		
		nowtime = QBL_GetTick();

		HMC5883_Read(HMC5883_axis);

		MS5611_Read(MS5611);
		
		MPU6050_Read(MPU6050_axis);
	}
}
#endif

static void TIM7_Int_Init(void)
{
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7,ENABLE);
		
		TIM_TimeBaseInitTypeDef tim;
	
		tim.TIM_Period = 250; //存在两倍的关系，5ms
		tim.TIM_Prescaler = 1680;
		tim.TIM_CounterMode=TIM_CounterMode_Up;
		tim.TIM_ClockDivision=TIM_CKD_DIV1;
		
		TIM_TimeBaseInit(TIM7,&tim);
		
		TIM_ITConfig(TIM7,TIM_IT_Update,ENABLE);
		TIM_Cmd(TIM7,ENABLE);
	}
	{
		NVIC_InitTypeDef nvic;
		
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);
		
		nvic.NVIC_IRQChannel = TIM7_IRQn;
		nvic.NVIC_IRQChannelPreemptionPriority=0x01;
		nvic.NVIC_IRQChannelSubPriority = 0x03;
		nvic.NVIC_IRQChannelCmd=ENABLE;
		NVIC_Init(&nvic);
	}
}

static FIL fdst;
static uint8_t SD_Write_init(void)
{
	UINT bw;
	BYTE textFileBuffer[] = "\nQBL SD card experiment\n";

	{
		static FATFS fs;
		FRESULT fresult;
		fresult = f_mount(&fs, "", 0);// Mount a logical drive
		if (fresult != FR_OK) {
			printf("mount fail!\n");
			return 0;
		}
	}
	{
		static int res;
		res = f_open(&fdst, "Test.txt", FA_WRITE | FA_OPEN_EXISTING);//open existed file
		if(res != FR_OK) {
			res = f_open(&fdst, "Test.txt", FA_WRITE | FA_CREATE_NEW);//new file
		}

		if (res == FR_OK) {
			res = f_write(&fdst , textFileBuffer, sizeof(textFileBuffer), &bw);
			res = f_lseek(&fdst, f_size(&fdst));//move the read/write pointer
			f_sync(&fdst);  // sync 等价与close，区别就是没有关闭文件
		}else {
			printf("make the file fail!\n");
			return 0;
		}
	}

	return 1;
}

static void SD_Write(int count,char FileBuffer[])
{
	UINT bw;
	static int res;
	
	res = f_lseek(&fdst, f_size(&fdst));
	res = f_write(&fdst, FileBuffer, count, &bw);
	res = f_sync(&fdst);
	
	res = res;
}

void test_acc_calibr_main(void)
{
	SysTick_Init();
	LED_Init();
	LED_Show(LED_COLOR_OFF);
	QBL_USART1_Init();
	QBL_SDIO_Init();
	QBL_I2C_Init();	
	QBL_SPI_Init();
	MPU6050_Init();
	MS5611_Init();
	HMC5883_Init();
	SD_Init();
	Key_init();
	TIM7_Int_Init();
	SD_Write_init(); // SD初始化，加载分区和打开或创建文件
	
	while(1)
	{
      static uint8_t flag_once = 0;
      if(isKeyPressed() || flag_once==1)
      {
			    flag_once = 1;
		      if(flag_5ms == 1)
		      {
				      flag_5ms = 0;		

				      printf("%d %0.2f %0.2f\n",QBL_GetTick(),HMC5883_axis[0],MS5611[0]);

				      sprintf(filebuffer,"\r\n%8d %0.2f %0.2f %0.2f ",
					       nowtime,MPU6050_axis[0],MPU6050_axis[1],MPU6050_axis[2]);

				      SD_Write(strlen(filebuffer),filebuffer);
		      }
      }
	}
}
