/*
 * delay.c
 *
 *  Created on: Mar 9, 2016
 *      Author: davbom-2
 */
#ifndef stm32f4xx_h
#define stm32f4xx_h
#include "stm32f4xx.h"
#endif
#include "delay.h"

uint32_t multiplier;

void TM_Delay_Init(void) {
    RCC_ClocksTypeDef RCC_Clocks;

    /* Get system clocks */
    RCC_GetClocksFreq(&RCC_Clocks);

    /* While loop takes 7 cycles */
    /* For 1 us delay, we need to divide with 7M */

    multiplier = 84000000 / 7000000;
}

void TM_DelayMicros(uint32_t micros) {
    /* Multiply micros with multipler */
    /* Substract 10 */
    micros = micros * multiplier - 10;
    /* 4 cycles for one loop */
    while (micros--);
}

void TM_DelayMillis(uint32_t millis) {
    /* Multiply millis with multipler */
    /* Substract 10 */
    millis = 1000 * millis * multiplier - 10;
    /* 4 cycles for one loop */
    while (millis--);
}

