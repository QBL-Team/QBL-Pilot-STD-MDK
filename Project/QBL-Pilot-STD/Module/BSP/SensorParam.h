#ifndef __SENSORPARAM_H__
#define __SENSORPARAM_H__

const float* getAccCalibrateParam(void);

float getAccGravity(void);

const float* getGyroBias(void);

const float* getMagCalibrateMatrix(void);

const float* getMagCalibrateBias(void);

float getMagField(void);

const float* getMagEllipse(void);

float getMagValidThreshold(void);

#endif
