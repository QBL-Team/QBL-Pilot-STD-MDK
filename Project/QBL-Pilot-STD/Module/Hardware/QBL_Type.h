#ifndef QBL_TYPE_H
#define QBL_TYPE_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus

extern "C" {

#endif


/**
 * 状态量定义
 */
typedef enum {
    QBL_OK = 0, /**< 没有错误发生 */
    QBL_FAIL, /**< 发生错误 */
    QBL_TIMEOUT, /**< 超出限定的执行时间 */
    QBL_BUSY,  /**< 模块正处于忙碌状态 */
    QBL_START_FAILED,  /**< 启动失败 */
    QBL_WRITE_ADDR_FAILED, /**< 写地址失败 */
    QBL_WRITE_FAILED,   /**< 写数据失败 */
    QBL_ADDR_NOT_MATCH, /**< 地址不匹配 */
    QBL_RECEIVE_FAILED, /**< 读取数据失败 */
} QBL_STATUS;


#ifdef __cplusplus

}

#endif

#endif // QBL_H
