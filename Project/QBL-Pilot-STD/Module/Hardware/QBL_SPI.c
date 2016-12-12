/**
 *\file QBL_SPI.c
 *\author no1wudi
 */

#include "QBL_Type.h"
#include "QBL_SPI.h"
#include "QBL_SysTick.h"
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_spi.h"

#define TOINSTANCE(QBL_SPI_Base) ((SPI_TypeDef*)QBL_SPI_Base) /**< 实例化模块 */

QBL_STATUS QBL_SPI_Init()
{
    GPIO_InitTypeDef io;
    SPI_InitTypeDef sp;

	{
		//SPI2用于MS5611
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

		GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_SPI2);
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource14, GPIO_AF_SPI2);
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_SPI2);

		io.GPIO_Mode = GPIO_Mode_AF;
		io.GPIO_OType = GPIO_OType_PP;
		io.GPIO_PuPd = GPIO_PuPd_UP;
		io.GPIO_Speed = GPIO_Speed_100MHz;
		io.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
		GPIO_Init(GPIOB, &io);

		RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);

		SPI_DeInit(SPI2);
		sp.SPI_Mode = SPI_Mode_Master;
		sp.SPI_CRCPolynomial = 7;
		sp.SPI_DataSize = SPI_DataSize_8b;
		sp.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
		sp.SPI_FirstBit = SPI_FirstBit_MSB;
		sp.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;//10.5MHz
		sp.SPI_CPHA = SPI_CPHA_1Edge;
		sp.SPI_CPOL = SPI_CPOL_Low;
		sp.SPI_NSS = SPI_NSS_Soft;

		SPI_Init(SPI2, &sp);
		SPI_Cmd(SPI2, ENABLE);
	}

	{
		//SPI1 用于W25Q16
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

		GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_SPI1);
		GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_SPI1);
		GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_SPI1);

		io.GPIO_Mode = GPIO_Mode_AF;
		io.GPIO_Speed = GPIO_Speed_100MHz;
		io.GPIO_OType = GPIO_OType_PP;
		io.GPIO_PuPd = GPIO_PuPd_UP;
		io.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
		GPIO_Init(GPIOA, &io);

		RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

		SPI_DeInit(SPI1);
		sp.SPI_Mode = SPI_Mode_Master;
		sp.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
		sp.SPI_CPOL = SPI_CPOL_Low;
		sp.SPI_CPHA = SPI_CPHA_1Edge;
		sp.SPI_CRCPolynomial = 7;
		sp.SPI_DataSize = SPI_DataSize_8b;
		sp.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
		sp.SPI_FirstBit = SPI_FirstBit_MSB;
		sp.SPI_NSS = SPI_NSS_Soft;

		SPI_Init(SPI1, &sp);
		SPI_Cmd(SPI1, ENABLE);
	}

    return QBL_OK;
}

QBL_STATUS QBL_SPI_TransmitReceive(const uint32_t QBL_SPI_Base, const uint8_t* Trans, uint8_t* Rece, uint8_t Length, uint16_t TimeOut)
{
    uint32_t max_time = TimeOut + QBL_GetTick();

    while (Length) {

			while (RESET == SPI_I2S_GetFlagStatus(TOINSTANCE(QBL_SPI_Base), SPI_I2S_FLAG_TXE)) {
				if (QBL_GetTick() > max_time) {
					return QBL_WRITE_FAILED;
				}
			}
			if (NULL != Trans) {
				SPI_I2S_SendData(TOINSTANCE(QBL_SPI_Base), *Trans++);
			}
			else {
				SPI_I2S_SendData(TOINSTANCE(QBL_SPI_Base), 0xff);
			}

			while (RESET == SPI_I2S_GetFlagStatus(TOINSTANCE(QBL_SPI_Base), SPI_I2S_FLAG_RXNE)) {
					if (QBL_GetTick() > max_time) {
						return QBL_RECEIVE_FAILED;
				}
			}
			if (NULL != Rece) {
				*Rece++ = SPI_I2S_ReceiveData(TOINSTANCE(QBL_SPI_Base));
			}
			else {
				SPI_I2S_ReceiveData(TOINSTANCE(QBL_SPI_Base));
			}
			Length--;
    }

    return QBL_OK;
}
