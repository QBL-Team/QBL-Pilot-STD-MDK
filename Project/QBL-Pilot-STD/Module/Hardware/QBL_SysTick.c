#include "stm32f4xx_rcc.h"
#include "QBL_SysTick.h"

static volatile uint32_t qbl_sys_ticks = 0; //系统上电计时器 单位ms

void SysTick_Init(void)
{
	NVIC_SetPriority(SysTick_IRQn, 0);

	SysTick_Config(SystemCoreClock / 1000);
}

/**
 * @brief SysTick_Handler 滴答定时器服务函数
 */
void SysTick_Handler(void)
{
    qbl_sys_ticks++;
}

uint32_t QBL_GetTick()
{
    return qbl_sys_ticks;
}

void QBL_Delay(uint32_t ms)
{
    uint32_t tick = QBL_GetTick() + ms;
    for (;;) {
        if (QBL_GetTick() >= tick) {
            break;
        }
    }
}
