/*!
 *
 * \file   MPU6050.h
 * \author no1wudi
 */

#ifndef __MPU6050_H
#define __MPU6050_H


/*!
  *
  * @defgroup mpu6050_driver MPU6050 驱动
  * @{
  */

#include "QBL_Type.h"


#ifdef __cplusplus
extern "C" {
#endif


/*!
 * \brief MPU6050_Init 初始化传感器
 * \return  如果初始化成功，返回true
 */
extern bool MPU6050_Init(void);



/*!
 * \brief MPU6050_Read    从传感器中读取最新的数据
 * \param axis  本数组长度为6，元素0~2对应于加速度计的X,Y,Z三个轴；元素3~5对应于陀螺仪的X,Y,Z三个轴
 * \param temp 读取传感器的温度，如果不需要，将本参数设置为NULL
 * \return  如果初始化成功，返回true
 */
extern bool MPU6050_Read(float* imuData);


#ifdef __cplusplus
}
#endif

/*!
 * @}
 */

#endif
