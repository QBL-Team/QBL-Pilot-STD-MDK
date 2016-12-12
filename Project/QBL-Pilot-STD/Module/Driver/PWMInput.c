/*!
 *
 * \file   PWMInput.c
 * \author no1wudi
 */

#include "PWMInput.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_tim.h"
#include "misc.h"
#include "LED.h"

void TIM2_IRQHandler(void)
{
    if (TIM2->SR & TIM_IT_CC1) {
        TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);
        return;
    }

    if (TIM2->SR & TIM_IT_CC2) {
        TIM_ClearITPendingBit(TIM2, TIM_IT_CC2);
        return;
    }

    if (TIM2->SR & TIM_IT_CC3) {
        TIM_ClearITPendingBit(TIM2, TIM_IT_CC3);
        return;
    }

    if (TIM2->SR & TIM_IT_CC4) {
        TIM_ClearITPendingBit(TIM2, TIM_IT_CC4);
        return;
    }
}

void TIM4_IRQHandler(void)
{
    if (TIM4->SR & TIM_IT_CC1) {
        TIM_ClearITPendingBit(TIM4, TIM_IT_CC1);
        return;
    }

    if (TIM4->SR & TIM_IT_CC2) {
        TIM_ClearITPendingBit(TIM4, TIM_IT_CC2);
        return;
    }

    if (TIM4->SR & TIM_IT_CC3) {
        TIM_ClearITPendingBit(TIM4, TIM_IT_CC3);
        return;
    }

    if (TIM4->SR & TIM_IT_CC4) {
        TIM_ClearITPendingBit(TIM4, TIM_IT_CC4);
        return;
    }
}

QBL_STATUS PWMInput_Init()
{
    GPIO_InitTypeDef io;
    TIM_TimeBaseInitTypeDef tb;
    TIM_ICInitTypeDef ti;
    NVIC_InitTypeDef n;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

    io.GPIO_Mode = GPIO_Mode_AF;
    io.GPIO_OType = GPIO_OType_PP;
    io.GPIO_PuPd = GPIO_PuPd_NOPULL;
    io.GPIO_Speed = GPIO_Speed_100MHz;

    /**TIM2 GPIO Configuration
    PA0-WKUP     ------> TIM2_CH1
    PA1     ------> TIM2_CH2
    PA2     ------> TIM2_CH3
    PA3     ------> TIM2_CH4
    */

    io.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
    GPIO_Init(GPIOA, &io);

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_TIM2);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_TIM2);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_TIM2);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_TIM2);

    /**TIM4 GPIO Configuration
    PD12     ------> TIM4_CH1
    PD13     ------> TIM4_CH2
    PD14     ------> TIM4_CH3
    PD15     ------> TIM4_CH4
    */

    io.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_Init(GPIOD, &io);

    GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_TIM4);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource13, GPIO_AF_TIM4);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_TIM4);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_TIM4);

    //Time base
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

    tb.TIM_ClockDivision = TIM_CKD_DIV1;
    tb.TIM_CounterMode = TIM_CounterMode_Up;
    tb.TIM_Period = 2499;
    tb.TIM_Prescaler = 83;
    tb.TIM_RepetitionCounter = 0;

    TIM_TimeBaseInit(TIM2, &tb);
    TIM_TimeBaseInit(TIM4, &tb);

    ti.TIM_ICFilter = 0;
    ti.TIM_ICPolarity = TIM_ICPolarity_Rising;
    ti.TIM_ICPrescaler = TIM_ICPSC_DIV1;
    ti.TIM_ICSelection = TIM_ICSelection_DirectTI;
    ti.TIM_Channel = TIM_Channel_1;
    TIM_ICInit(TIM2, &ti);
    TIM_ICInit(TIM4, &ti);

    ti.TIM_Channel = TIM_Channel_2;
    TIM_ICInit(TIM2, &ti);
    TIM_ICInit(TIM4, &ti);

    ti.TIM_Channel = TIM_Channel_3;
    TIM_ICInit(TIM2, &ti);
    TIM_ICInit(TIM4, &ti);

    ti.TIM_Channel = TIM_Channel_4;
    TIM_ICInit(TIM2, &ti);
    TIM_ICInit(TIM4, &ti);

    TIM_Cmd(TIM2, ENABLE);
    TIM_Cmd(TIM4, ENABLE);

    n.NVIC_IRQChannel = TIM2_IRQn;
    n.NVIC_IRQChannelCmd = ENABLE;
    n.NVIC_IRQChannelPreemptionPriority = 1;
    n.NVIC_IRQChannelSubPriority = 0;
    NVIC_Init(&n);

    n.NVIC_IRQChannel = TIM4_IRQn;
    NVIC_Init(&n);

    TIM_ITConfig(TIM2, TIM_IT_CC1, ENABLE);
    TIM_ITConfig(TIM2, TIM_IT_CC2, ENABLE);
    TIM_ITConfig(TIM2, TIM_IT_CC3, ENABLE);
    TIM_ITConfig(TIM2, TIM_IT_CC4, ENABLE);

    TIM_ITConfig(TIM4, TIM_IT_CC1, ENABLE);
    TIM_ITConfig(TIM4, TIM_IT_CC2, ENABLE);
    TIM_ITConfig(TIM4, TIM_IT_CC3, ENABLE);
    TIM_ITConfig(TIM4, TIM_IT_CC4, ENABLE);

    return QBL_OK;
}
