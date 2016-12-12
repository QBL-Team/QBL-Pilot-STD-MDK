/*!
 * \file HMC5883.c
 * \author no1wudi
 */

#include "stm32f4xx.h"
#include "QBL_I2C.h"
#include "HMC5883.h"


/*!
  *
  * @ingroup hmc5883_driver
  * @{
  */

/*!
 * @defgroup hmc5883_driver_private HMC5883驱动私有数据
 * @{
 */

const float HMC5883_SCALE = 440.0f;   ///< 原始数据到真实数据的缩放比例，参见其数据手册

#define HMC5883_IIC_ADDRESS     0x3C  ///< 芯片的IIC地址，参见其数据手册
#define HMC5883_REG_DATA_ADDR   0x03  ///< 数据寄存器起始地址
#define HMC5883_REG_DATA_LENGTH 6     ///< 读取的数据量，单位为byte
#define HMC5883_REG_ADDR_WIDTH  1     ///< 寄存器地址的宽度，单位为byte

/*!
 *\brief 数据配置表，左侧为寄存器，右侧为参数值，具体参见其数据手册
 */

static const uint8_t HMC5883_CONFIG_TABLE[] =
{
    0x00, 0x18,
    0x01, 0x80,
    0x02, 0x00,
};

bool HMC5883_Init(void)
{
    uint8_t cnts = 0;
    QBL_STATUS status;
    for(; cnts < sizeof(HMC5883_CONFIG_TABLE); cnts += 2)
    {
        status = QBL_I2C_Mem_Write(I2C1_BASE, HMC5883_IIC_ADDRESS, HMC5883_CONFIG_TABLE[cnts],
									(uint8_t *)(HMC5883_CONFIG_TABLE + 1 + cnts), 1, 2);
        if(QBL_OK != status)
        {
            return false;
        }
    }

    return true;
}

static bool HMC5883_Assemble(uint8_t* data, float* imuData)
{
    if ((NULL==data) || (NULL==imuData)) {
        return false;
    }

    imuData[0] = ((int16_t)((data[0] << 8) | data[1])) / HMC5883_SCALE;
    imuData[1] = ((int16_t)((data[4] << 8) | data[5])) / HMC5883_SCALE;
    imuData[2] = ((int16_t)((data[2] << 8) | data[3])) / HMC5883_SCALE;

	return true;
}

bool HMC5883_Read(float* imuData)
{
	static uint8_t data[HMC5883_REG_DATA_LENGTH];

#ifdef USE_DMA_I2C1_INTERRUPT
	static uint8_t is_mhc5883_read = 0;

	while (SET == I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY)){}

	if(RESET == I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY))
	{
		if (QBL_I2C_LastReadFinished() && is_mhc5883_read == 1) {
			HMC5883_Assemble(data, imuData);
			is_mhc5883_read = 0;
		}

		QBL_STATUS status;
		status = QBL_I2C_Mem_Read(I2C1_BASE, HMC5883_IIC_ADDRESS, HMC5883_REG_DATA_ADDR, data, HMC5883_REG_DATA_LENGTH, 5);
		if(QBL_OK != status)
		{
			return false;
		}
		is_mhc5883_read = 1;
	}
#else
	{
		QBL_STATUS status;
		status = QBL_I2C_Mem_Read(I2C1_BASE, HMC5883_IIC_ADDRESS, HMC5883_REG_DATA_ADDR, data, HMC5883_REG_DATA_LENGTH, 5);

		if(QBL_OK != status)
		{
			return false;
		}

		HMC5883_Assemble(data, imuData);
	}
#endif

	return true;
}

/*!
 * @}
 */


/*!
 * @}
 */
