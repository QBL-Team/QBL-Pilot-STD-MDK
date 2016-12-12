/*!
 * \file SD.h
 * \author no1wudi
 */

#ifndef __SD_H
#define __SD_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "ff.h"
#include "diskio.h"

/*!
 *@defgroup sd_card SD卡驱动
 *@{
 */

typedef enum {
    /**
  * @brief  SDIO错误类型定义
  */
    SD_CMD_CRC_FAIL = (1), /*!< 接收到了命令响应，但是CRC校验失败 */
    SD_DATA_CRC_FAIL = (2), /*!<  数据块发送/接收成功，但是CRC校验失败*/
    SD_CMD_RSP_TIMEOUT = (3), /*!< 命令响应超时 */
    SD_DATA_TIMEOUT = (4), /*!< 数据超时 */
    SD_TX_UNDERRUN = (5), /*!< 发送FIFO尚未清空 */
    SD_RX_OVERRUN = (6), /*!< 接收FIFO溢出 */
    SD_START_BIT_ERR = (7), /*!< 宽总线模式下未在数据信号上检测到起始位 */
    SD_CMD_OUT_OF_RANGE = (8), /*!< 命令的参数超出范围 */
    SD_ADDR_MISALIGNED = (9), /*!< 地址没有对齐 */
    SD_BLOCK_LEN_ERR = (10), /*!< 发送的数据块的长度不能被用于本SD卡，或者发送的数据长度与块的长度不匹配 */
    SD_ERASE_SEQ_ERR = (11), /*!< 擦除指令执行过程中发生了一个错误 */
    SD_BAD_ERASE_PARAM = (12), /*!< 擦除组选择非法*/
    SD_WRITE_PROT_VIOLATION = (13), /*!< 试图写入一个受保护的块 */
    SD_LOCK_UNLOCK_FAILED = (14), /*!< 试图访问一个锁定的卡，或者检测到序列或密码错误*/
    SD_COM_CRC_FAILED = (15), /*!< 前一条指令的CRC校验出错 */
    SD_ILLEGAL_CMD = (16), /*!< 当前卡的状态下此指令非法 */
    SD_CARD_ECC_FAILED = (17), /*!< 存储卡尝试执行了ECC校验，但数据没有被纠正 */
    SD_CC_ERROR = (18), /*!< 存储卡内部控制器错误 */
    SD_GENERAL_UNKNOWN_ERROR = (19), /*!< 未知错误 */
    SD_STREAM_READ_UNDERRUN = (20),
    SD_STREAM_WRITE_OVERRUN = (21),
    SD_CID_CSD_OVERWRITE = (22),
    SD_WP_ERASE_SKIP = (23),
    SD_CARD_ECC_DISABLED = (24),
    SD_ERASE_RESET = (25),
    SD_AKE_SEQ_ERROR = (26),
    SD_INVALID_VOLTRANGE = (27),
    SD_ADDR_OUT_OF_RANGE = (28),
    SD_SWITCH_ERROR = (29),
    SD_SDIO_DISABLED = (30),
    SD_SDIO_FUNCTION_BUSY = (31),
    SD_SDIO_FUNCTION_FAILED = (32),
    SD_SDIO_UNKNOWN_FUNCTION = (33),

    /**
  * @brief  标准错误定义
  */
    SD_INTERNAL_ERROR,
    SD_NOT_CONFIGURED,
    SD_REQUEST_PENDING,
    SD_REQUEST_NOT_APPLICABLE,
    SD_INVALID_PARAMETER,
    SD_UNSUPPORTED_FEATURE,
    SD_UNSUPPORTED_HW,
    SD_ERROR,
    SD_OK = 0
} SD_Error;

/**
  * @brief  SDIO 传输状态
  */
typedef enum {
    SD_TRANSFER_OK = 0,
    SD_TRANSFER_BUSY = 1,
    SD_TRANSFER_ERROR
} SDTransferState;

/**
  * @brief  SD卡状态
  */
typedef enum {
    SD_CARD_READY = ((uint32_t)0x00000001),
    SD_CARD_IDENTIFICATION = ((uint32_t)0x00000002),
    SD_CARD_STANDBY = ((uint32_t)0x00000003),
    SD_CARD_TRANSFER = ((uint32_t)0x00000004),
    SD_CARD_SENDING = ((uint32_t)0x00000005),
    SD_CARD_RECEIVING = ((uint32_t)0x00000006),
    SD_CARD_PROGRAMMING = ((uint32_t)0x00000007),
    SD_CARD_DISCONNECTED = ((uint32_t)0x00000008),
    SD_CARD_ERROR = ((uint32_t)0x000000FF)
} SDCardState;

/**
  * @brief  存储卡专有数据CSD
  */
typedef struct
    {
    volatile uint8_t CSDStruct; /*!< 专有数据结构体 */
    volatile uint8_t SysSpecVersion; /*!< 系统识别号 */
    volatile uint8_t Reserved1; /*!< 保留 */
    volatile uint8_t TAAC; /*!< 数据读取访问时间1 */
    volatile uint8_t NSAC; /*!< 数据读取访问时间2，单位为时钟周期 */
    volatile uint8_t MaxBusClkFrec; /*!< 最大总线频率 */
    volatile uint16_t CardComdClasses; /*!< 卡命令级别 */
    volatile uint8_t RdBlockLen; /*!< 读取块的最大长度 */
    volatile uint8_t PartBlockRead; /*!< 部分块允许读取 */
    volatile uint8_t WrBlockMisalign; /*!< 写入块没有对齐 */
    volatile uint8_t RdBlockMisalign; /*!< 读取块没有对齐 */
    volatile uint8_t DSRImpl; /*!< DSR 实现 */
    volatile uint8_t Reserved2; /*!< 保留 */
    volatile uint32_t DeviceSize; /*!< 设备大小 */
    volatile uint8_t MaxRdCurrentVDDMin;
    volatile uint8_t MaxRdCurrentVDDMax;
    volatile uint8_t MaxWrCurrentVDDMin;
    volatile uint8_t MaxWrCurrentVDDMax;
    volatile uint8_t DeviceSizeMul;
    volatile uint8_t EraseGrSize;
    volatile uint8_t EraseGrMul;
    volatile uint8_t WrProtectGrSize;
    volatile uint8_t WrProtectGrEnable;
    volatile uint8_t ManDeflECC;
    volatile uint8_t WrSpeedFact;
    volatile uint8_t MaxWrBlockLen;
    volatile uint8_t WriteBlockPaPartial;
    volatile uint8_t Reserved3;
    volatile uint8_t ContentProtectAppli;
    volatile uint8_t FileFormatGrouop;
    volatile uint8_t CopyFlag;
    volatile uint8_t PermWrProtect;
    volatile uint8_t TempWrProtect;
    volatile uint8_t FileFormat;
    volatile uint8_t ECC;
    volatile uint8_t CSD_CRC;
    volatile uint8_t Reserved4;
} SD_CSD;

/**
  * @brief  存储卡识别数据
  */
typedef struct
    {
    volatile uint8_t ManufacturerID; /*!< 制造商识别码 */
    volatile uint16_t OEM_AppliID; /*!< 产品识别码 */
    volatile uint32_t ProdName1; /*!< 产品名称（1） */
    volatile uint8_t ProdName2; /*!< 产品名称（2）*/
    volatile uint8_t ProdRev; /*!< 产品版本 */
    volatile uint32_t ProdSN; /*!< 产品序列号 */
    volatile uint8_t Reserved1; /*!< 保留 */
    volatile uint16_t ManufactDate; /*!< 生产数据 */
    volatile uint8_t CID_CRC;
    volatile uint8_t Reserved2;
} SD_CID;

/**
  * @brief SD卡状态
  */
typedef struct
    {
    volatile uint8_t DAT_BUS_WIDTH;
    volatile uint8_t SECURED_MODE;
    volatile uint16_t SD_CARD_TYPE;
    volatile uint32_t SIZE_OF_PROTECTED_AREA;
    volatile uint8_t SPEED_CLASS;
    volatile uint8_t PERFORMANCE_MOVE;
    volatile uint8_t AU_SIZE;
    volatile uint16_t ERASE_SIZE;
    volatile uint8_t ERASE_TIMEOUT;
    volatile uint8_t ERASE_OFFSET;
} SD_CardStatus;

/**
  * @brief SD卡信息
  */
typedef struct
    {
    SD_CSD SD_csd;
    SD_CID SD_cid;
    uint64_t CardCapacity; /*!< SD卡容量 */
    uint32_t CardBlockSize; /*!< SD卡块大小 */
    uint16_t RCA;
    uint8_t CardType;
} SD_CardInfo;

/**
  * @brief SDIO 命令索引
  */
#define SD_CMD_GO_IDLE_STATE ((uint8_t)0)
#define SD_CMD_SEND_OP_COND ((uint8_t)1)
#define SD_CMD_ALL_SEND_CID ((uint8_t)2)
#define SD_CMD_SET_REL_ADDR ((uint8_t)3)
#define SD_CMD_SET_DSR ((uint8_t)4)
#define SD_CMD_SDIO_SEN_OP_COND ((uint8_t)5)
#define SD_CMD_HS_SWITCH ((uint8_t)6)
#define SD_CMD_SEL_DESEL_CARD ((uint8_t)7)
#define SD_CMD_HS_SEND_EXT_CSD ((uint8_t)8)
#define SD_CMD_SEND_CSD ((uint8_t)9)
#define SD_CMD_SEND_CID ((uint8_t)10)
#define SD_CMD_READ_DAT_UNTIL_STOP ((uint8_t)11)
#define SD_CMD_STOP_TRANSMISSION ((uint8_t)12)
#define SD_CMD_SEND_STATUS ((uint8_t)13)
#define SD_CMD_HS_BUSTEST_READ ((uint8_t)14)
#define SD_CMD_GO_INACTIVE_STATE ((uint8_t)15)
#define SD_CMD_SET_BLOCKLEN ((uint8_t)16)
#define SD_CMD_READ_SINGLE_BLOCK ((uint8_t)17)
#define SD_CMD_READ_MULT_BLOCK ((uint8_t)18)
#define SD_CMD_HS_BUSTEST_WRITE ((uint8_t)19)
#define SD_CMD_WRITE_DAT_UNTIL_STOP ((uint8_t)20)
#define SD_CMD_SET_BLOCK_COUNT ((uint8_t)23)
#define SD_CMD_WRITE_SINGLE_BLOCK ((uint8_t)24)
#define SD_CMD_WRITE_MULT_BLOCK ((uint8_t)25)
#define SD_CMD_PROG_CID ((uint8_t)26)
#define SD_CMD_PROG_CSD ((uint8_t)27)
#define SD_CMD_SET_WRITE_PROT ((uint8_t)28)
#define SD_CMD_CLR_WRITE_PROT ((uint8_t)29)
#define SD_CMD_SEND_WRITE_PROT ((uint8_t)30)
#define SD_CMD_SD_ERASE_GRP_START ((uint8_t)32)
#define SD_CMD_SD_ERASE_GRP_END ((uint8_t)33)
#define SD_CMD_ERASE_GRP_START ((uint8_t)35)

#define SD_CMD_ERASE_GRP_END ((uint8_t)36)

#define SD_CMD_ERASE ((uint8_t)38)
#define SD_CMD_FAST_IO ((uint8_t)39)
#define SD_CMD_GO_IRQ_STATE ((uint8_t)40)
#define SD_CMD_LOCK_UNLOCK ((uint8_t)42)
#define SD_CMD_APP_CMD ((uint8_t)55)
#define SD_CMD_GEN_CMD ((uint8_t)56)
#define SD_CMD_NO_CMD ((uint8_t)64)

#define SD_CMD_APP_SD_SET_BUSWIDTH ((uint8_t)6) /*!< 仅对SD卡有效 */
#define SD_CMD_SD_APP_STAUS ((uint8_t)13) /*!< 仅对SD卡有效  */
#define SD_CMD_SD_APP_SEND_NUM_WRITE_BLOCKS ((uint8_t)22) /*!< 仅对SD卡有效  */
#define SD_CMD_SD_APP_OP_COND ((uint8_t)41) /*!< 仅对SD卡有效  */
#define SD_CMD_SD_APP_SET_CLR_CARD_DETECT ((uint8_t)42) /*!< 仅对SD卡有效  */
#define SD_CMD_SD_APP_SEND_SCR ((uint8_t)51) /*!< 仅对SD卡有效  */
#define SD_CMD_SDIO_RW_DIRECT ((uint8_t)52) /*!< 仅对SD卡有效  */
#define SD_CMD_SDIO_RW_EXTENDED ((uint8_t)53) /*!<仅对SD卡有效  */

#define SD_CMD_SD_APP_GET_MKB ((uint8_t)43) /*!<仅对SD卡有效 */
#define SD_CMD_SD_APP_GET_MID ((uint8_t)44) /*!< 仅对SD卡有效  */
#define SD_CMD_SD_APP_SET_CER_RN1 ((uint8_t)45) /*!<仅对SD卡有效 */
#define SD_CMD_SD_APP_GET_CER_RN2 ((uint8_t)46) /*!< 仅对SD卡有效  */
#define SD_CMD_SD_APP_SET_CER_RES2 ((uint8_t)47) /*!< 仅对SD卡有效  */
#define SD_CMD_SD_APP_GET_CER_RES1 ((uint8_t)48) /*!< 仅对SD卡有效 */
#define SD_CMD_SD_APP_SECURE_READ_MULTIPLE_BLOCK ((uint8_t)18) /*!< 仅对SD卡有效  */
#define SD_CMD_SD_APP_SECURE_WRITE_MULTIPLE_BLOCK ((uint8_t)25) /*!< 仅对SD卡有效  */
#define SD_CMD_SD_APP_SECURE_ERASE ((uint8_t)38) /*!< 仅对SD卡有效  */
#define SD_CMD_SD_APP_CHANGE_SECURE_AREA ((uint8_t)49) /*!< 仅对SD卡有效  */
#define SD_CMD_SD_APP_SECURE_WRITE_MKB ((uint8_t)48) /*!< 仅对SD卡有效  */

/* 选择SD卡的工作模式 */
#if !defined(SD_DMA_MODE) && !defined(SD_POLLING_MODE)
#define SD_DMA_MODE ((uint32_t)0x00000000)
//#define SD_POLLING_MODE                            ((uint32_t)0x00000002)
#endif

/**
  * @brief  是否在SD卡槽中检测到SD卡
  */
#define SD_PRESENT ((uint8_t)0x01)
#define SD_NOT_PRESENT ((uint8_t)0x00)

/**
  * @brief 支持的存储卡类型
  */
#define SDIO_STD_CAPACITY_SD_CARD_V1_1 ((uint32_t)0x00000000)
#define SDIO_STD_CAPACITY_SD_CARD_V2_0 ((uint32_t)0x00000001)
#define SDIO_HIGH_CAPACITY_SD_CARD ((uint32_t)0x00000002)
#define SDIO_MULTIMEDIA_CARD ((uint32_t)0x00000003)
#define SDIO_SECURE_DIGITAL_IO_CARD ((uint32_t)0x00000004)
#define SDIO_HIGH_SPEED_MULTIMEDIA_CARD ((uint32_t)0x00000005)
#define SDIO_SECURE_DIGITAL_IO_COMBO_CARD ((uint32_t)0x00000006)
#define SDIO_HIGH_CAPACITY_MMC_CARD ((uint32_t)0x00000007)

/**
  * @brief  初始化存储卡
  * @retval SD_Error: 存储卡错误状态
  */
SD_Error SD_Init(void);

/**
  * @brief  获取当前的SD卡传输状态
  * @retval SDTransferState: 数据传输状态
  *   This value can be:
  *        - SD_TRANSFER_OK: 没有数据正在传输
  *        - SD_TRANSFER_BUSY: 有数据正在传输
  */

SDTransferState SD_GetStatus(void);

/**
  * @brief  获取当前卡的状态
  * @retval SDCardState: 存储卡错误代码
  */

SDCardState SD_GetState(void);

/**
 * @brief  检测存储卡是否在插槽中
 * @retval 存储卡是否在插槽中
 */

uint8_t SD_Detect(void);

/**
  * @brief  查询存储卡的操作电压和时钟配置
  * @retval SD_Error: 存储卡错误代码
  */

SD_Error SD_PowerON(void);

/**
  * @brief  关闭SDIO输出信号
  * @retval SD_Error: 存储卡错误代码
  */

SD_Error SD_PowerOFF(void);

/**
  * @brief  初始化存储卡（已被 SD_Init() 内部调用）
  * @retval SD_Error: 存储卡错误代码
  */

SD_Error SD_InitializeCards(void);
/**
  * @brief  获取存储卡的信息
  * @param  cardinfo: 存储卡信息结构体
  * @retval SD_Error: 存储卡错误代码
  */

SD_Error SD_GetCardInfo(SD_CardInfo* cardinfo);

/**
 * @brief SD_GetCardStatus 获取当前的存储卡状态
 * @param cardstatus 状态缓冲
 * @return  存储卡错误代码
 */

SD_Error SD_GetCardStatus(SD_CardStatus* cardstatus);

/**
  * @brief  如果存储卡支持的话，开启宽总线操作
  * @param  WideMode: 具体的总线模式
  *     本参数可以为下列值:
  *     @arg SDIO_BusWide_8b: 8位数据模式（仅对MMC卡有效）
  *     @arg SDIO_BusWide_4b: 4位数据模式
  *     @arg SDIO_BusWide_1b: 1位数据模式
  * @retval SD_Error: 存储卡错误代码
  */

SD_Error SD_EnableWideBusOperation(uint32_t WideMode);

/**
  * @brief  选择或反选响应的存储卡
  * @param  addr:选择的地址
  * @retval SD_Error: 存储卡错误代码
  */

SD_Error SD_SelectDeselect(uint64_t addr);

/**
  * @brief  读取指定块中的数据
  * @note   本操作须要通过以下两个函数来检查DMA控制器和存储卡的状态
  *          - SD_ReadWaitOperation(): 本函数确认DMA控制器传输完成
  *          - SD_GetStatus(): 本函数检查SD卡内部是否完成了数据的读取操作
  * @param  readbuff: 读取数据的缓冲
  * @param  ReadAddr: 读取块的起始地址
  * @param  BlockSize: 块的大小
  * @retval SD_Error: 存储卡错误代码
  */

SD_Error SD_ReadBlock(uint8_t* readbuff, uint64_t ReadAddr, uint16_t BlockSize);
/**
  * @brief  读取多个块中的数据
  * @note   本操作须要通过以下两个函数来检查DMA控制器和存储卡的状态
  *          - SD_ReadWaitOperation():  本函数确认DMA控制器传输完成
  *          - SD_GetStatus(): 本函数检查SD卡内部是否完成了数据的读取操作
  * @param  readbuff: 读取数据的缓冲
  * @param  ReadAddr: 读取块的起始地址
  * @param  BlockSize: 块的大小
  * @param  NumberOfBlocks: 读取的块的数量
  * @retval SD_Error: 存储卡错误代码
  */
SD_Error SD_ReadMultiBlocks(uint8_t* readbuff, uint64_t ReadAddr, uint16_t BlockSize, uint32_t NumberOfBlocks);
/**
  * @brief  向卡中指定块写入数据
  * @note   本操作须要通过以下两个函数来检查DMA控制器和存储卡的状态
  *          - SD_WriteWaitOperation(): 本函数确认DMA控制器传输完成
  *          - SD_GetStatus():本函数检查SD卡内部是否完成了数据的写入操作
  * @param  writebuff: 要写入的数据缓冲
  * @param  WriteAddr: 要写入的块地址
  * @param  BlockSize: 块大小
  * @retval SD_Error: 存储卡错误代码
  */
SD_Error SD_WriteBlock(uint8_t* writebuff, uint64_t WriteAddr, uint16_t BlockSize);

/**
  * @brief  写入多个块，但是只能在DMA模式下管理传输状态
  *
  * @note   本操作须要通过以下两个函数来检查DMA控制器和存储卡的状态
  *          - SD_WriteWaitOperation(): 本函数确认DMA控制器传输完成
  *          - SD_GetStatus(): 本函数检查SD卡内部是否完成了数据的写入操作
  * @param  WriteAddr: 要写入的地址
  * @param  writebuff: 要写入的数据缓冲
  * @param  BlockSize: 块大小
  * @param  NumberOfBlocks: 要写入的块的数量
  * @retval SD_Error: 存储卡错误代码
  */

SD_Error SD_WriteMultiBlocks(uint8_t* writebuff, uint64_t WriteAddr, uint16_t BlockSize, uint32_t NumberOfBlocks);

/**
  * @brief  获取当前的传输状态
  * @retval SDTransferState: 数据传输状态
  *   This value can be:
  *        - SD_TRANSFER_OK: 当前没有数据正在传输
  *        - SD_TRANSFER_BUSY: 当前有数据正在传输
  */

SDTransferState SD_GetTransferState(void);

/**
  * @brief  终止传输
  * @retval SD_Error:  存储卡错误代码
  */

SD_Error SD_StopTransfer(void);

/**
  * @brief  擦除存储卡上指定地址
  * @param  startaddr: 起始地址
  * @param  endaddr: 结束地址
  * @retval SD_Error: 存储卡错误代码
  */

SD_Error SD_Erase(uint64_t startaddr, uint64_t endaddr);

/**
  * @brief  发送存储卡状态
  * @param  pcardstatus: 状态寄存器缓冲
  * @retval SD_Error: 存储卡错误代码
  */

SD_Error SD_SendStatus(uint32_t* pcardstatus);

/**
  * @brief  获取当前存储卡的状态
  * @param  psdstatus: 状态寄存器缓冲
  * @retval SD_Error: 存储卡错误代码
  */

SD_Error SD_SendSDStatus(uint32_t* psdstatus);

/**
  * @brief  处理被触发的中断
  * @retval SD_Error: 存储卡错误代码
  */

SD_Error SD_ProcessIRQSrc(void);

/**
  * @brief  本函数等待SDIO的DMA传输完成
  */

void SD_ProcessDMAIRQ(void);

/**
  * @brief  本函数等待SDIO的DMA传输完成
  *         本函数应该在 SDIO_ReadBlock() 和 SDIO_ReadMultiBlocks() 调用之后调用，以确保数据传输完成
  * @param  无
  * @retval SD_Error: 存储卡错误代码
  */
SD_Error SD_WaitReadOperation(void);

/**
  * @brief  本函数等待SDIO的DMA传输完成
  *         本函数应该在 SDIO_WriteBlock() 和 SDIO_WriteMultiBlocks() 调用之后调用，以确保数据传输完成
  * @param  无
  * @retval SD_Error: 存储卡错误代码
  */
SD_Error SD_WaitWriteOperation(void);

/**
* @brief  切换到高速模式
* @note   本操作将PLL输出到SDIO的时钟配置为67-75Mhz
* @param  无
* @retval SD_Error: 存储卡错误代码
*/
SD_Error SD_HighSpeed(void);

/*！
 *@}
 */
#ifdef __cplusplus
}
#endif

#endif /* __SD_H */
