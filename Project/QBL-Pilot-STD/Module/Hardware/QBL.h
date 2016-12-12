/**
 *\file QBL.h
 *\author no1wudi
 */

/**
 *
 * @defgroup qbl QBL Common
 * @{
 */

#ifndef QBL_H
#define QBL_H

#include "QBL_Type.h"
#include "QBL_SysTick.h"
#include "QBL_USART1.h"
#include "QBL_I2C.h"
#include "QBL_SPI.h"
#include "QBL_SD.h"
#include "QBL_Key.h"
#include "QBL_FPU_BasicMath.h"
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief QBL_Init 初始化QBL库底层所必须的资源
 */
extern void QBL_Init(void);



#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif // QBL_H
