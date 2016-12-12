#include "QBL_Type.h"
#include "QBL_SysTick.h"
#include "QBL_USART1.h"
#include "QBL_I2C.h"
#include <stdio.h>
#include "MPU6050.h"

static float MPU6050_axis[7];

void test_MPU6050_main(void)
{
	SysTick_Init();
	QBL_I2C_Init();
	QBL_USART1_Init();

	uint32_t status = 0;
	status = MPU6050_Init();

	if (QBL_OK!= status) {
		printf("status=%d\n", status);
		while(1) {;}
	}

	for (;;) {
		QBL_Delay(100);

		status = MPU6050_Read(MPU6050_axis);
		if (status == QBL_OK) {
			printf("MPU6050:%0.2f %0.2f %0.2f %0.2f\n",
							MPU6050_axis[0],MPU6050_axis[1],MPU6050_axis[2],MPU6050_axis[6]);
		}else {
			printf("!error\n");
		}

		printf("\n");
	}
}
