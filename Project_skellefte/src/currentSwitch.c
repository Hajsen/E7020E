
#ifndef stm32f4xx_h
#define stm32f4xx_h
#include "stm32f4xx.h"
#endif
#include "currentSwitch.h"


void initTransceiverGPIO()
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	//GPIOA->
	//433T at PA6
	GPIOA->MODER |= GPIO_MODER_MODER6_0;
	GPIOB->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR6_1;

}
void setTransceiverHigh()
{
	GPIOA->ODR |= 1 << 6;
}
void setTransceiverLow()
{
	GPIOA->ODR &= ~(1 << 6);
}
