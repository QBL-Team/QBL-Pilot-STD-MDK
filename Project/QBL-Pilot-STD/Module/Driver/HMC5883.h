/*!
 *
 * \file   HMC5883.h
 * \author no1wudi
 */

#ifndef __HMC5883_H
#define __HMC5883_H

#include "QBL_Type.h"


#ifdef __cplusplus
extern "C" {
#endif

/*!
  *
  * @defgroup hmc5883_driver HMC5883 驱动
  * @{
  */


/*!
 * \brief HMC5883_Init  初始化HMC5883L
 * \return  如果初始化成功，返回true
 */

extern bool HMC5883_Init(void);

/*!
 * \brief HMC5883_Read 读取最新的传感器数据
 * \param axis 数组的长度为3，数组的元素0~2分别对应X,Y,Z三个轴
 * \return 如果读取成功，返回true
 */

extern bool HMC5883_Read(float* imuData);


/*!
 * @}
 */


#ifdef __cplusplus
}
#endif

#endif
