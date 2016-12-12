/*!
 *
 * \file   PWMOutput.c
 * \author no1wudi
 */

#include "PWMOutput.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_tim.h"

QBL_STATUS PWMOutput_Init()
{
    GPIO_InitTypeDef io;
    TIM_TimeBaseInitTypeDef tb;
    TIM_OCInitTypeDef to;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

    /**TIM1 GPIO Configuration
    PA8     ------> TIM1_CH1
    PA9     ------> TIM1_CH2
    PA10     ------> TIM1_CH3
    PA11     ------> TIM1_CH4
    */

    io.GPIO_Mode = GPIO_Mode_AF;
    io.GPIO_OType = GPIO_OType_PP;
    io.GPIO_PuPd = GPIO_PuPd_NOPULL;
    io.GPIO_Speed = GPIO_Speed_100MHz;

    io.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11;
    GPIO_Init(GPIOA, &io);

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_TIM1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_TIM1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_TIM1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_TIM1);

    /**TIM3 GPIO Configuration
    PB0     ------> TIM3_CH3
    PB1     ------> TIM3_CH4
    PC6     ------> TIM3_CH1
    PC7     ------> TIM3_CH2
    */

    io.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_Init(GPIOC, &io);

    GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_TIM3);
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_TIM3);

    io.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_Init(GPIOB, &io);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource0, GPIO_AF_TIM3);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource1, GPIO_AF_TIM3);

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

    //定时器配置
    tb.TIM_ClockDivision = TIM_CKD_DIV1;
    tb.TIM_CounterMode = TIM_CounterMode_Up;
    tb.TIM_Period = 52499;
    tb.TIM_Prescaler = 7;
    tb.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM1, &tb);
    TIM_TimeBaseInit(TIM3, &tb);

    to.TIM_OCIdleState = TIM_OCIdleState_Reset;
    to.TIM_OCMode = TIM_OCMode_PWM1;
    to.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
    to.TIM_OCNPolarity = TIM_OCNPolarity_Low;
    to.TIM_OCPolarity = TIM_OCPolarity_High;
    to.TIM_OutputNState = TIM_OutputNState_Disable;
    to.TIM_OutputState = TIM_OutputState_Enable;
    to.TIM_Pulse = 20999;

    TIM_OC1Init(TIM1, &to);
    TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
    TIM_OC1Init(TIM3, &to);
    TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);

    TIM_OC2Init(TIM1, &to);
    TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);
    TIM_OC2Init(TIM3, &to);
    TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);

    TIM_OC3Init(TIM1, &to);
    TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);
    TIM_OC3Init(TIM3, &to);
    TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);

    TIM_OC4Init(TIM1, &to);
    TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);
    TIM_OC4Init(TIM3, &to);
    TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);

    TIM_ARRPreloadConfig(TIM1, ENABLE);
    TIM_ARRPreloadConfig(TIM3, ENABLE);

    TIM_Cmd(TIM1, ENABLE);
    TIM_Cmd(TIM3, ENABLE);

    return QBL_OK;
}

QBL_STATUS PWMOutput_Apply(uint16_t Output[8])
{
    TIM1->CCR1 = Output[0];
    TIM1->CCR2 = Output[1];
    TIM1->CCR3 = Output[2];
    TIM1->CCR4 = Output[3];

    TIM3->CCR1 = Output[4];
    TIM3->CCR2 = Output[5];
    TIM3->CCR3 = Output[6];
    TIM3->CCR4 = Output[7];

    return QBL_OK;
}
