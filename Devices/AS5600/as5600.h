/*
 * AS5600.h
 *
 *  Created on: 2024Äê9ÔÂ22ÈÕ
 *      Author: timmo
 */

#ifndef DEVICES_AS5600_AS5600_H_
#define DEVICES_AS5600_AS5600_H_

#include "debug.h"
#define AS5600_ADDR (0x36<<1)
#define  SUCCESS   1

void AS5600_Init(void);
uint16_t AS5600_ReadRawAngle(void);
#endif /* DEVICES_AS5600_AS5600_H_ */
