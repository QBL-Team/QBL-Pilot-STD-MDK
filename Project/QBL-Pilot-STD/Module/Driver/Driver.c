#include "Driver.h"
#include "QBL.h"

void Driver_Init(void)
{
	QBL_Init();
	LED_Init();
	MPU6050_Init();
	HMC5883_Init();
	MS5611_Init();
	W25Q_Init();
	SD_Init();
}


