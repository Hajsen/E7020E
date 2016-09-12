/*
 * transceiver.h
 *
 *  Created on: Mar 9, 2016
 *      Author: davbom-2
 */

#ifndef TRANSCEIVER_H_
#define TRANSCEIVER_H_

void init433Transceiver();
void sendAnalogOne();
void sendStartBit();
void sendStopBit();
void sendAnalogZero();
void sendDigitalOne();
void sendDigitalZero();
void sendPhysicalSequence(char* sequence); //binary
void sendSequence(char* sequence);		   //hexa
#endif /* TRANSCEIVER_H_ */
