#ifndef __SENSORCOLLECT_H__
#define __SENSORCOLLECT_H__

#include "Basic_Type.h"

/* clear sensor data stack */
void SensorDataStackClear(void);

/* Note:0:current -1:last one -2:penult ... */
/* get sensor data from stack */
void GetSensorRawData(FLOAT_XYZ_t *data, int8_t Index, Sensor_e sensor);

/* Note:0:current -1:last one -2:penult ... */
/* get calibrated data from stack */
void GetSensorData(FLOAT_XYZ_t *data, int8_t Index, Sensor_e sensor);

/* get temperature data from stack */
void GetTempData(float *data, int8_t Index, Sensor_e sensor);

/* filter and calibrate Acc data */
void FilterAndCalibrateAcc(FLOAT_XYZ_t *rawdata, FLOAT_XYZ_t *data);

/* filter and calibrate Gyro data */
void FilterAndCalibrateGyro(FLOAT_XYZ_t *rawdata, FLOAT_XYZ_t *data);

/* filter and calibrate Mag data */
void FilterAndCalibrateMag(FLOAT_XYZ_t *rawdata, FLOAT_XYZ_t *data);

#endif
