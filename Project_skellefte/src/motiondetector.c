/*
 * motiondetector.c
 *
 *  Created on: Mar 7, 2016
 *      Author: davbom-2
 */


#ifndef stm32f4xx_h
#define stm32f4xx_h
#include "stm32f4xx.h"
#endif
#include "motiondetector.h"
#include "remoteSwitchController.h"
#include "usart.h"
// include 433MHZ needed later when switching state

int motionLogStatus = 1;

void initTestLED()		//PA5
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	GPIOA->MODER |= GPIO_MODER_MODER5_0;
}
void initTimerMotionDetector()
{
		NVIC_EnableIRQ(TIM3_IRQn);
			//	reset enable GPIO A-pin
		RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;

		TIM3->PSC = 42000;	// counter operating at 1KHz
		TIM3->ARR = 10000-1;	// 10000
		TIM3->DIER |= 0x1;
		TIM3->CR1 |= 0x1;
	// 10 second, reset CNT value when motion detected.
}
int getMotionLogStatus()
{
	return motionLogStatus;
}
void setMotionLogStatus(int status)
{
	if(motionLogStatus == 1 || motionLogStatus==0)
	{
		motionLogStatus = status;
	}

}

//Enable an interrupt for pin PA4 (PA5)


/* Configure pins to be interrupts */
void Configure_PA5(void)
{
    /* Set variables used */
    GPIO_InitTypeDef GPIO_InitStruct;
    EXTI_InitTypeDef EXTI_InitStruct;
    NVIC_InitTypeDef NVIC_InitStruct;

    /* Enable clock for GPIOD */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    /* Enable clock for SYSCFG */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

    /* Set pin as input */
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* Tell system that you will use PD0 for EXTI_Line0 */
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource5);

    /* PD0 is connected to EXTI_Line0 */
    EXTI_InitStruct.EXTI_Line = EXTI_Line5;
    /* Enable interrupt */
    EXTI_InitStruct.EXTI_LineCmd = ENABLE;
    /* Interrupt mode */
    EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
    /* Triggers on rising and falling edge */
    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
    /* Add to EXTI */
    EXTI_Init(&EXTI_InitStruct);

    /* Add IRQ vector to NVIC */
    /* PD0 is connected to EXTI_Line0, which has EXTI0_IRQn vector */
    NVIC_InitStruct.NVIC_IRQChannel = EXTI9_5_IRQn;

    /* Set priority */
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x00;
    /* Set sub priority */
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x00;
    /* Enable interrupt */
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    /* Add to NVIC */
    NVIC_Init(&NVIC_InitStruct);
}

/* Set interrupt handlers */
/* Handle PD0 interrupt */
void EXTI9_5_IRQHandler(void) {
    /* Make sure that interrupt flag is set */
    if (EXTI_GetITStatus(EXTI_Line5) != RESET) {
        /* Do your stuff when PD0 is changed */
    	//GPIOA->ODR |= (1 << 5);
    	setMotionRemoteSwitch("D", ON);
    	TIM3->CNT = 0x0;
        /* Clear interrupt flag */
        EXTI_ClearITPendingBit(EXTI_Line5);
    }
}




