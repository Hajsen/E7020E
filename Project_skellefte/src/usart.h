/*
 * usart.h
 *
 *  Created on: Mar 3, 2016
 *      Author: axehal-2
 */

#ifndef USART_H_
#define USART_H_


char input[50];
extern int startupCount;


void setLEDOnState();
void setLEDOffState();
void setLEDBlinkState(int frequency);
void runLEDBlinkStartup();
void initUSARTTimer();
void runUSARTStartUp();
void initializeSerial();
void send_USART_String(const char *str);
void getUSARTTemperature();




#endif /* USART_H_ */
