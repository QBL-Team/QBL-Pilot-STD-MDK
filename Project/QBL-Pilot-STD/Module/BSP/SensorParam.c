#include "SensorParam.h"
#include "Driver.h"

/* Accelermeter calibration Parameter */
static const float AccCaliParam[3*4] = 
	{	1.0124597985515917f,   -0.0025881638042618473f, -0.002183749962832618f, -0.010081584089083578f,
	  0.021452417876964f,    1.0045057360513936f,     0.004553727906133259f,   0.00640184570236102f,
	 -0.011896365138163056f, 0.020021362575261804f,   0.9934400668351213f,    -0.02098946769663934f };
static const float Gravity = 9.914054f;
const float* getAccCalibrateParam(void)
{
	return AccCaliParam;
}

float getAccGravity(void)
{
	return Gravity;
}

/* Gyro calibration Parameter */
static const float GyroBias[3] = {-1.2075138f, -1.2075138f, -1.2075138f};
const float* getGyroBias(void)
{
	return GyroBias;
}

/* Magnetic calibration Parameter */
static const float MagInverseW[3*3] = 
	{0.9948953361522075f,   0.0032258439532671826f, 0.020138717160326478f,
   0.003225843953267139f, 0.9913152835337199f,    -0.007164433972455679f,
   0.02013871716032636f,  -0.007164433972455578f, 1.0144078320339744f };
static const float MagBais[3] = {0.015988603f, -0.075602725f, 0.21653368f};
static const float MagField = 0.5196252f;
static const float MagEllipse[10] = {0.9894145930040497f,
                                     0.9819557879443614f,
                                     1.028629619051589f,
                                     0.006257755654849456f,
                                     0.04040827527969822f,
                                    -0.014293093805740176f,
                                    -0.024096006145747913f,
                                     0.07723341858057009f,
                                    -0.22445962882575465f,
                                    -0.21495987818927098f};
static const float MagValidThreshold = 0.05392726f;
const float* getMagCalibrateMatrix(void)
{
	return MagInverseW;
}

const float* getMagCalibrateBias(void)
{
	return MagBais;
}

float getMagField(void)
{
	return MagField;
}

const float* getMagEllipse(void)
{
	return MagEllipse;
}

float getMagValidThreshold(void)
{
	return MagValidThreshold;
}

