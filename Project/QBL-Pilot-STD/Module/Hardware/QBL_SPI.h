/**
 *\file QBL_SPI.h
 *\author no1wudi
 */

#ifndef QBL_SPI_H
#define QBL_SPI_H

#include "QBL_Type.h"


#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup qbl_spi QBL SPI
 * @{
 */

extern QBL_STATUS QBL_SPI_Init(void);

/**
 * @brief QBL_SPI_TransmitReceive 通过SPI模块交换数据
 * @param QBL_SPI_Base SPI模块的地址
 * @param Trans 发送数据的缓冲，若仅接收，参数可设置为NULL
 * @param Rece 接收数据的缓冲区，若仅发送数据，参数可设置为NULL
 * @param Length 交换数据的长度
 * @param TimeOut 允许操作的最长时间
 * @return 总线操作状态
 */
extern QBL_STATUS QBL_SPI_TransmitReceive(const uint32_t QBL_SPI_Base, const uint8_t* Trans, uint8_t* Rece, uint8_t Length, uint16_t TimeOut);


/**
 * @}
 */

#ifdef __cplusplus
}
#endif


#endif // QBL_SPI_H
