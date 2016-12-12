#include "QBL_Type.h"
#include "QBL_SysTick.h"
#include "QBL_USART1.h"
#include "QBL_SPI.h"
#include <stdio.h>
#include "MS5611.h"

static float MS5611[2];

int test_MS5611_main(void)
{
	SysTick_Init();
    QBL_SPI_Init();
	QBL_USART1_Init();

	uint32_t status = MS5611_Init();
	if (QBL_OK!= status) {
		printf("status=%d\n", status);
		while(1) {;}
	}

	for (;;) {

			QBL_Delay(100);

	bool ms5611_status = MS5611_Read(MS5611);
	if (ms5611_status == true) {
		printf("MS5611:%0.2f %0.2f\n",
						MS5611[0], MS5611[1]);
	}else {
		printf("ms5611_test\n");
	}
	
	printf("\n");
	}
}
