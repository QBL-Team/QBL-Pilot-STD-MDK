/*!
 * \file MS5611.c
 * \author no1wudi
 */

#include "MS5611.h"
#include "QBL_SPI.h"
#include "QBL_SysTick.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stdio.h"

/*!
  *
  * @ingroup ms5611_driver MS5611 驱动
  * @{
  */

/*!
 * @defgroup ms5611_driver_internal MS5611 驱动私有数据
 * @{
 */

#define MS_SPI_BASE SPI2_BASE ///< 使用的SPI端口
#define MS5611_CS_On() GPIO_ResetBits(GPIOE, GPIO_Pin_9) ///< 开启芯片片选
#define MS5611_CS_Off() GPIO_SetBits(GPIOE, GPIO_Pin_9) ///< 关闭芯片片选
#define MS5611_CONVERSION_TIME 10 ///< 芯片的转换时间，单位为ms

const uint8_t MS5611_CMD_RST[] = { 0x1E }; ///< 重置芯片
const uint8_t MS5611_CMD_CONVERT_D1[] = { 0x48 }; ///< 设置过采样率为4096转换气压值
const uint8_t MS5611_CMD_CONVERT_D2[] = { 0x58 }; ///< 设置过采样率为4096转换温度值
const uint8_t MS5611_CMD_READ_ADC[] = { 0x00 }; ///< 读取模数转换器转换结果
const uint8_t MS5611_CMD_READ_C1[] = { 0xA2 }; ///< 读取出厂预设值C1
const uint8_t MS5611_CMD_READ_C2[] = { 0xA4 }; ///< 读取出厂预设值C2
const uint8_t MS5611_CMD_READ_C3[] = { 0xA6 }; ///< 读取出厂预设值C3
const uint8_t MS5611_CMD_READ_C4[] = { 0xA8 }; ///< 读取出厂预设值C4
const uint8_t MS5611_CMD_READ_C5[] = { 0xAA }; ///< 读取出厂预设值C5
const uint8_t MS5611_CMD_READ_C6[] = { 0xAC }; ///< 读取出厂预设值C6

static uint64_t ms_ticks = 0; ///< 读取操作的时间戳，单位为ms

static uint16_t C1 = 0; ///<出厂预设值C1,压力灵敏度 | SENS T1
static uint16_t C2 = 0; ///<出厂预设值C2,压力偏移值 | OFF T1
static uint16_t C3 = 0; ///<出厂预设值C3,传感器灵敏度的温度系数 | TCS
static uint16_t C4 = 0; ///<出厂预设值C4,压力偏移的温度系数 | TCO
static uint16_t C5 = 0; ///<出厂预设值C5,参考温度 | T REF
static uint16_t C6 = 0; ///<出厂预设值C6,温度的校正系数 | TEMPSENS

static uint32_t D1 = 0; ///< 压力值
static uint32_t D2 = 0; ///< 温度值

static int32_t dT = 0; ///< 实际温度和参考温度的差值
static int32_t TEMP = 0; ///< 实际温度，范围为-40~85°C，分辨率为0.01°C，例如值为2000，表示20.00°C

static int64_t OFF = 0; ///< 实际温度的偏移
static int64_t SENS = 0; ///< 实际温度的灵敏度
static int32_t P = 0; ///< 温度矫正后的气压值，范围为10~1200mbar，分辨率为0.01mbar，例如值为10000，表示100.00mbr

static int32_t T2 = 0;
static int64_t OFF2 = 0;
static int64_t SENS2 = 0;

/*!
 *\enum MS5611_STATE
 * 读取过程的状态标志
 */
typedef enum {
    MS5611_STATE_IDLE = 0, ///< 上电之后，默认处于本状态
    MS5611_STATE_WAIT_FOR_D1, ///<等待气压转换操作
    MS5611_STATE_WAIT_FOR_D2, ///<等待温度转换操作
} MS5611_STATE;

static MS5611_STATE ms_state; ///< 读取过程的状态变量

/*!
 * \brief MS5611_ReadPROM   读取芯片的出厂预设值
 * \param MS5611_CMD        读取预设值的命令代码
 * \return 读取出来的值
 */
static uint16_t MS5611_ReadPROM(const uint8_t* MS5611_CMD)
{
    uint8_t tmp[2];

    MS5611_CS_On();
    QBL_SPI_TransmitReceive(MS_SPI_BASE, MS5611_CMD, NULL, 1, 5);
    QBL_SPI_TransmitReceive(MS_SPI_BASE, NULL, tmp, 2, 5);
    MS5611_CS_Off();

    return ((uint16_t)tmp[0] << 8 | tmp[1]);
}

/*!
 * \brief MS5611_SendCMD 向芯片发送一条命令
 * \param MS5611_CMD    将被发送的命令，以MS5611_CMD开头
 */
static QBL_STATUS MS5611_SendCMD(const uint8_t* MS5611_CMD)
{
    MS5611_CS_On();
    QBL_STATUS status = QBL_SPI_TransmitReceive(MS_SPI_BASE, MS5611_CMD, NULL, 1, 5);
    MS5611_CS_Off();

	return status;
}
/*!
 * \brief MS5611_Compute    在转换和读取完成后，计算温度值和气压值
 */
static void MS5611_Compute(void)
{
    //refered to datasheet
    dT = D2 - ((int32_t)C5 << 8);
    TEMP = 2000 + ((dT * C6) >> 23);

    OFF = ((int64_t)C2 << 16) + ((C4 * dT) >> 7);
    SENS = ((int64_t)C1 << 15) + ((C3 * dT) >> 8);
    P = (((D1 * SENS) >> 21) - OFF) >> 15;

    if (TEMP < 2000) {
        T2 = (dT * dT) >> 31;
        OFF2 = ((int64_t)5 * (TEMP - 2000) * (TEMP - 2000)) > 1;
        SENS2 = ((int64_t)5 * (TEMP - 2000) * (TEMP - 2000)) > 2;

        if (TEMP < -1500) {
            OFF2 = OFF2 + (int64_t)7 * (TEMP + 1500) * (TEMP + 1500);
            SENS2 = SENS2 + (((int64_t)11 * (TEMP + 1500) * (TEMP + 1500)) >> 1);
        }
    }
    else {
        T2 = 0;
        SENS2 = 0;
        OFF2 = 0;
    }

    TEMP = TEMP - T2;
    OFF = OFF - OFF2;
    SENS = SENS - SENS2;
}

/*!
 * @}
 */

QBL_STATUS MS5611_Init(void)
{
	{
		//初始化ms5611片选引脚
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

		GPIO_InitTypeDef io;
		io.GPIO_Pin = GPIO_Pin_9;
		io.GPIO_Mode = GPIO_Mode_OUT;
		io.GPIO_OType = GPIO_OType_PP;
		io.GPIO_PuPd = GPIO_PuPd_UP;
		io.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_Init(GPIOE, &io);
	}

    //reset the state machine
    ms_state = MS5611_STATE_IDLE;

    //Send reset command
    QBL_STATUS status = MS5611_SendCMD(MS5611_CMD_RST);
	if (status!= QBL_OK) {
		return status;
	}

    //Wait for chip reset
    QBL_Delay(10);

    //Read PROM
    C1 = MS5611_ReadPROM(MS5611_CMD_READ_C1); //Dummy read to activate sensor

    C1 = MS5611_ReadPROM(MS5611_CMD_READ_C1);
    C2 = MS5611_ReadPROM(MS5611_CMD_READ_C2);
    C3 = MS5611_ReadPROM(MS5611_CMD_READ_C3);
    C4 = MS5611_ReadPROM(MS5611_CMD_READ_C4);
    C5 = MS5611_ReadPROM(MS5611_CMD_READ_C5);
    C6 = MS5611_ReadPROM(MS5611_CMD_READ_C6);

    return QBL_OK;
}

bool MS5611_Read(float* Pressure)
{
  uint8_t tmp[3];
	QBL_STATUS status;

	switch (ms_state) {
    case MS5611_STATE_IDLE:
        MS5611_SendCMD(MS5611_CMD_CONVERT_D1);
        ms_ticks = QBL_GetTick();
        ms_state = MS5611_STATE_WAIT_FOR_D1;
        return false;

    case MS5611_STATE_WAIT_FOR_D1:

        if (QBL_GetTick() - ms_ticks >= MS5611_CONVERSION_TIME) {
            //Read adc result
            MS5611_CS_On();
			status = QBL_SPI_TransmitReceive(MS_SPI_BASE, MS5611_CMD_READ_ADC, NULL, 1, 5);
            status = QBL_SPI_TransmitReceive(MS_SPI_BASE, NULL, tmp, 3, 5);
			if (status!= QBL_OK) {
				return false;
			}
            MS5611_CS_Off();

            //Conver to normal order
            D1 = (((uint32_t)tmp[0] << 16) | (tmp[1] << 8) | tmp[2]);

            MS5611_SendCMD(MS5611_CMD_CONVERT_D2);
            ms_ticks = QBL_GetTick();
            ms_state = MS5611_STATE_WAIT_FOR_D2;
        }

        return false;

    case MS5611_STATE_WAIT_FOR_D2:

        if (QBL_GetTick() - ms_ticks >= MS5611_CONVERSION_TIME) {
            //Read adc result
            MS5611_CS_On();
            status = QBL_SPI_TransmitReceive(MS_SPI_BASE, MS5611_CMD_READ_ADC, NULL, 1, 5);
            status = QBL_SPI_TransmitReceive(MS_SPI_BASE, NULL, tmp, 3, 5);
			if (status!= QBL_OK) {
				return false;
			}
            MS5611_CS_Off();

            //Conver to normal order
            D2 = (((uint32_t)tmp[0] << 16) | (tmp[1] << 8) | tmp[2]);

            MS5611_SendCMD((void*)&MS5611_CMD_CONVERT_D1);
            ms_ticks = QBL_GetTick();
            ms_state = MS5611_STATE_WAIT_FOR_D1;

            MS5611_Compute();

            Pressure[0] = P / 100.0f; //pressure
            Pressure[1] = TEMP / 100.0f; //temperature

            return true;
        }

        return false;

    default:
        return false;
    }
}

/*!
 * @}
 */
