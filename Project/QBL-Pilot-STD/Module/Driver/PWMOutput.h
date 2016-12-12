/*!
 *
 * \file   PWMOutput.h
 * \author no1wudi
 */


#ifndef PWMOUTPUT_H
#define PWMOUTPUT_H


/**
  *
  * @defgroup pwm_output PWM输出
  * @{
  */

#include "QBL_Type.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief PWMOutput_Init 初始化PWM输出所用的定时器
 * @return 初始化的状态
 */
extern QBL_STATUS PWMOutput_Init(void);

/**
 * @brief PWMOutput_Apply 应用输出到定时器各个通道上，注意本函数不会做范围检查，只是简单映射，20999对应1ms，41999对应2ms，数值线性映射在此区间内
 * @param Output 元素0-3对应定时器1的通道1-4，元素4-7对应定时器3的通道1-4
 * @return 输出状态
 */

extern QBL_STATUS PWMOutput_Apply(uint16_t Output[8]);


#ifdef __cplusplus
}
#endif

/**
  *@}
  */

#endif // PWMOUTPUT_H
