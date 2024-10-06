/*
 * FOC.c
 *
 *  Created on: 2024��9��16��
 *      Author: timmo
 */

#include "FOC.h"
#include "FastMath.h"
#include "time.h"
#include "debug.h"

static unsigned long open_loop_timestamp = 0;
float shaft_angle = 0;
/*
 �������ƣ�electricAngle
 �������ܣ��ɻ�е�Ƕ�����Ƕ�
 ����������
 float shaftAngle    ��е�Ƕ�
 �������أ�float ��Ƕ�
 ������ע����
 */
float electricAngle(float shaftAngle) {
    return (shaftAngle * POLAR_PAIRS);
}

/*
 �������ƣ�normalizeAngle
 �������ܣ���һ���Ƕ�
 ����������
 float angle    ����Ƕ�
 �������أ�float ��һ����Ƕ�
 ������ע����
 */
float normalizeAngle(float angle) {
    float a = (angle - (int) (angle / (2 * PI)) * (2 * PI));
    return a >= 0 ? a : (a + 2 * PI);
}

/*
 �������ƣ�foc_calculate
 �������ܣ�foc ����
 ����������
 float q ���˱任����q
 float d ���˱仯����d
 float angle Ŀ��Ƕ�
 float *u    �����˱任����u��ָ��
 float *v    �����˱任����v��ָ��
 float *w    �����˱任����w��ָ��
 �������أ���
 ������ע����
 */

void foc_calculate(float q, float d, float angle, float *u, float *v, float *w) {
    printf("q = %f d = %f angle = %f u = %f v = %f w = %f\r\n",q,d,angle,*u,*v,*w);
    //������任
    float alpha = -q * fast_sin(normalizeAngle(angle));
    float beta = q * fast_cos(normalizeAngle(angle));
    //�����˱任
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
