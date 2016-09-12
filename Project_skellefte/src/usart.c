/*
 * usart.c
 *
 *  Created on: Mar 3, 2016
 *      Author: axehal-2
 */

#ifndef stm32f4xx_h
#define stm32f4xx_h
#include "usart.h"
#include "stm32f4xx.h"
#endif
#include <math.h>
#include "termistor.h"

int startupCount = 0;
void setLEDOnState()
{
	//disable blink.
	NVIC_DisableIRQ(TIM2_IRQn);
	//output to putty "status LED on"
	send_USART_String("\r\n status on \r\n");

	//GPIOA->ODR |= 1 << 5; // toggle LED state
}
void setLEDOffState()
{
	NVIC_DisableIRQ(TIM2_IRQn);
	send_USART_String("\r\n status off \r\n");
	//GPIOA->ODR &= ~(1 << 5); // toggle LED state
}
void setLEDBlinkState(int frequency)
{
	NVIC_DisableIRQ(TIM2_IRQn);

	//reset counter value.
	TIM2->CNT = 0;
	if(frequency==0)
	{
		TIM2->ARR = 1000000;
	}
	else
	{

		TIM2->ARR = frequency*1000;
	}

	char param_parcer[10];
	sprintf(param_parcer,"%d",frequency);
	char output[200];
	strcpy(output, "\r\n status blink ");
	strcat(output, param_parcer);
	strcat(output, " \r\n");

	send_USART_String(output);
	//NVIC_EnableIRQ(TIM2_IRQn);
	NVIC_EnableIRQ(TIM2_IRQn);
	//enable clock-interrupt
	//set clock-timer for blink
}


//blink 3 times and go to onState
void runLEDBlinkStartup()
{
	send_USART_String("System Operational");
	while(startupCount<6)
	{

	}
	//setLEDOnState();
}

void getUSARTTemperature()
{
	int temp = getSampledTemperature();
	char param_parcer[10];
	sprintf(param_parcer,"%d",temp);
	char output[200];
	strcpy(output, "\r\n Temperature is ");
	strcat(output, param_parcer);
	strcat(output, " degrees Celsius. \r\n");

	send_USART_String(output);

}


void initUSARTTimer()
{

	NVIC_EnableIRQ(TIM2_IRQn);
		//	reset enable GPIO A-pin
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	//GPIOA->MODER |= GPIO_MODER_MODER5_0;

	TIM2->PSC = 42;
	TIM2->ARR = 500000;
	TIM2->DIER |= 0x1;
	TIM2->CR1 |= 0x1;
}

void runUSARTStartUp()
{
	initUSARTTimer();
	initializeSerial();
	runLEDBlinkStartup();
}

void initializeSerial()
{
	NVIC_EnableIRQ(USART6_IRQn);

		RCC->APB2ENR |= RCC_APB2ENR_USART6EN;

		//Alternate function at PA_11	// Must connect to nucleo's RX
		GPIOA->MODER |= GPIO_MODER_MODER11_1;
		//Alternate function at PA_12	// Must connect to nucleo's TX
		GPIOA->MODER |= GPIO_MODER_MODER12_1;


	//	//Alternate function at PA_2
	//	GPIOA->MODER |= GPIO_MODER_MODER2_1;
	//	//Alternate function at PA_3
	//	GPIOA->MODER |= GPIO_MODER_MODER3_1;


		//map alternate function to USART2_TX (PA_11)

			GPIOA->AFR[1] |= (GPIO_AF_USART6 << 12);

			//map alternate function to USART2_RX (PA_12)
			GPIOA->AFR[1] |= (GPIO_AF_USART6 << 16);



	//	//map alternate function to USART2_TX (PA2)
	//	GPIOA->AFR[0] |= (7 << 8);
	//
	//	//map alternate function to USART2_RX (PA3)
	//	GPIOA->AFR[0] |= (7 << 12);
				//0xFFFF7FFF;

		// set pin switch speed to high on PA2 and PA3
		GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR11;
		GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR12;

		USART6->BRR |= 0x880; //mantissa: 0d65 // 41 << 4
		USART6->BRR |= 0x12;

		USART6->CR1 |= USART_CR1_UE;	//Enable usart6
		//USART2->CR1 |= USART_CR1_TXEIE;
		USART6->CR1 |= USART_CR1_RXNEIE;
		//USART2->CR1 &= 0xFFFFDFFF;     //USART_CR1_M; 8-bit word

		//USART2->CR2 &= 0xFFFFCFFF;		//USART_CR2_STOP  1-bit stop
		//send an idle frame as first transmission
		USART6->CR1 |= USART_CR1_RE;
		USART6->CR1 |= USART_CR1_TE;
}

void send_USART_String(const char *str)
{
    while (*str)
    {
        while(USART_GetFlagStatus(USART6, USART_FLAG_TXE) == RESET);
        USART_SendData(USART6, *str++);
    }
}
