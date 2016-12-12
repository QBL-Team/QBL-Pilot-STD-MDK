#include "Tasks.h"

#include "DispatchClock.h"
#include "Driver.h"
#include "SDcardWriteAndRead.h"
#include "SensorDataCollect.h"
#include "SensorDataCollectForCali.h"

int main(void)
{
	systemInitAll();
	
	//SDcard_log("hello world!!!!!!");
	while(1)
	{
//		char RxBuffer[12];
//		Fetch_USART1_Data(RxBuffer, 3, 0, false);
//		printf("USART1_RxBuffer:%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n", 
//								RxBuffer[0],RxBuffer[1],RxBuffer[2],
//								RxBuffer[3],RxBuffer[4],RxBuffer[5],
//								RxBuffer[6],RxBuffer[7],RxBuffer[8],
//								RxBuffer[9],RxBuffer[10],RxBuffer[11]);
	}
}
