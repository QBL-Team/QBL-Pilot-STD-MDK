#include "Tasks.h"
#include "DispatchClock.h"
#include "Driver.h"
#include "SDcardWriteAndRead.h"
#include "SensorDataCollect.h"
#include "SensorDataCollectForCali.h"

void systemInitAll(void)
{
	Driver_Init();

	SDcard_reset(1);

	SensorDataStackClear();

	DispatchClockInit_TIM7();
}

void Timer_callback_200hz(void)
{
	CollectAndSaveSensorDataToSDcard();
}

void Timer_callback_100hz(void)
{

}

void Timer_callback_50hz(void)
{

}

