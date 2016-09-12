/*
 * main.h
 *
 *  Created on: Feb 22, 2016
 *      Author: wilmok-2
 */

#ifndef MAIN_H_
#define MAIN_H_
void initializeLED();
void powerEnable();
void tim2Settings();
void usart2GPIOSettings();
void usart2Settings();
void ledSettings();
void NVICSetup();
void setLEDPeriod();
void setLEDPeriod(int period);
void enableLEDTimer();
void disableLEDTimer();
void disableLED();
void enableLED();
void tim5Settings();
void startup();
long convertStringToLong(char input[16]);

#define TRUE  	1;
#define FALSE  	0;

#endif /* MAIN_H_ */
