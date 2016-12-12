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
 * @brief SysTick_Init �δ�ʱ����ʼ��
 */
extern void SysTick_Init(void);

/**
 * @brief QBL_GetTick ��ȡQBL��ʼ������������ʱ��
 * @return ����ʱ�䣬��λms
 */
extern uint32_t QBL_GetTick(void);

/**
 * @brief QBL_Delay �ṩ��ʱ����
 * @param ms ��ʱ��ʱ�䣬��λms
 */
extern void QBL_Delay(uint32_t ms);


/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif // QBL_SYSTICK_H
