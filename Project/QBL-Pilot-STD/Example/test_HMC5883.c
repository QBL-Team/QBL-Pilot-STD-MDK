#include "QBL_Type.h"
#include "QBL_SysTick.h"
#include "QBL_USART1.h"
#include "QBL_I2C.h"
#include <stdio.h>
#include "HMC5883.h"


static float HMC5883_axis[3];

int32_t test_HMC5883_main(void)
{
	SysTick_Init();
	QBL_I2C_Init();
	QBL_USART1_Init();

	uint32_t status = HMC5883_Init();
	if (QBL_OK!= status) {
		printf("status=%d\n", status);
		while(1) {;}
	}

	for (;;) {

		QBL_Delay(100);

		status = HMC5883_Read(HMC5883_axis);
		if (status==QBL_OK) {
			printf("HMC5883:%0.2f %0.2f %0.2f\n",
							HMC5883_axis[0],HMC5883_axis[1],HMC5883_axis[2]);
		}else {
			printf("!test\n");
		}

		printf("\n");
	}
}
