/*!
 * \file W25QXX.h
 * \author no1wudi
 */

#ifndef __W25QXX_H
#define __W25QXX_H

#include "QBL_Type.h"

/*!
  *
  * @defgroup w25qxx_driver W25QXX 驱动
  * @{
  */

/*!
 * \brief W25Q_Init 初始化W25Qx系列的闪存芯片
 * \param handle    挂载闪存芯片的SPI总线的句柄
 * \return 如果初始化成功，返回true
 */

extern QBL_STATUS W25Q_Init(void);

/*!
 * \brief W25Q_CheckForIdle 检查闪存芯片是否处于空闲状态，只有空闲状态可以进行操作
 * \return 如果芯片处于空闲状态，返回true
 */


extern QBL_STATUS W25Q_CheckForIdle(void);

/*!
 * \brief W25Q_EraseSector 擦除制定的扇区
 * \param addr  要擦除的扇区的地址
 * \return
 */

extern QBL_STATUS W25Q_EraseSector(uint32_t addr);

/*!
 * \brief W25Q_EraseChp 擦除整个芯片
 * \return 如果指令发送成功，返回true
 */


extern QBL_STATUS W25Q_EraseChip(void);

/*!
 * \brief W25Q_EraseSector 擦除制定的扇区
 * \param addr  要擦除的扇区的地址
 * \return
 */

extern QBL_STATUS W25Q_Write(uint32_t addr,uint8_t * buffer,uint16_t length);

/*!
 * \brief W25Q_Read 从闪存中读取数据
 * \param addr 将要读取数据的起始地址
 * \param buffer 接收数据的缓冲
 * \param length 要读取的数据的长度
 * \return 如果读取成功，返回true
 */

extern QBL_STATUS W25Q_Read(uint32_t addr,uint8_t * buffer,uint16_t length);

/*!
 * @}
 */

#endif
