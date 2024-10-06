/*
 * time.c
 *
 *  Created on: 2024��9��18��
 *      Author: timmo
 */

#include "../../Devices/Time/time.h"
#include "debug.h"

void SystemTimeInit(){
    SysTick->CTLR &= ~(1<<31);  //�ر�����ж�
    SysTick->CTLR |= (1<<5);    //���¼�������ֵ
    SysTick->CTLR &= ~(1<<4);   //���ϼ���
    SysTick->CTLR &= ~(1<<3);   //����CMP֮������
    SysTick->CTLR &= ~(1<<2);   //ʱ��=HCLK/8
    SysTick->CTLR &= ~(1<<1);   //�رռ������ж�
    SysTick->CTLR |= (1<<0);    //����ϵͳ��ʱ��
}


uint64_t get_micros(){
//    uint32_t * timel =(uint32_t *)0xE000F008;
//    uint32_t * timeh =(uint32_t *)0xE000F00C;
//    uint32_t coretime = 0;
    uint64_t * time1 =(uint64_t *)0xE000F008;
//    printf("time = %lld",*time1);
//    coretime= (*timel)/(1*1000);
//    printf("h=%x,l=%x,coretime = %d",*timeh,*timel,(coretime));
    return *time1;
}
