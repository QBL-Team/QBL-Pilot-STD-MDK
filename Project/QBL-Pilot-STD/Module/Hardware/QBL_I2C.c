/**
 *\file QBL_I2C.c
 *\author no1wudi
 */

#include "QBL_Type.h"
#include "QBL_I2C.h"
#include "QBL_SysTick.h"
#include "stm32f4xx_i2c.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_dma.h"
#include <stdbool.h>

#define TOINSTANCE(I2C_Base) ((I2C_TypeDef*)I2C_Base) /**< 实例化模块 */


#ifdef USE_DMA_I2C1_INTERRUPT
static uint32_t I2C_DMA_DATA_DataStamp = 0;
static uint32_t I2C_DMA_DATA_DataStampRef = 0;
#endif

static void QBL_I2C_Config(void)
{
	{
		//HMC5883和MPU6050公用I2C1
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//SDA SCL GPIO clock enable
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);//I2C1 clock enable
	}

	{
		GPIO_InitTypeDef io;

		io.GPIO_Mode = GPIO_Mode_AF;
		io.GPIO_OType = GPIO_OType_OD;
		io.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
		io.GPIO_PuPd = GPIO_PuPd_UP;
		io.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_Init(GPIOB, &io);
	}

	{
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_I2C1);
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_I2C1);
	}

	{
		I2C_InitTypeDef ii;

		I2C_DeInit(I2C1);
		ii.I2C_Ack = I2C_Ack_Enable;
		ii.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
		ii.I2C_ClockSpeed = 400000;
		ii.I2C_DutyCycle = I2C_DutyCycle_2;
		ii.I2C_Mode = I2C_Mode_I2C;
		ii.I2C_OwnAddress1 = 0xAA;
		I2C_Init(I2C1, &ii);

		I2C_AnalogFilterCmd(I2C1,ENABLE); //滤波暂时不考虑
		I2C_DigitalFilterConfig(I2C1,0x0F);

		I2C_Cmd(I2C1, ENABLE);
	}
}

#if (defined USE_DMA_I2C1_INTERRUPT) || (defined USE_DMA_I2C1)
static void QBL_I2C_DMA_Config(void)
{
	{
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
	}

	{
		DMA_ClearFlag(DMA1_Stream0, DMA_FLAG_TCIF0 | DMA_FLAG_FEIF0 | DMA_FLAG_DMEIF0 | \
							   DMA_FLAG_TEIF0 | DMA_FLAG_HTIF0);//清除接收状态标志位
	}

	{
		DMA_InitTypeDef  dma;

		DMA_Cmd(DMA1_Stream0, DISABLE);

		dma.DMA_Channel = DMA_Channel_1;
		dma.DMA_PeripheralBaseAddr = (I2C1_BASE + I2C_Register_DR);
		dma.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
		dma.DMA_MemoryInc = DMA_MemoryInc_Enable;
		dma.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
		dma.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
		dma.DMA_Mode = DMA_Mode_Normal;
		dma.DMA_Priority = DMA_Priority_VeryHigh;
		dma.DMA_FIFOMode = DMA_FIFOMode_Enable;
		dma.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
		dma.DMA_MemoryBurst = DMA_MemoryBurst_Single;
		dma.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
		dma.DMA_DIR = DMA_DIR_PeripheralToMemory;
		dma.DMA_Memory0BaseAddr = (uint32_t)0xffff;
		dma.DMA_BufferSize = 14;//默认读取mpu6050的数据字节数
		DMA_DeInit(DMA1_Stream0);

		DMA_Init(DMA1_Stream0, &dma);//初始化DMA用于接收
	}
}
#endif

#ifdef USE_DMA_I2C1_INTERRUPT
static void QBL_I2C_DMA_NVIC_Config(void)
{
	NVIC_InitTypeDef nvic;

	/* Configure one bit for preemption priority */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

	nvic.NVIC_IRQChannel = DMA1_Stream0_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority = 0;
	nvic.NVIC_IRQChannelSubPriority = 0;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic);
	
	DMA_ITConfig(DMA1_Stream0, DMA_IT_TC, DISABLE);
}
#endif

#ifdef USE_DMA_I2C1_INTERRUPT
void DMA1_Stream0_IRQHandler(void)
{
	/* 判断是否为DMA接收完成中断 */
	if (DMA_GetFlagStatus(DMA1_Stream0,DMA_FLAG_TCIF0)!=RESET) {

		DMA_ITConfig(DMA1_Stream0, DMA_IT_TC, DISABLE);
		
		I2C_GenerateSTOP(I2C1, ENABLE);

		DMA_Cmd(DMA1_Stream0, DISABLE);
		
		I2C_DMACmd(I2C1, DISABLE);

		DMA_ClearFlag(DMA1_Stream0, DMA_FLAG_TCIF0 | DMA_FLAG_FEIF0 | DMA_FLAG_DMEIF0 | \
                                       DMA_FLAG_TEIF0 | DMA_FLAG_HTIF0);

		I2C_DMA_DATA_DataStamp ++;
	}
}
#endif

void QBL_I2C_Reset(void)
{
#ifdef USE_DMA_I2C1_INTERRUPT	
	I2C_DMA_DATA_DataStampRef = 0;
	I2C_DMA_DATA_DataStamp = 0;
#else

#endif
}

bool QBL_I2C_IsBusy(void)
{
#ifdef USE_DMA_I2C1_INTERRUPT		
	if (I2C_DMA_DATA_DataStamp!= I2C_DMA_DATA_DataStampRef) {
		return false;
	}else {
		return true;
	}
#else
	return false;
#endif
}

bool QBL_I2C_LastReadFinished(void)
{
#ifdef USE_DMA_I2C1_INTERRUPT		
	if (I2C_DMA_DATA_DataStamp!= I2C_DMA_DATA_DataStampRef) {
		return true;
	}else {
		return false;
	}
#else
	return false;
#endif
}

#ifndef USE_BASE_I2C1
static inline void QBL_I2C_DMA_SetMemoryAddr(const uint8_t* Addr)
{
	DMA_Cmd(DMA1_Stream0, DISABLE);
	DMA1_Stream0->M0AR = (uint32_t)Addr;
//	DMA_MemoryTargetConfig(DMA1_Stream0, (uint32_t)Addr, DMA_Memory_0);
}

static inline void QBL_I2C_DMA_SetPeripheralAddr(uint32_t Addr)
{
	DMA_Cmd(DMA1_Stream0, DISABLE);
	DMA1_Stream0->PAR = Addr;
}

static inline void QBL_I2C_DMA_SetBufferSize(uint16_t size)
{
	DMA_Cmd(DMA1_Stream0, DISABLE);
	DMA1_Stream0->NDTR = size;
//	DMA_SetCurrDataCounter(DMA1_Stream0, size);
}
#endif

QBL_STATUS QBL_I2C_Init(void)
{
	QBL_I2C_Config();

#ifdef USE_DMA_I2C1
	QBL_I2C_DMA_Config();
#endif

#ifdef USE_DMA_I2C1_INTERRUPT
	QBL_I2C_DMA_NVIC_Config(); // 这个顺序很重要
	QBL_I2C_DMA_Config();
#endif

	return QBL_OK;
}

QBL_STATUS QBL_I2C_Mem_Write(const uint32_t QBL_I2C_Base, const uint8_t DevAddr, const uint8_t RegAddr, const uint8_t* Value, uint8_t Length, uint16_t TimeOut)
{
    uint32_t max_time = TimeOut + QBL_GetTick();

    //检测总线是否为忙状态
    while (SET == I2C_GetFlagStatus(TOINSTANCE(QBL_I2C_Base), I2C_FLAG_BUSY)) {
        if (QBL_GetTick() >= max_time) {
            return QBL_BUSY;
        }
    }

    //生成起始信号
    I2C_GenerateSTART(TOINSTANCE(QBL_I2C_Base), ENABLE);
    while (ERROR == I2C_CheckEvent(TOINSTANCE(QBL_I2C_Base), I2C_EVENT_MASTER_MODE_SELECT)) {
        if (QBL_GetTick() >= max_time) {
            return QBL_START_FAILED;
        }
    }

    //发送7位地址
    I2C_Send7bitAddress(TOINSTANCE(QBL_I2C_Base), DevAddr, I2C_Direction_Transmitter);
    while (ERROR == I2C_CheckEvent(TOINSTANCE(QBL_I2C_Base),
                        I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) {
        if (QBL_GetTick() >= max_time) {
            return QBL_ADDR_NOT_MATCH;
        }
    }

    //写寄存器地址
    I2C_SendData(TOINSTANCE(QBL_I2C_Base), RegAddr);
    while (ERROR == I2C_CheckEvent(TOINSTANCE(QBL_I2C_Base), I2C_EVENT_MASTER_BYTE_TRANSMITTED)) {
        if (QBL_GetTick() >= max_time) {
            return QBL_WRITE_ADDR_FAILED;
        }
    }

    while (Length--) {
        //向寄存器中写入数据
        I2C_SendData(TOINSTANCE(QBL_I2C_Base), *Value++);
        while (ERROR == I2C_CheckEvent(TOINSTANCE(QBL_I2C_Base), I2C_EVENT_MASTER_BYTE_TRANSMITTED)) {
            if (QBL_GetTick() >= max_time) {
                return QBL_WRITE_FAILED;
            }
        }
    }

    I2C_GenerateSTOP(TOINSTANCE(QBL_I2C_Base), ENABLE);

    return QBL_OK;
}

QBL_STATUS QBL_I2C_Mem_Read(const uint32_t QBL_I2C_Base, const uint8_t DevAddr, const uint8_t RegAddr, uint8_t* Value, uint8_t Length, uint8_t TimeOut)
{
    uint32_t max_time = TimeOut + QBL_GetTick();

    //检测总线是否为忙状态
    while (SET == I2C_GetFlagStatus(TOINSTANCE(QBL_I2C_Base), I2C_FLAG_BUSY)) {
        if (QBL_GetTick() >= max_time) {
            return QBL_BUSY;
        }
    }

    //生成起始信号
    I2C_GenerateSTART(TOINSTANCE(QBL_I2C_Base), ENABLE);
    while (ERROR == I2C_CheckEvent(TOINSTANCE(QBL_I2C_Base), I2C_EVENT_MASTER_MODE_SELECT)) {
        if (QBL_GetTick() >= max_time) {
            return QBL_START_FAILED;
        }
    }

    //发送7位地址
    I2C_Send7bitAddress(TOINSTANCE(QBL_I2C_Base), DevAddr, I2C_Direction_Transmitter);
    while (ERROR == I2C_CheckEvent(TOINSTANCE(QBL_I2C_Base),
                        I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) {
        if (QBL_GetTick() >= max_time) {
            return QBL_ADDR_NOT_MATCH;
        }
    }

    //写寄存器地址
    I2C_SendData(TOINSTANCE(QBL_I2C_Base), RegAddr);
    while (ERROR == I2C_CheckEvent(TOINSTANCE(QBL_I2C_Base), I2C_EVENT_MASTER_BYTE_TRANSMITTED)) {
        if (QBL_GetTick() >= max_time) {
            return QBL_WRITE_ADDR_FAILED;
        }
    }

    //生成起始信号
    I2C_GenerateSTART(TOINSTANCE(QBL_I2C_Base), ENABLE);
    while (ERROR == I2C_CheckEvent(TOINSTANCE(QBL_I2C_Base), I2C_EVENT_MASTER_MODE_SELECT)) {
        if (QBL_GetTick() >= max_time) {
            return QBL_START_FAILED;
        }
    }

    //发送7位地址
    I2C_Send7bitAddress(TOINSTANCE(QBL_I2C_Base), DevAddr, I2C_Direction_Receiver);
    while (ERROR == I2C_CheckEvent(TOINSTANCE(QBL_I2C_Base),
                        I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)) {
        if (QBL_GetTick() >= max_time) {
            return QBL_ADDR_NOT_MATCH;
        }
    }

#ifdef USE_BASE_I2C1
    while (Length) {

        if (Length == 1) {
            I2C_AcknowledgeConfig(TOINSTANCE(QBL_I2C_Base), DISABLE);
            I2C_GenerateSTOP(TOINSTANCE(QBL_I2C_Base), ENABLE);
        }

        while (ERROR == I2C_CheckEvent(TOINSTANCE(QBL_I2C_Base), I2C_EVENT_MASTER_BYTE_RECEIVED)) {
            if (QBL_GetTick() >= max_time) {
                return QBL_RECEIVE_FAILED;
            }
        }

        *Value++ = I2C_ReceiveData(TOINSTANCE(QBL_I2C_Base));
        Length--;
    }

	I2C_AcknowledgeConfig(TOINSTANCE(QBL_I2C_Base), ENABLE);

	return QBL_OK;
#endif

#ifdef USE_DMA_I2C1
	{
		QBL_I2C_DMA_SetBufferSize((uint16_t)Length);
		QBL_I2C_DMA_SetPeripheralAddr(I2C_Register_DR+QBL_I2C_Base);
		QBL_I2C_DMA_SetMemoryAddr(Value);

		//使能DMA自动产生非应答
		I2C_DMALastTransferCmd(I2C1, ENABLE);

		I2C_DMACmd(TOINSTANCE(QBL_I2C_Base), ENABLE);
		DMA_Cmd(DMA1_Stream0, ENABLE);

		while ((DMA_GetCmdStatus(DMA1_Stream0)!= ENABLE)) {
			if (QBL_GetTick() >= max_time) {
				return QBL_WRITE_ADDR_FAILED;
			}
		}

		while (DMA_GetFlagStatus(DMA1_Stream0,DMA_FLAG_TCIF0)==RESET) {
			if (QBL_GetTick() >= max_time) {
				return QBL_WRITE_ADDR_FAILED;
			}
		}

		I2C_GenerateSTOP(TOINSTANCE(QBL_I2C_Base), ENABLE);

		DMA_Cmd(DMA1_Stream0, DISABLE);

		while ((DMA_GetCmdStatus(DMA1_Stream0)!= DISABLE)) {
			if (QBL_GetTick() >= max_time) {
				return QBL_WRITE_ADDR_FAILED;
			}
		}

		I2C_DMACmd(TOINSTANCE(QBL_I2C_Base), DISABLE);

		DMA_ClearFlag(DMA1_Stream0, DMA_FLAG_TCIF0 | DMA_FLAG_FEIF0 | DMA_FLAG_DMEIF0 | \
                                       DMA_FLAG_TEIF0 | DMA_FLAG_HTIF0);
	}

	return QBL_OK;
#endif

#ifdef USE_DMA_I2C1_INTERRUPT
	{
		QBL_I2C_DMA_SetBufferSize((uint16_t)Length);
		QBL_I2C_DMA_SetPeripheralAddr(I2C_Register_DR+QBL_I2C_Base);
		QBL_I2C_DMA_SetMemoryAddr(Value);
		
		I2C_DMA_DATA_DataStampRef ++;

		//使能DMA自动产生非应答
		I2C_DMALastTransferCmd(I2C1, ENABLE);

		I2C_DMACmd(TOINSTANCE(QBL_I2C_Base), ENABLE);
		DMA_Cmd(DMA1_Stream0, ENABLE);

		while ((DMA_GetCmdStatus(DMA1_Stream0)!= ENABLE)) {
			if (QBL_GetTick() >= max_time) {
				return QBL_WRITE_ADDR_FAILED;
			}
		}

		/* 接下来判断是否完成接收交给DMA中断来判断 */
		DMA_ITConfig(DMA1_Stream0, DMA_IT_TC, ENABLE);
	}
	return QBL_OK;
#endif
}

QBL_STATUS QBL_I2C_Write(const uint32_t QBL_I2C_Base, const uint8_t DevAddr, const uint8_t* Data, uint8_t Length, uint8_t TimeOut)
{
    uint32_t max_time = TimeOut + QBL_GetTick();

    //检测总线是否为忙状态
    while (SET == I2C_GetFlagStatus(TOINSTANCE(QBL_I2C_Base), I2C_FLAG_BUSY)) {
        if (QBL_GetTick() >= max_time) {
            return QBL_BUSY;
        }
    }

    //生成起始信号
    I2C_GenerateSTART(TOINSTANCE(QBL_I2C_Base), ENABLE);
    while (ERROR == I2C_CheckEvent(TOINSTANCE(QBL_I2C_Base), I2C_EVENT_MASTER_MODE_SELECT)) {
        if (QBL_GetTick() >= max_time) {
            return QBL_START_FAILED;
        }
    }

    //发送7位地址
    I2C_Send7bitAddress(TOINSTANCE(QBL_I2C_Base), DevAddr, I2C_Direction_Transmitter);
    while (ERROR == I2C_CheckEvent(TOINSTANCE(QBL_I2C_Base),
                        I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) {
        if (QBL_GetTick() >= max_time) {
            return QBL_ADDR_NOT_MATCH;
        }
    }

    while (Length--) {
        //向寄存器中写入数据
        I2C_SendData(TOINSTANCE(QBL_I2C_Base), *Data++);
        while (ERROR == I2C_CheckEvent(TOINSTANCE(QBL_I2C_Base), I2C_EVENT_MASTER_BYTE_TRANSMITTED)) {
            if (QBL_GetTick() >= max_time) {
                return QBL_WRITE_FAILED;
            }
        }
    }

    I2C_GenerateSTOP(TOINSTANCE(QBL_I2C_Base), ENABLE);
    return QBL_OK;
}

QBL_STATUS QBL_I2C_Read(const uint32_t QBL_I2C_Base, const uint8_t DevAddr, uint8_t* Data, uint8_t Length, uint8_t TimeOut)
{
    uint32_t max_time = TimeOut + QBL_GetTick();

    //检测总线是否为忙状态
    while (SET == I2C_GetFlagStatus(TOINSTANCE(QBL_I2C_Base), I2C_FLAG_BUSY)) {
        if (QBL_GetTick() >= max_time) {
            return QBL_BUSY;
        }
    }

    //生成起始信号
    I2C_GenerateSTART(TOINSTANCE(QBL_I2C_Base), ENABLE);
    while (ERROR == I2C_CheckEvent(TOINSTANCE(QBL_I2C_Base), I2C_EVENT_MASTER_MODE_SELECT)) {
        if (QBL_GetTick() >= max_time) {
            return QBL_START_FAILED;
        }
    }

    //发送7位地址
    I2C_Send7bitAddress(TOINSTANCE(QBL_I2C_Base), DevAddr, I2C_Direction_Transmitter);
    while (ERROR == I2C_CheckEvent(TOINSTANCE(QBL_I2C_Base),
                        I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) {
        if (QBL_GetTick() >= max_time) {
            return QBL_ADDR_NOT_MATCH;
        }
    }

    //生成起始信号
    I2C_GenerateSTART(TOINSTANCE(QBL_I2C_Base), ENABLE);
    while (ERROR == I2C_CheckEvent(TOINSTANCE(QBL_I2C_Base), I2C_EVENT_MASTER_MODE_SELECT)) {
        if (QBL_GetTick() >= max_time) {
            return QBL_START_FAILED;
        }
    }

    //发送7位地址
    I2C_Send7bitAddress(TOINSTANCE(QBL_I2C_Base), DevAddr, I2C_Direction_Receiver);
    while (ERROR == I2C_CheckEvent(TOINSTANCE(QBL_I2C_Base),
                        I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)) {
        if (QBL_GetTick() >= max_time) {
            return QBL_ADDR_NOT_MATCH;
        }
    }

    while (Length) {
        while (ERROR == I2C_CheckEvent(TOINSTANCE(QBL_I2C_Base), I2C_EVENT_MASTER_BYTE_RECEIVED)) {
            if (QBL_GetTick() >= max_time) {
                return QBL_RECEIVE_FAILED;
            }
        }

        *Data++ = I2C_ReceiveData(TOINSTANCE(QBL_I2C_Base));

        if (Length == 1) {
            I2C_AcknowledgeConfig(TOINSTANCE(QBL_I2C_Base), DISABLE);
        }
        Length--;
    }

    I2C_GenerateSTOP(TOINSTANCE(QBL_I2C_Base), ENABLE);
    I2C_AcknowledgeConfig(TOINSTANCE(QBL_I2C_Base), ENABLE);

    return QBL_OK;
}
