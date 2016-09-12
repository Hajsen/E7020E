/*
 * transceiver.c
 *
 *  Created on: Mar 9, 2016
 *      Author: davbom-2
 */
#ifndef stm32f4xx_h
#define stm32f4xx_h
#include "stm32f4xx.h"
#endif
#include "transceiver.h"
#include "currentSwitch.h"
#include "delay.h"
#include "hexamapping.h"

void init433Transceiver()
{
	TM_Delay_Init();
	initTransceiverGPIO();
	//enable GPIO

}

void sendAnalogOne()
{
	setTransceiverHigh();
	TM_DelayMicros(T);
	setTransceiverLow();
	TM_DelayMicros(5*T);
}

void sendAnalogZero()
{
	setTransceiverHigh();
		TM_DelayMicros(T);
		setTransceiverLow();
		TM_DelayMicros(T);
}

void sendStartBit()
{
	setTransceiverHigh();
	TM_DelayMicros(T);
	setTransceiverLow();
	TM_DelayMicros(10*T);
}
void sendStopBit()
{
	setTransceiverHigh();
	TM_DelayMicros(T);
	setTransceiverLow();
	TM_DelayMicros(40*T);
}

void sendDigitalOne()
{
	sendAnalogOne();
	sendAnalogZero();
}
void sendDigitalZero()
{
	sendAnalogZero();
	sendAnalogOne();
}
void sendPhysicalSequence(char* sequence) // Sends a binarysequence from physical 433Mhz-sender
{
	sendStartBit();
	int i = 0;
	for (i = 0; i < strlen(sequence); i++)
	{
		if (sequence[i] == '1')
		{
			sendDigitalOne();
		}
		else if (sequence[i] == '0')
		{
			sendDigitalZero();
		}
	}
	sendStopBit();
}
/*
 * remote control mapping 1: ,2263FB9F EEEE = 0xF
 * remote control mapping 2: ,2263FB9E EEEE = 0xE
 * remote control mapping 3: ,2263FB9D EEEE = 0xD
 *
*/
void sendSequence(char* sequence)		//send 433mhz digital hexasequence, sequence in hexadecimal-format // 0bHHHHHHHHHHHHHHHHHHHHHHHHHHGOEEEE
{
	char* binary = convertFromHexToBin(sequence);
	sendPhysicalSequence(binary);
	TM_DelayMillis(500);
	sendPhysicalSequence(binary);
	TM_DelayMillis(500);
	sendPhysicalSequence(binary);
	free(binary);
}



