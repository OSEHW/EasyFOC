/*
 * Motors.c
 *
 *  Created on: 2024Äê9ÔÂ18ÈÕ
 *      Author: timmo
 */
#include "Motors.h"
#include "FOC.h"
#include "ch32v20x_rcc.h"
void MotorInit() {
    GPIO_InitTypeDef GPIO_InitStructure = { 0 };
    TIM_OCInitTypeDef TIM_OCInitStructure = { 0 };
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure = { 0 };

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);


    TIM_TimeBaseInitStructure.TIM_Period = 10000;
    TIM_TimeBaseInitStructure.TIM_Prescaler = 8 - 1;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;

    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 5000;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC1Init(TIM2, &TIM_OCInitStructure);
    TIM_OC2Init(TIM2, &TIM_OCInitStructure);
    TIM_OC3Init(TIM2, &TIM_OCInitStructure);

    TIM_CtrlPWMOutputs(TIM2, ENABLE);

    TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Disable);
    TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Disable);
    TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Disable);

    TIM_ARRPreloadConfig(TIM2, ENABLE);

    TIM_Cmd(TIM2, ENABLE);
}

void MotorPWMUpdate(float u, float v, float w) {
    TIM_SetCompare1(TIM2, (u / VBAT) * (10000));
    TIM_SetCompare2(TIM2, (v / VBAT) * (10000));
    TIM_SetCompare3(TIM2, (w / VBAT) * (10000));
//    printf("%f %f %f\r\n",(u / VBAT) * (10000),(v / VBAT) * (10000),(w / VBAT) * (10000));
}

