/*!
 * \file W25QXX.c
 * \author no1wudi
 */

#include "W25QXX.h"
#include "QBL_SPI.h"
#include "stm32f4xx_gpio.h"
#include <stdbool.h>

/*!
  *
  * @ingroup w25qxx_driver W25QXX 驱动
  * @{
  */

/*!
 *
 * @defgroup w25qxx_driver_private W25QXX 驱动私有数据
 * @{
 */

#define W25QXX_CMD_WRITE_ENABLE 0x06 ///< 写使能指令
#define W25QXX_CMD_WRITE_DISABLE 0x04 ///< 写失能指令
#define W25QXX_CMD_READ_STATUS_1 0x05 ///< 读取状态寄存器1
#define W25QXX_CMD_CHIP_EARSE 0xC7 ///< 擦出整个芯片
#define W25QXX_CMD_POWER_DOWN 0xB9 ///< 掉电指令
#define W25QXX_CMD_PROGRAM 0x02 ///< 写数据指令，每次最大写入256字节
#define W25QXX_CMD_SECTOR_ERASE 0x20 ///< 扇区擦除指令
#define W25QXX_CMD_BLOCK_ERASE 0xD8 ///< 块区擦除指令
#define W25QXX_CMD_READ 0x03 ///< 读数据指令
#define W25QXX_CMD_READ_ID 0x9F ///< 读ID指令

#define W25QXX_BUSY_MASK 0x01 ///< 忙碌位掩码
#define W25QXX_WRITE_ENABLE_MASK 0x02 ///< 写使能掩码

#define W25QXX_CHIP_WINBOND_ID 0xEF ///< 华邦芯片的ID

#define W25QXX_CS_ON() GPIO_ResetBits(GPIOE, GPIO_Pin_10) ///< 片选使能
#define W25QXX_CS_OFF() GPIO_SetBits(GPIOE, GPIO_Pin_10) ///< 片选失能

#define W25Q_SPI_BASE SPI1_BASE ///< 使用的SPI端口

/*!
 * \brief W25Q_GetStatus  读取状态寄存器1
 * \return 返回当前状态寄存器的值
 */
static uint8_t W25Q_GetStatus(void)
{
    uint8_t tmp[2];
    tmp[0] = W25QXX_CMD_READ_STATUS_1;
    W25QXX_CS_ON();
    QBL_SPI_TransmitReceive(W25Q_SPI_BASE, tmp, tmp, 2, 5);
    W25QXX_CS_OFF();
    return tmp[1];
}

/*!
 * \brief W25Q_SetWriteEnable   使能芯片的写入功能
 * \return  如果使能成功，返回true
 */
static bool W25Q_SetWriteEnable(void)
{
    uint8_t retry = 15;
    uint8_t tmp = W25QXX_CMD_WRITE_ENABLE;

    while (retry--) {
        if (W25Q_GetStatus() & W25QXX_WRITE_ENABLE_MASK) {
            return true;
        }
        else {
            W25QXX_CS_ON();

            QBL_SPI_TransmitReceive(W25Q_SPI_BASE, &tmp, NULL, 1, 2);

            W25QXX_CS_OFF();
        }
    }
    return false;
}

/*!
 * @}
 */
/*!
 * \brief W25Q_Init 初始化W25Qx系列的闪存芯片
 * \param handle    挂载闪存芯片的SPI总线的句柄
 * \return 如果初始化成功，返回true
 */
QBL_STATUS W25Q_Init(void)
{
    uint8_t tmp[4];

    GPIO_InitTypeDef io;

    //初始化片选引脚
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

    io.GPIO_Mode = GPIO_Mode_OUT;
    io.GPIO_OType = GPIO_OType_PP;
    io.GPIO_Pin = GPIO_Pin_10;
    io.GPIO_PuPd = GPIO_PuPd_NOPULL;
    io.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOE, &io);

    tmp[0] = W25QXX_CMD_READ_ID;

    W25QXX_CS_ON();
    QBL_SPI_TransmitReceive(W25Q_SPI_BASE, tmp, tmp, 4, 5);
    W25QXX_CS_OFF();

    if (tmp[1] != W25QXX_CHIP_WINBOND_ID) {
        return QBL_FAIL;
    }

    return QBL_OK;
}

/*!
 * \brief W25Q_CheckForIdle 检查闪存芯片是否处于空闲状态，只有空闲状态可以进行操作
 * \return 如果芯片处于空闲状态，返回true
 */

QBL_STATUS W25Q_CheckForIdle(void)
{
    if (W25Q_GetStatus() & W25QXX_BUSY_MASK) {
        return QBL_BUSY;
    }

    return QBL_OK;
}

/*!
 * \brief W25Q_EraseChp 擦除整个芯片
 * \return 如果指令发送成功，返回true
 */

QBL_STATUS W25Q_EraseChip(void)
{
    uint8_t tmp = W25QXX_CMD_CHIP_EARSE;

    W25QXX_CS_ON();
    QBL_SPI_TransmitReceive(W25Q_SPI_BASE, &tmp, NULL, 1, 5);
    W25QXX_CS_OFF();

    return QBL_OK;
}

/*!
 * \brief W25Q_EraseSector 擦除制定的扇区
 * \param addr  要擦除的扇区的地址
 * \return
 */
QBL_STATUS W25Q_EraseSector(uint32_t addr)
{
    uint8_t tmp[4];
    tmp[0] = W25QXX_CMD_SECTOR_ERASE;
    tmp[1] = addr >> 16;
    tmp[2] = (addr >> 8) & 0xff;
    tmp[3] = addr & 0xff;

    W25QXX_CS_ON();
    QBL_SPI_TransmitReceive(W25Q_SPI_BASE, tmp, NULL, 4, 5);
    W25QXX_CS_OFF();

    return QBL_OK;
}
/*!
 * \brief W25Q_Write 写入数据到闪存芯片中
 * \param addr 写入数据的起始地址
 * \param buffer 写入数据的缓存
 * \param length 要写入的数据的长度，最大256字节
 * \return 如果写入成功，返回true
 */
QBL_STATUS W25Q_Write(uint32_t addr, uint8_t* buffer, uint16_t length)
{
    uint8_t tmp[4];

    if (false == W25Q_SetWriteEnable()) {
        return QBL_FAIL;
    }

    tmp[0] = W25QXX_CMD_PROGRAM;
    tmp[1] = addr >> 16;
    tmp[2] = (addr >> 8) & 0xff;
    tmp[3] = addr & 0xff;

    W25QXX_CS_ON();
    QBL_SPI_TransmitReceive(W25Q_SPI_BASE, tmp, NULL, 4, 5);
    QBL_SPI_TransmitReceive(W25Q_SPI_BASE, buffer, NULL, length, 10);
    W25QXX_CS_OFF();
    return QBL_OK;
}

/*!
 * \brief W25Q_Read 从闪存中读取数据
 * \param addr 将要读取数据的起始地址
 * \param buffer 接收数据的缓冲
 * \param length 要读取的数据的长度
 * \return 如果读取成功，返回true
 */

QBL_STATUS W25Q_Read(uint32_t addr, uint8_t* buffer, uint16_t length)
{
    uint8_t tmp[4];
    tmp[0] = W25QXX_CMD_READ;
    tmp[1] = addr >> 16;
    tmp[2] = (addr >> 8) & 0xff;
    tmp[3] = addr & 0xff;

    W25QXX_CS_ON();
    QBL_SPI_TransmitReceive(W25Q_SPI_BASE, tmp, NULL, 4, 5);
    QBL_SPI_TransmitReceive(W25Q_SPI_BASE, NULL, buffer, length, 10);
    W25QXX_CS_OFF();
    return QBL_OK;
}

/*!
 * @}
 */
