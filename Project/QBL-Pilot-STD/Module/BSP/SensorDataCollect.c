#include "SensorDataCollect.h"
#include "SensorParam.h"
#include "QBL_FPU_BasicMath.h"
#include "Basic_Type.h"
#include "BasicMath.h"
#include "MPU6050.h"
#include "HMC5883.h"
#include "MS5611.h"
#include <string.h>

static FLOAT_XYZ_DataStack SensorRawDataAcc;
static FLOAT_XYZ_DataStack SensorRawDataGyro;
static FLOAT_XYZ_DataStack SensorRawDataMag;

static FLOAT_XYZ_DataStack SensorDataAcc;
static FLOAT_XYZ_DataStack SensorDataGyro;
static FLOAT_XYZ_DataStack SensorDataMag;

static FLOAT_DataStack SensorDataTemp;

void SensorDataStackClear(void)
{
	memset(&SensorRawDataAcc, 0, sizeof(SensorRawDataAcc));
	memset(&SensorRawDataGyro, 0, sizeof(SensorRawDataGyro));
	memset(&SensorRawDataMag, 0, sizeof(SensorRawDataMag));

	memset(&SensorDataAcc, 0, sizeof(SensorDataAcc));
	memset(&SensorDataGyro, 0, sizeof(SensorDataGyro));
	memset(&SensorDataMag, 0, sizeof(SensorDataMag));

	memset(&SensorDataTemp, 0, sizeof(SensorDataTemp));
}

void PushSensorRawData(FLOAT_XYZ_t *data, Sensor_e sensor)
{
	if(sensor == e_ACC)
	{
		uint8_t writeIndex = SensorRawDataAcc.writeIndex;

		SensorRawDataAcc.data[writeIndex].X = data->X;
		SensorRawDataAcc.data[writeIndex].Y = data->Y;
		SensorRawDataAcc.data[writeIndex].Z = data->Z;

		SensorRawDataAcc.writeIndex ++;

		SensorRawDataAcc.writeIndex = SensorRawDataAcc.writeIndex % DataStackSize;

		SensorRawDataAcc.readIndex = (SensorRawDataAcc.writeIndex - 1 + DataStackSize) % DataStackSize;

		SensorRawDataAcc.validNum++;
		if(SensorRawDataAcc.validNum > DataStackSize)
		{
			SensorRawDataAcc.validNum = DataStackSize;
		}
	}
	if(sensor == e_GYRO)
	{
		uint8_t writeIndex = SensorRawDataGyro.writeIndex;

		SensorRawDataGyro.data[writeIndex].X = data->X;
		SensorRawDataGyro.data[writeIndex].Y = data->Y;
		SensorRawDataGyro.data[writeIndex].Z = data->Z;

		SensorRawDataGyro.writeIndex ++;

		SensorRawDataGyro.writeIndex = SensorRawDataGyro.writeIndex % DataStackSize;

		SensorRawDataGyro.readIndex = (SensorRawDataGyro.writeIndex - 1 + DataStackSize) % DataStackSize;

		SensorRawDataGyro.validNum++;
		if(SensorRawDataGyro.validNum > DataStackSize)
		{
			SensorRawDataGyro.validNum = DataStackSize;
		}
	}
	if(sensor == e_MAG)
	{
		uint8_t writeIndex = SensorRawDataMag.writeIndex;

		SensorRawDataMag.data[writeIndex].X = data->X;
		SensorRawDataMag.data[writeIndex].Y = data->Y;
		SensorRawDataMag.data[writeIndex].Z = data->Z;

		SensorRawDataMag.writeIndex ++;

		SensorRawDataMag.writeIndex = SensorRawDataMag.writeIndex % DataStackSize;

		SensorRawDataMag.readIndex = (SensorRawDataMag.writeIndex - 1 + DataStackSize) % DataStackSize;

		SensorRawDataMag.validNum++;
		if(SensorRawDataMag.validNum > DataStackSize)
		{
			SensorRawDataMag.validNum = DataStackSize;
		}
	}
}

void GetSensorRawData(FLOAT_XYZ_t *data, int8_t Index, Sensor_e sensor)
{
	if(Index > 0)
	{
		while(1);
	}

	if(sensor == e_ACC)
	{
		uint8_t readIndex = (SensorRawDataAcc.readIndex + Index + DataStackSize) % DataStackSize;

		data->X = SensorRawDataAcc.data[readIndex].X;
		data->Y = SensorRawDataAcc.data[readIndex].Y;
		data->Z = SensorRawDataAcc.data[readIndex].Z;
	}
	if(sensor == e_GYRO)
	{
		uint8_t readIndex = (SensorRawDataGyro.readIndex + Index + DataStackSize) % DataStackSize;

		data->X = SensorRawDataGyro.data[readIndex].X;
		data->Y = SensorRawDataGyro.data[readIndex].Y;
		data->Z = SensorRawDataGyro.data[readIndex].Z;
	}
	if(sensor == e_MAG)
	{
		uint8_t readIndex = (SensorRawDataMag.readIndex + Index + DataStackSize) % DataStackSize;

		data->X = SensorRawDataMag.data[readIndex].X;
		data->Y = SensorRawDataMag.data[readIndex].Y;
		data->Z = SensorRawDataMag.data[readIndex].Z;
	}
}

void PushSensorData(FLOAT_XYZ_t *data, Sensor_e sensor)
{
	if(sensor == e_ACC)
	{
		uint8_t writeIndex = SensorDataAcc.writeIndex;

		SensorDataAcc.data[writeIndex].X = data->X;
		SensorDataAcc.data[writeIndex].Y = data->Y;
		SensorDataAcc.data[writeIndex].Z = data->Z;

		SensorDataAcc.writeIndex ++;

		SensorDataAcc.writeIndex = SensorDataAcc.writeIndex % DataStackSize;

		SensorDataAcc.readIndex = (SensorDataAcc.writeIndex - 1 + DataStackSize) % DataStackSize;

		SensorDataAcc.validNum++;
		if(SensorDataAcc.validNum > DataStackSize)
		{
			SensorDataAcc.validNum = DataStackSize;
		}
	}
	if(sensor == e_GYRO)
	{
		uint8_t writeIndex = SensorDataGyro.writeIndex;

		SensorDataGyro.data[writeIndex].X = data->X;
		SensorDataGyro.data[writeIndex].Y = data->Y;
		SensorDataGyro.data[writeIndex].Z = data->Z;

		SensorDataGyro.writeIndex ++;

		SensorDataGyro.writeIndex = SensorDataGyro.writeIndex % DataStackSize;

		SensorDataGyro.readIndex = (SensorDataGyro.writeIndex - 1 + DataStackSize) % DataStackSize;

		SensorDataGyro.validNum++;
		if(SensorDataGyro.validNum > DataStackSize)
		{
			SensorDataGyro.validNum = DataStackSize;
		}
	}
	if(sensor == e_MAG)
	{
		uint8_t writeIndex = SensorDataMag.writeIndex;

		SensorDataMag.data[writeIndex].X = data->X;
		SensorDataMag.data[writeIndex].Y = data->Y;
		SensorDataMag.data[writeIndex].Z = data->Z;

		SensorDataMag.writeIndex ++;

		SensorDataMag.writeIndex = SensorDataMag.writeIndex % DataStackSize;
		
		SensorDataMag.readIndex = (SensorDataMag.writeIndex - 1 + DataStackSize) % DataStackSize;

		SensorDataMag.validNum++;
		if(SensorDataMag.validNum > DataStackSize)
		{
			SensorDataMag.validNum = DataStackSize;
		}
	}
}

void GetSensorData(FLOAT_XYZ_t *data, int8_t Index, Sensor_e sensor)
{
	if(Index > 0)
	{
		while(1);
	}

	if(sensor == e_ACC)
	{
		uint8_t readIndex = (SensorDataAcc.readIndex + Index + DataStackSize) % DataStackSize;

		data->X = SensorDataAcc.data[readIndex].X;
		data->Y = SensorDataAcc.data[readIndex].Y;
		data->Z = SensorDataAcc.data[readIndex].Z;
	}
	if(sensor == e_GYRO)
	{
		uint8_t readIndex = (SensorDataGyro.readIndex + Index + DataStackSize) % DataStackSize;

		data->X = SensorDataGyro.data[readIndex].X;
		data->Y = SensorDataGyro.data[readIndex].Y;
		data->Z = SensorDataGyro.data[readIndex].Z;
	}
	if(sensor == e_MAG)
	{
		uint8_t readIndex = (SensorDataMag.readIndex + Index + DataStackSize) % DataStackSize;

		data->X = SensorDataMag.data[readIndex].X;
		data->Y = SensorDataMag.data[readIndex].Y;
		data->Z = SensorDataMag.data[readIndex].Z;
	}
}

void PushTempData(float *data, Sensor_e sensor)
{
	if(sensor == e_Temp)
	{
		uint8_t writeIndex = SensorDataTemp.writeIndex;

		SensorDataTemp.data[writeIndex] = *data;

		SensorDataTemp.writeIndex ++;

		SensorDataTemp.writeIndex = SensorDataTemp.writeIndex % DataStackSize;

		SensorDataTemp.readIndex = (SensorDataTemp.writeIndex - 1 + DataStackSize) % DataStackSize;

		SensorDataTemp.validNum++;
		if(SensorDataTemp.validNum > DataStackSize)
		{
			SensorDataTemp.validNum = DataStackSize;
		}
	}
}

void GetTempData(float *data, int8_t Index, Sensor_e sensor)
{
	if(Index > 0)
	{
		while(1);
	}

	if(sensor == e_Temp)
	{
		uint8_t readIndex = (SensorDataTemp.readIndex + Index + DataStackSize) % DataStackSize;

		*data = SensorDataTemp.data[readIndex];
	}
}

bool isAccDataValid(FLOAT_XYZ_t *rawdata)
{
	float Gravity = getAccGravity();
	float AccNorm = sqrt_f32(rawdata->X * rawdata->X + rawdata->Y * rawdata->Y + rawdata->Z * rawdata->Z);

	if(AccNorm>2.0f*Gravity || AccNorm<0.5f*Gravity)
	{
		return false;
	}

	return true;
}

#define SlideWindowSize 5
void FilterAndCalibrateAcc(FLOAT_XYZ_t *rawdata, FLOAT_XYZ_t *data)
{
	float AvgAcc[4] = {0,0,0,0};

	/* push new raw data to stack */
	PushSensorRawData(rawdata, e_ACC);

	/* slide window filter */
	if(SensorRawDataAcc.validNum>=5)
	{
		int16_t i;
		for(i=0;i<SlideWindowSize;i++)
		{
			FLOAT_XYZ_t RawAcc_i;
			GetSensorRawData(&RawAcc_i, -i, e_ACC);

			AvgAcc[0] += RawAcc_i.X;
			AvgAcc[1] += RawAcc_i.Y;
			AvgAcc[2] += RawAcc_i.Z;
		}
		AvgAcc[0] /= SlideWindowSize;
		AvgAcc[1] /= SlideWindowSize;
		AvgAcc[2] /= SlideWindowSize;
		AvgAcc[3] = 1;
	}
	else
	{
		FLOAT_XYZ_t RawAcc_i;
		GetSensorRawData(&RawAcc_i, 0, e_ACC);

		AvgAcc[0] = RawAcc_i.X;
		AvgAcc[1] = RawAcc_i.Y;
		AvgAcc[2] = RawAcc_i.Z;
		AvgAcc[3] = 1;
	}

	/* calibrate data */
	const float *AccCaliParam = getAccCalibrateParam();
	MatrixMultiply((float *)AccCaliParam,3,4, AvgAcc,4,1, (float *)data,3,1);

	/* push new calibrated data to stack */
	PushSensorData(data, e_ACC);
}

bool isGyroDataValid(INT16_XYZ_t *rawdata)
{
	return true;
}

void FilterAndCalibrateGyro(FLOAT_XYZ_t *rawdata, FLOAT_XYZ_t *data)
{
	const float *GYRO_Bias = getGyroBias();

	/* push new raw data to stack */
	PushSensorRawData(rawdata, e_GYRO);

	data->X = rawdata->X - GYRO_Bias[0];
	data->Y = rawdata->Y - GYRO_Bias[1];
	data->Z = rawdata->Z - GYRO_Bias[2];

	/* push new calibrated data to stack */
	PushSensorData(data, e_GYRO);
}

bool isMagDataValid(FLOAT_XYZ_t *rawdata)
{
	const float *Ellipse = getMagEllipse();
	const float MagValidThreshold = getMagValidThreshold();

	float error = (float)(Ellipse[0]*rawdata->X*rawdata->X +
	                      Ellipse[1]*rawdata->Y*rawdata->Y +
	                      Ellipse[2]*rawdata->Z*rawdata->Z +
	                      2*Ellipse[3]*rawdata->X*rawdata->Y +
	                      2*Ellipse[4]*rawdata->X*rawdata->Z +
	                      2*Ellipse[5]*rawdata->Y*rawdata->Z +
	                      2*Ellipse[6]*rawdata->X +
	                      2*Ellipse[7]*rawdata->Y +
	                      2*Ellipse[8]*rawdata->Z +
	                      Ellipse[8]);

	if(ABS(error) > MagValidThreshold)
	{
		return false;
	}
	
	return true;
}

void FilterAndCalibrateMag(FLOAT_XYZ_t *rawdata, FLOAT_XYZ_t *data)
{
	const float *MagCaliParam_Matrix = getMagCalibrateMatrix();
	const float *MagCaliParam_Bias = getMagCalibrateBias();

	/* push new raw data to stack */
	PushSensorRawData(rawdata, e_MAG);

	FLOAT_XYZ_t RawMag_i;
	GetSensorRawData(&RawMag_i, 0, e_MAG);
	MatrixSub((float *)(&RawMag_i),3,1, (float *)MagCaliParam_Bias,3,1, (float *)(&RawMag_i),3,1);
	MatrixMultiply((float *)MagCaliParam_Matrix,3,3, (float *)(&RawMag_i),3,1, (float *)data,3,1);

	/* push new calibrated data to stack */
	PushSensorData(data, e_MAG);
}

void FilterAndCalibrateTemp(float *rawdata, float *data)
{
	*data = *rawdata;

	/* push new calibrated data to stack */
	PushTempData(data, e_MAG);
}

