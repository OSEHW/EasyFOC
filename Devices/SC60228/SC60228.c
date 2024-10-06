/*
 * SC60228.c
 *
 *  Created on: 2024Äê9ÔÂ19ÈÕ
 *      Author: timmo
 */

#include "SC60228.h"

void SC60228_Init(void) {
    GPIO_InitTypeDef GPIO_InitStructure = { 0 };
    SPI_InitTypeDef SPI_InitStructure = { 0 };

    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA | RCC_APB2Periph_SPI1, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init( GPIOA, &GPIO_InitStructure);
    GPIO_WriteBit(GPIOA, GPIO_Pin_4, RESET);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init( GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init( GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init( GPIOA, &GPIO_InitStructure);

    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;

    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;

    SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_LSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_Init( SPI1, &SPI_InitStructure);

    SPI_Cmd( SPI1, ENABLE);
}

uint16_t SC60228_Get(void) {
    uint16_t ret = 0;
    if (SPI_I2S_GetFlagStatus( SPI1, SPI_I2S_FLAG_TXE) != RESET) {
//        printf("spi send data\r\n");
        SPI_I2S_SendData( SPI1, 0xAAAA);
    }
    if (SPI_I2S_GetFlagStatus( SPI1, SPI_I2S_FLAG_RXNE) != RESET) {
//        printf("spi receive data\r\n");
        ret = SPI_I2S_ReceiveData( SPI1);
    }
    return ret;
}
