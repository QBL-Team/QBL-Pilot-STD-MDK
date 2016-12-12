/*!
 * \file MPU6050.c
 * \author no1wudi
 */

/*!
  *
  * @ingroup mpu6050_driver
  * @{
  */

#include "MPU6050.h"
#include "QBL_I2C.h"
#include "stm32f4xx.h"

const float MPU6050_GYRO_SCALE = 32.8f;   ///< 陀螺仪的数据缩放比例
const float MPU6050_ACCE_SCALE = 4096.0f; ///< 加速度计的数据缩放比例

#define MPU6050_IIC_ADDRESS 0xD0   ///< 芯片的IIC地址，参见其数据手册
#define MPU6050_CHIP_ID 0x68       ///< 芯片的ID
#define MPU6050_REG_ID_ADDR 0x75   ///< 芯片ID寄存器的地址
#define MPU6050_REG_DATA_ADDR 0x3B ///< 数据寄存器的起始地址
#define MPU6050_REG_DATA_LENGTH 14 ///< 将要读取的数据的长度，单位为byte
#define MPU6050_REG_ADDR_WIDTH 1   ///< 寄存器地址的宽度，单位为byte

/*!
 *\brief 传感器的配置表，左边为寄存器地址，右边为具体的配置值
 */
static uint8_t MPU_CONFIG_TABLE[] = {
    0x6B, 0x00, //PWM MGMT 1
    0x1B, 0x10, //GYRO Range 0 - 1000
    0x1C, 0x10, //Acce Range 0 - 8g
    0x1A, 0x00, //Low pass filter
    0x6A, 0x00,
    0x37, 0x02
};

bool MPU6050_Init(void)
{
    QBL_STATUS status;

	{
		uint8_t data[14]; //temp variable

		QBL_I2C_Reset();

		status = QBL_I2C_Mem_Read(I2C1_BASE, MPU6050_IIC_ADDRESS, MPU6050_REG_ID_ADDR, data, 14, 5);
		if (QBL_OK != status) {
			return false;
		}

		while(QBL_I2C_IsBusy()==true);

		QBL_I2C_Reset();
	}

	{
		uint8_t cnts = 0; //temp counter
		for (cnts = 0; cnts < sizeof(MPU_CONFIG_TABLE); cnts += 2) {
			status = QBL_I2C_Mem_Write(I2C1_BASE, MPU6050_IIC_ADDRESS, MPU_CONFIG_TABLE[cnts], (uint8_t*)(MPU_CONFIG_TABLE + 1 + cnts), 1, 2);
			if (QBL_OK != status) {
				return false;
			}
		}

		return true;
	}
}

static bool MPU6050_Assemble(uint8_t* data, float* imuData)
{
	if ((NULL==data) || (NULL==imuData)) {
			return false;
	}

	imuData[0] = (int16_t)((data[0] << 8) | data[1]);
	imuData[1] = (int16_t)((data[2] << 8) | data[3]);
	imuData[2] = (int16_t)((data[4] << 8) | data[5]);
	imuData[3] = (int16_t)((data[8] << 8) | data[9]);
	imuData[4] = (int16_t)((data[10] << 8) | data[11]);
	imuData[5] = (int16_t)((data[12] << 8) | data[13]);
	imuData[6] = (int16_t)((data[6] << 8) | data[7]);
	
	//Acc data normalize ~ x,y,z
	imuData[0] = imuData[0] * 9.8f / MPU6050_ACCE_SCALE;
	imuData[1] = imuData[1] * 9.8f / MPU6050_ACCE_SCALE;
	imuData[2] = imuData[2] * 9.8f / MPU6050_ACCE_SCALE;

	//Gyro data normalize ~ x,y,z
	imuData[3] /= MPU6050_GYRO_SCALE;
	imuData[4] /= MPU6050_GYRO_SCALE;
	imuData[5] /= MPU6050_GYRO_SCALE;

	//temperature data
	imuData[6] = imuData[6] / 340;
	imuData[6] += 36.53f;

	return true;
}

bool MPU6050_Read(float* imuData)
{
	static uint8_t data[MPU6050_REG_DATA_LENGTH];

#ifdef USE_DMA_I2C1_INTERRUPT
	//先读取上一次触发的MPU6050的数据
	static uint8_t is_mpu6050_read = 0;

	while (SET == I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY)){}

	if(RESET == I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY))
	{
		//注意：多个芯片使用I2C1时，QBL_I2C_LastReadFinished并不能准确判断是谁触发的中断
		//但是，只要多个芯片在使用I2C1时都正常，则没有问题。
		if (QBL_I2C_LastReadFinished() && is_mpu6050_read == 1) {
			MPU6050_Assemble(data, imuData);

			is_mpu6050_read = 0;
		}

		//再触发下一次读取
		QBL_STATUS status;
		status = QBL_I2C_Mem_Read(I2C1_BASE, MPU6050_IIC_ADDRESS, MPU6050_REG_DATA_ADDR, data, MPU6050_REG_DATA_LENGTH, 2);
		if (QBL_OK != status) {
			return false;
		}
		is_mpu6050_read = 1;
	}

#else
	{
		QBL_STATUS status;
		status = QBL_I2C_Mem_Read(I2C1_BASE, MPU6050_IIC_ADDRESS, MPU6050_REG_DATA_ADDR, data, MPU6050_REG_DATA_LENGTH, 2);
		if (QBL_OK != status) {
			return false;
		}

		MPU6050_Assemble(data, imuData);
	}
#endif

	return true;
}


/*!
 * @}
 */
