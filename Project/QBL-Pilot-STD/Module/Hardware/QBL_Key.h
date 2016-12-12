/**
 *\file QBL_I2C.h
 *\author no1wudi
 */


#ifndef QBL_KEY_H
#define QBL_KEY_H

#include "QBL_Type.h"

#include "stm32f4xx.h"

#ifdef __cplusplus
extern "C" {
#endif
	
#define KEY GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_0)

void Key_init(void);

bool isKeyPressed(void);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif


#endif
