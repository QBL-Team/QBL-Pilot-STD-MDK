#ifndef QBL_USART1_H
#define QBL_USART1_H

#include <stdbool.h>
#include "stm32f4xx_usart.h"

#define USE_DMA_USART1_TRANSMIT

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup qbl_spi QBL USART1
 * @{
 */


extern void QBL_USART1_Init(void);

#ifdef USE_DMA_USART1_TRANSMIT
extern bool QBL_USART1_DMA_SendBuffer(uint8_t* buffer, uint32_t sendSize,  uint16_t TimeOut);
#endif

uint16_t Fetch_USART1_Data(char *data, uint16_t length, uint8_t ReadDirection, bool flag_moveReadIndex);


/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif // QBL_USART1_H
