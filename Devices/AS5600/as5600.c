/*
 * as5600.c
 *
 *  Created on: 2024Äê9ÔÂ22ÈÕ
 *      Author: timmo
 */


#include "as5600.h"

static void AS5600_WaitEvent(I2C_TypeDef* I2Cx, uint32_t I2C_EVENT) {
    uint32_t Timeout = 10000;
    while (I2C_CheckEvent(I2Cx, I2C_EVENT) != SUCCESS)
    {
        Timeout --;
        if (Timeout == 0)
        {
            break;
        }
    }
}

void AS5600_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure={0};
    I2C_InitTypeDef I2C_InitTSturcture={0};

    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB , ENABLE );
    RCC_APB1PeriphClockCmd( RCC_APB1Periph_I2C1, ENABLE );

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init( GPIOB, &GPIO_InitStructure );

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init( GPIOB, &GPIO_InitStructure );

    I2C_InitTSturcture.I2C_ClockSpeed =  100 * 1000;
    I2C_InitTSturcture.I2C_Mode = I2C_Mode_I2C;
    I2C_InitTSturcture.I2C_DutyCycle = I2C_DutyCycle_16_9;
    I2C_InitTSturcture.I2C_OwnAddress1 = 0x00;
    I2C_InitTSturcture.I2C_Ack = I2C_Ack_Enable;
    I2C_InitTSturcture.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_Init( I2C1, &I2C_InitTSturcture );

    I2C_Cmd( I2C1, ENABLE );
}


uint16_t AS5600_ReadRawAngle(void) {
    uint8_t Data1;
    uint8_t Data2;
    I2C_GenerateSTART(I2C1, ENABLE);    //Start_Event
    AS5600_WaitEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT);   //Event_5

    I2C_Send7bitAddress(I2C1, AS5600_ADDR, I2C_Direction_Transmitter);  //Fram_Head
    AS5600_WaitEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED); //Event_9

    I2C_SendData(I2C1, 0x0C);   //addr
    AS5600_WaitEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED);  //Event_6

    I2C_GenerateSTART(I2C1, ENABLE);    //ReStart
    AS5600_WaitEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT);   //Event_5

    I2C_Send7bitAddress(I2C1, AS5600_ADDR, I2C_Direction_Receiver); //Fram_Head
    AS5600_WaitEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED);    //Event_6

    I2C_AcknowledgeConfig(I2C1, ENABLE);    //Non_ACK
    I2C_GenerateSTOP(I2C1, ENABLE); //Stop_Event

    AS5600_WaitEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED); //Event_7
    Data1 = I2C_ReceiveData(I2C1);   //Read_Data

    I2C_AcknowledgeConfig(I2C1, ENABLE);    //ACK

    I2C_GenerateSTART(I2C1, ENABLE);    //Start_Event
    AS5600_WaitEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT);   //Event_5

    I2C_Send7bitAddress(I2C1, AS5600_ADDR, I2C_Direction_Transmitter);  //Fram_Head
    AS5600_WaitEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED); //Event_9

    I2C_SendData(I2C1, 0x0D);   //addr
    AS5600_WaitEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED);  //Event_6

    I2C_GenerateSTART(I2C1, ENABLE);    //ReStart
    AS5600_WaitEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT);   //Event_5

    I2C_Send7bitAddress(I2C1, AS5600_ADDR, I2C_Direction_Receiver); //Fram_Head
    AS5600_WaitEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED);    //Event_6

    I2C_AcknowledgeConfig(I2C1, ENABLE);    //Non_ACK
    I2C_GenerateSTOP(I2C1, ENABLE); //Stop_Event

    AS5600_WaitEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED); //Event_7
    Data2 = I2C_ReceiveData(I2C1);   //Read_Data

    I2C_AcknowledgeConfig(I2C1, ENABLE);    //ACK

    return ((Data1<<8)|Data2);
}
