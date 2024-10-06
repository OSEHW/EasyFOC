/*
 * FOC.c
 *
 *  Created on: 2024年9月16日
 *      Author: timmo
 */

#include "FOC.h"
#include "FastMath.h"
#include "time.h"
#include "debug.h"

static unsigned long open_loop_timestamp = 0;
float shaft_angle = 0;
/*
 函数名称：electricAngle
 函数功能：由机械角度求解电角度
 函数参数：
 float shaftAngle    机械角度
 函数返回：float 电角度
 函数备注：无
 */
float electricAngle(float shaftAngle) {
    return (shaftAngle * POLAR_PAIRS);
}

/*
 函数名称：normalizeAngle
 函数功能：归一化角度
 函数参数：
 float angle    待求角度
 函数返回：float 归一化后角度
 函数备注：无
 */
float normalizeAngle(float angle) {
    float a = (angle - (int) (angle / (2 * PI)) * (2 * PI));
    return a >= 0 ? a : (a + 2 * PI);
}

/*
 函数名称：foc_calculate
 函数功能：foc 计算
 函数参数：
 float q 帕克变换参数q
 float d 帕克变化参数d
 float angle 目标角度
 float *u    克拉克变换参数u的指针
 float *v    克拉克变换参数v的指针
 float *w    克拉克变换参数w的指针
 函数返回：无
 函数备注：无
 */

void foc_calculate(float q, float d, float angle, float *u, float *v, float *w) {
    printf("q = %f d = %f angle = %f u = %f v = %f w = %f\r\n",q,d,angle,*u,*v,*w);
    //帕克逆变换
    float alpha = -q * fast_sin(normalizeAngle(angle));
    float beta = q * fast_cos(normalizeAngle(angle));
    //克拉克变换
    *u = (((alpha + VBAT / 2)>VBAT) || ((alpha + VBAT / 2)<0))?*u:(alpha + VBAT / 2);
    *v = ((((SQRT3 * beta - alpha) / 2 + VBAT / 2)>VBAT) || (((SQRT3 * beta - alpha) / 2 + VBAT / 2)<0))?*v:((SQRT3 * beta - alpha) / 2 + VBAT / 2);
    *w = ((((-SQRT3 * beta - alpha) / 2 + VBAT / 2)>VBAT) || (((-SQRT3 * beta - alpha) / 2 + VBAT / 2)<0))?*w:((-SQRT3 * beta - alpha) / 2 + VBAT / 2);
//    printf("u=%f v=%f w=%f \r\n",*u,*v,*w);
}

void velocityOpenloop(float target_velocity, float *u, float *v, float *w) {
    uint64_t now_us = get_micros() / 1000;
//    printf("now_us = %lld",now_us);
    float Ts = (now_us - open_loop_timestamp)/(1.0*1000*1000);  //time(s)
//    printf("Ts = %f\r\n",Ts);
    shaft_angle = normalizeAngle(shaft_angle + target_velocity * Ts);
//    printf("shaft_angle=%f\r\n",shaft_angle);
    float Uq = VBAT / 3;

    foc_calculate(Uq, 0, electricAngle(shaft_angle), u, v, w);

    open_loop_timestamp = now_us;
}
