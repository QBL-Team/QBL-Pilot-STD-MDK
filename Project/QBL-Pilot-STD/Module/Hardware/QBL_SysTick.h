/**
 *\file QBL_Systick.h
 *\author no1wudi wubo
 */

#ifndef QBL_SYSTICK_H
#define QBL_SYSTICK_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 *
 * @defgroup qbl_systick QBL Systick
 * @{
 */


/**
 * @brief SysTick_Init 滴答定时器初始化
 */
extern void SysTick_Init(void);

/**
 * @brief QBL_GetTick 获取QBL初始化以来经历的时间
 * @return 返回时间，单位ms
 */
extern uint32_t QBL_GetTick(void);

/**
 * @brief QBL_Delay 提供延时功能
 * @param ms 延时的时间，单位ms
 */
extern void QBL_Delay(uint32_t ms);


/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif // QBL_SYSTICK_H
