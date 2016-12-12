#include "QBL_Type.h"
#include "QBL_SysTick.h"
#include "QBL_USART1.h"
#include "QBL_SPI.h"
#include <stdio.h>
#include "W25QXX.h"

#define  FLASH_WriteAddress     0x00000
#define  BufferSize (sizeof(Tx_Buffer)-1)

uint8_t Tx_Buffer[] = " W25QXX test is scuccessful ";
uint8_t Rx_Buffer[BufferSize];


void test_W25QXX_main(void)
{
	SysTick_Init();
	QBL_SPI_Init();
	QBL_USART1_Init();

	uint32_t status = W25Q_Init();
	if (QBL_OK!= status) {
		printf("status=%d\n", status);
		while(1) {;}
	}

	printf("the test of the W25QXX\n");
	W25Q_Write((uint32_t)Tx_Buffer, FLASH_WriteAddress, BufferSize);
	W25Q_Read((uint32_t)Rx_Buffer, FLASH_WriteAddress, BufferSize);
	printf("\n%s",Rx_Buffer);
}
