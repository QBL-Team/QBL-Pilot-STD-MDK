/*!
 * \file MS5611.h
 * \author no1wudi
 */

#ifndef __MS5611_H
#define __MS5611_H

#include "QBL_Type.h"


#ifdef __cplusplus
extern "C" {
#endif

/*!
  *
  * @defgroup ms5611_driver MS5611 驱动
  * @{
  */


/*!
 * \brief MS5611_Init 初始化传感器，并读取出厂预设值，为计算做准备
 * \return  如果初始化成功，返回true
 */

extern QBL_STATUS MS5611_Init(void);

/*!
 * \brief MS5611_Read 尝试读取最新的气压和温度数据
 * \param Pressure  气压值的指针
 * \param Temperature   温度值的指针
 * \return  仅当返回true时读取到的数据有效，请务必检查返回值
 */

extern bool MS5611_Read(float* Pressure);

/*!
 * @}
 */


#ifdef __cplusplus
}
#endif

#endif
