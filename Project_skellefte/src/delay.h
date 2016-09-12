/*
 * delay.h
 *
 *  Created on: Mar 9, 2016
 *      Author: davbom-2
 */

#ifndef DELAY_H_
#define DELAY_H_

void TM_Delay_Init(void);
void TM_DelayMicros(uint32_t micros);
void TM_DelayMillis(uint32_t millis);

#endif /* DELAY_H_ */
