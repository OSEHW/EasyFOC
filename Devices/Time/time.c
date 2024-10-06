/*
 * time.c
 *
 *  Created on: 2024年9月18日
 *      Author: timmo
 */

#include "../../Devices/Time/time.h"
#include "debug.h"

void SystemTimeInit(){
    SysTick->CTLR &= ~(1<<31);  //关闭软件中断
    SysTick->CTLR |= (1<<5);    //更新计数器初值
    SysTick->CTLR &= ~(1<<4);   //向上计数
    SysTick->CTLR &= ~(1<<3);   //到达CMP之后不重载
    SysTick->CTLR &= ~(1<<2);   //时基=HCLK/8
    SysTick->CTLR &= ~(1<<1);   //关闭计数器中断
    SysTick->CTLR |= (1<<0);    //启动系统定时器
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
