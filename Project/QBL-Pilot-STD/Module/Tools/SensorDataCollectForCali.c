#include "SensorDataCollectForCali.h"
#include "QBL.h"
#include "Driver.h"

#include <string.h>

static bool CollectAndSaveGyroToSDcard(void)
{
	static uint8_t open_flag = 0;
	char filename[100] = "GyroDataForCali.txt";

	FIL fdst;

	if(open_flag==0)
	{
		open_flag = 1;

		/* FA_OPEN_ALWAYS:if file not exist, new the file. */
		f_open(&fdst, filename, FA_WRITE | FA_OPEN_ALWAYS);
	}
	else
	{
		float MPU6050_axis[7];
		MPU6050_Read(MPU6050_axis);

		uint32_t nowtime = QBL_GetTick();

		char filebuffer[100] = "\n";
		sprintf(filebuffer, "\r\n%10d,%0.2f,%0.2f,%0.2f",
				nowtime, MPU6050_axis[3], MPU6050_axis[4], MPU6050_axis[5]);

		UINT bw;
		f_lseek(&fdst, f_size(&fdst));
		f_write(&fdst, filebuffer, strlen(filebuffer), &bw);
		f_sync(&fdst);

		if (f_size(&fdst) > 10000000)
		{
			f_close(&fdst);
			return false;
		}
	}

	return true;
}

static bool CollectAndSaveAccToSDcard(uint8_t axis)
{
	static uint8_t open_flag = 0;
	char filename[100];

	FIL fdst;

	static int8_t axis_last = -1;
	if(axis != axis_last)
	{
		if(axis_last!=-1)
		{
			/* close last file */
			f_close(&fdst);
		}

		axis_last = axis;
		open_flag = 0;
	}

	if(open_flag==0)
	{
		open_flag = 1;

		sprintf(filename, "AccDataForCali%d.txt", axis);
		/* FA_OPEN_ALWAYS:if file not exist, new the file. */
		f_open(&fdst, filename, FA_WRITE | FA_OPEN_ALWAYS);
	}
	else
	{
		float MPU6050_axis[7];
		MPU6050_Read(MPU6050_axis);

		uint32_t nowtime = QBL_GetTick();

		char filebuffer[100] = "\n";
		sprintf(filebuffer, "\r\n%10d,%f,%f,%f",
				nowtime, MPU6050_axis[0], MPU6050_axis[1], MPU6050_axis[2]);

		UINT bw;
		f_lseek(&fdst, f_size(&fdst));
		f_write(&fdst, filebuffer, strlen(filebuffer), &bw);
		f_sync(&fdst);


		if (f_size(&fdst) > 10000000)
		{
			f_close(&fdst);
			return false;
		}
	}

	return true;
}

static bool CollectAndSaveMagToSDcard(void)
{
	static uint8_t open_flag = 0;
	char filename[100] = "MagDataForCali.txt";

	FIL fdst;

	if(open_flag==0)
	{
		open_flag = 1;

		//FA_OPEN_ALWAYS:if file not exist, new the file.
		f_open(&fdst, filename, FA_WRITE | FA_OPEN_ALWAYS);
	}
	else
	{
		float HMC5883_axis[3];
		HMC5883_Read(HMC5883_axis);

		uint32_t nowtime = QBL_GetTick();

		char filebuffer[100] = "\n";
		sprintf(filebuffer, "\r\n%10d,%0.2f,%0.2f,%0.2f",
				nowtime, HMC5883_axis[0], HMC5883_axis[1], HMC5883_axis[2]);

		UINT bw;
		f_lseek(&fdst, f_size(&fdst));
		f_write(&fdst, filebuffer, strlen(filebuffer), &bw);
		f_sync(&fdst);

		if (f_size(&fdst) > 10000000)
		{
			f_close(&fdst);
			return false;
		}
	}

	return true;
}

void CollectAndSaveSensorDataToSDcard(void)
{
	char RxBuffer[2] = {0, 0};
	Fetch_USART1_Data(RxBuffer, 2, 1, false);

	/* G? */
	if(RxBuffer[0]=='G')
	{
		CollectAndSaveGyroToSDcard();
		printf("collecting Gyro data\n");
	}

	/* A0 A1 A2 A3 A4 A5 */
	if(RxBuffer[0]=='A')
	{
		if(RxBuffer[1]>='0' && RxBuffer[1]<='5')
		{
			CollectAndSaveAccToSDcard(RxBuffer[1] - '0');
			printf("collecting Acc%d data\n", RxBuffer[1] - '0');
		}
	}

	/* M? */
	if(RxBuffer[0]=='M')
	{
		CollectAndSaveMagToSDcard();
		printf("collecting Mag data\n");
	}
}

