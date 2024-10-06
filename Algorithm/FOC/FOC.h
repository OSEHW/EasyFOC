/*
 * FOC.h
 *
 *  Created on: 2024��9��16��
 *      Author: timmo
 */

#ifndef ALGORITHM_FOC_FOC_H_
#define ALGORITHM_FOC_FOC_H_
#include "debug.h"
/********** config **********/
#define POLAR_PAIRS     7       //������
#define VBAT            7.0f    //��ص�ѹ
#define PI              (3.1415926f)    //pi

float electricAngle(float shaftAngle);
void foc_calculate(float q,float d,float angle,float *u,float *v,float *w);
void velocityOpenloop(float target_velocity,float *u,float *v,float *w);

#endif /* ALGORITHM_FOC_FOC_H_ */
