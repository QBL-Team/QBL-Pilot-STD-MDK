/**
 *\file QBL_I2C.h
 *\author no1wudi
 */


#ifndef QBL_I2C_H
#define QBL_I2C_H

#include "QBL_Type.h"

#ifdef __cplusplus
extern "C" {
#endif

//#define USE_DMA_I2C1_INTERRUPT
#define USE_DMA_I2C1
//#define USE_BASE_I2C1

/**
 * @defgroup qbl_i2c QBL I2C
 * @{
 */


/**
 * @brief QBL_I2C_Mem_Write 使用I2C总线向设备的寄存器写入值
 * @param QBL_I2C_Base 使用的I2C模块的地址
 * @param DevAddr 设备的识别地址
 * @param RegAddr 设备内寄存器的地址
 * @param Value 要写入此寄存器的值
 * @param Length  要写入的数据大小
 * @param TimeOut 单位为毫秒，允许操作的最大用时，注意最小为1，不可为0
 * @return 指示操作状态
 */
extern QBL_STATUS QBL_I2C_Mem_Write(const uint32_t QBL_I2C_Base,const uint8_t DevAddr,const uint8_t RegAddr,const uint8_t * Value, uint8_t Length, uint16_t TimeOut);

/**
 * @brief QBL_I2C_Reset 复位I2C变量
 */
extern void QBL_I2C_Reset(void);

/**
 * @brief QBL_I2C_DMA_IsBusy 判断上一次I2C是否读取完成
 * @return 指示操作状态
 */
extern bool QBL_I2C_IsBusy(void);

/**
 * @brief QBL_I2C_DMA_IsFinished 判断上一次I2C是否读取完成
 * @return 指示操作状态
 */
extern bool QBL_I2C_LastReadFinished(void);
	
/**
 * @brief QBL_I2C_Mem_Read 使用I2C总线从设备的寄存器读取值
 * @param QBL_I2C_Base 使用的I2C模块的地址
 * @param DevAddr 设备的识别地址
 * @param RegAddr 设备内寄存器的地址
 * @param Value 读取缓冲区
 * @param Length 要读出的数据量
 * @param TimeOut 单位为毫秒，允许操作的最大用时，注意最小为1，不可为0
 * @return 指示操作状态
 */
extern QBL_STATUS QBL_I2C_Mem_Read(const uint32_t QBL_I2C_Base,const uint8_t DevAddr,const uint8_t RegAddr,uint8_t * Value,uint8_t Length,uint8_t TimeOut);


/**
 * @brief QBL_I2C_Write 使用I2C总线向设备写入数据
 * @param QBL_I2C_Base 使用的I2C模块的地址
 * @param DevAddr 设备的识别地址
 * @param Data 要写入的数据
 * @param Length 要写入的数据大小
 * @param TimeOut 单位为毫秒，允许操作的最大用时，注意最小为1，不可为0
 * @return 指示操作状态
 */
extern QBL_STATUS QBL_I2C_Write(const uint32_t QBL_I2C_Base,const uint8_t DevAddr,const uint8_t * Data,uint8_t Length,uint8_t TimeOut);

/**
 * @brief QBL_I2c_Read 使用I2C总线从设备写取数据
 * @param QBL_I2C_Base 使用的I2C模块的地址
 * @param DevAddr 设备的识别地址
 * @param Data 读取数据缓冲
 * @param Length 要写入的数据大小
 * @param TimeOut 单位为毫秒，允许操作的最大用时，注意最小为1，不可为0
 * @return 指示操作状态
 */
extern QBL_STATUS QBL_I2C_Read(const uint32_t QBL_I2C_Base,const uint8_t DevAddr,uint8_t * Data,uint8_t Length,uint8_t TimeOut);


/**
 * @brief QBL_I2C_Init 初始化所使用到的I2C总线的硬件模块
 * @return 初始化状态
 */
extern QBL_STATUS QBL_I2C_Init(void);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif


#endif // QBL_I2C_H
