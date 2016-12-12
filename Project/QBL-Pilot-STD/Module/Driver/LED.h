/**
 *\file LED.h
 *\author no1wudi
 */

#ifndef LED_H
#define LED_H


#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup led_driver LED 驱动
 * @{
 */

#include <stdint.h>

/**
 * 颜色描述符
 */

typedef enum {
    LED_COLOR_OFF = 0, /**< 关闭显示 */
    LED_COLOR_WHITE, /**< 显示白色 */
    LED_COLOR_RED, /**< 显示红色 */
    LED_COLOR_GREEN, /**< 显示绿色 */
    LED_COLOR_BLUE, /**< 显示蓝色 */
    LED_COLOR_FUCHSIN, /**< 显示品红 */
    LED_COLOR_YELLOW, /**< 显示黄色 */
    LED_COLOR_CYAN /**< 显示青色 */
}LED_COLOR;

/**
 * @brief LED_Init 初始化硬件
 */
extern void LED_Init(void);

/**
 * @brief LED_Show 驱动LED显示颜色
 * @param LED_Color 将要显示的颜色
 * @see LED_COLOR
 */

extern void LED_Show(uint8_t LED_Color);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif // LED_H
