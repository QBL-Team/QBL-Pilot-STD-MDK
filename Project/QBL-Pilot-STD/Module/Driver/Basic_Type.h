#ifndef __DRIVER_TYPE_H__
#define __DRIVER_TYPE_H__

#include <stdint.h>

typedef enum
{
	e_ACC = 0,
	e_GYRO = 1,
	e_Temp = 2,
	e_MAG = 3
}Sensor_e;

typedef struct
{
	int16_t X;
	int16_t Y;
	int16_t Z;
}INT16_XYZ_t;

typedef struct
{
	float X;
	float Y;
	float Z;
}FLOAT_XYZ_t;

typedef struct
{
	float YAW;
	float PIT;
	float ROL;
}FLOAT_ANGEL_t;

typedef struct
{
	INT16_XYZ_t Acc;
	INT16_XYZ_t Gyro;
	
}MPU6050Data_t;

#define DataStackSize 15
typedef struct
{
	uint16_t readIndex;
	uint16_t writeIndex;
	uint16_t validNum;
	float data[DataStackSize];
}FLOAT_DataStack;

typedef struct
{
	uint16_t readIndex;
	uint16_t writeIndex;
	uint16_t validNum;
	INT16_XYZ_t data[DataStackSize];
}INT16_XYZ_DataStack;

typedef struct
{
	uint16_t readIndex;
	uint16_t writeIndex;
	uint16_t validNum;
	FLOAT_XYZ_t data[DataStackSize];
}FLOAT_XYZ_DataStack;

float INT16_XYZNorm(const INT16_XYZ_t *input);

void INT16_XYZNormalize(const INT16_XYZ_t *input, FLOAT_XYZ_t *result);

#endif
