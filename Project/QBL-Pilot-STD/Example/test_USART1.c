#include <stdio.h>
#include "QBL_USART1.h"

void test_USART1_main(void)
{
	QBL_USART1_Init();

#ifdef USE_DMA_USART1_TRANSMIT
	uint8_t buffer[10] = "123456789";
	QBL_USART1_DMA_SendBuffer(buffer, 9, 2);
#endif

	printf("hello world!\n");
}
