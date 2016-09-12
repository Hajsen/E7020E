
#ifndef stm32f4xx_h
#define stm32f4xx_h
#include "stm32f4xx.h"
#include "pwm_control.h"
#endif
#include <math.h>



int colour_combinations[10][3] = {
{255,0,127},// PURPLE	-20 < T <= -16
{0,0,255},	//	BLUE	-16 < T <= -12
{0,127,255},//	LIGHT BLUE		-12 < T <= -8
{0,255,255},//	TEAL	-8 < T <= -4
{0,255,25},//	LIME GREEN 		-4 < T <= 0
{0,255,0},	//	GREEN	-0 < T <= 4
{127,127,0},//	LIGHT GREEN	4 < T <= 8
{255,100,0},//	YELLOW		8 < T <= 12
{255,10,0},//	ORANGE		12 < T <= 16
{255,0,0}//RED		16 < T <= 20
	//
};

void init_RGBColours()
{
	int lowerBoundTemp = 10;
	int upperBoundTemp = 12;
	int i;
	for (i = 0; i < 10; i++) {
		temperatureInterval[i].temp_interval[0] = lowerBoundTemp;
		temperatureInterval[i].temp_interval[1] = upperBoundTemp;
		int j;
		for(j = 0; j < 3; j++)
		{
			temperatureInterval[i].colour_combination[j] = colour_combinations[i][j];
		}
		lowerBoundTemp+=2;
		upperBoundTemp+=2;

	}
	//temperatureInterval.temp_interval[1] = 4;
}
//enable GPIOB for enable PWM-output PB13(RED), PB14(BLUE) ,PB15(GREEN)
void initPWMGPIO()
{


	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;

	//initalize pin 13 B - relating to timer 1 channel 3 (PB13 <-> TIM1_CH1)
	GPIOB->MODER |= GPIO_MODER_MODER13_1;
	GPIOB->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR13_1;	//50 MHZ GPIOspeed (fast speed)
	GPIOB->PUPDR |= GPIO_PUPDR_PUPDR13_0;
	GPIOB->AFR[1] |= GPIO_AF_TIM1 << 20;

	//initalize pin 14 B - relating to timer 1 channel 3 (PB14 <-> TI1_CH2)
	GPIOB->MODER |= GPIO_MODER_MODER14_1;
	GPIOB->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR14_1;	//50 MHZ GPIOspeed (fast speed)
	GPIOB->PUPDR |= GPIO_PUPDR_PUPDR14_0;
	GPIOB->AFR[1] |= GPIO_AF_TIM1 << 24;

	//initalize pin 15 B - relating to timer 1 channel 3 (PB15 <-> TIM1_CH3)
	GPIOB->MODER |= GPIO_MODER_MODER15_1;
	GPIOB->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR15_1;	//50 MHZ GPIOspeed (fast speed)
	GPIOB->PUPDR |= GPIO_PUPDR_PUPDR15_0;
	GPIOB->AFR[1] |= GPIO_AF_TIM1 << 28;

}

void initPWMTimer()
{
	/* Enable TIM4 Clock */
		//RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
		RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
		/* Setup PWM */
		uint16_t PrescalerValue = 0;
		PrescalerValue = (uint16_t) ((SystemCoreClock /2) / 42000000) - 1;
		//84*10^6/ 21*10^6 = 4
																						//4-1 = 3
		/* Setup timer defaults */
	//	TIM4->ARR = 665;
	//	TIM4->PSC = PrescalerValue;

		/* Configure timer for PWM - channel 1*/
	//	TIM4->CCMR1 |= TIM_OCMode_PWM1;
	//	TIM4->CCER |= TIM_OutputState_Enable;
	//	TIM4->CCR1 = 665;


		/* Setup timer defaults */
		TIM1->ARR = 665;
		TIM1->PSC = PrescalerValue;

		/* Configure timer for PWM - channel 1*/
		TIM1->CCMR1 |= TIM_OCMode_PWM1;
		TIM1->CCER |= TIM_OutputNState_Enable;
		TIM1->CCR1 = 0;


		/* Start timer*/



		/* Configure timer for PWM - channel 2*/
		TIM1->CCMR1 |= TIM_OCMode_PWM1 << 8;
		TIM1->CCER |= TIM_OutputNState_Enable << 4;
		TIM1->CCR2 = 0;

		/* Configure timer for PWM - channel 3*/

		TIM1->CCMR2 |= TIM_OCMode_PWM1;
		TIM1->CCER |= TIM_OutputNState_Enable << 8;
		TIM1->CCR3 = 0;


		TIM1->BDTR |= TIM_BDTR_MOE;
		TIM1->CR1 |= TIM_CR1_ARPE;
		/* Start timer*/
		TIM1->CR1 |= TIM_CR1_CEN;
}
// Returns the index of the array that holds the temperature interval.
int findTemperatureInterval(int temperature)
{
	int arraylength = sizeof(temperatureInterval)/sizeof(temperatureInterval[0]);
	int i;
	for (i=0; i< arraylength ; i++)
	{
		if(temperature > temperatureInterval[i].temp_interval[0] && temperature <= temperatureInterval[i].temp_interval[1])
		{
			return i;
		}
	}

	return 9;
}


void setSingleLEDColour(colour LED, int colour_strength)
{
	//255 colour strength equals 700 power units
	//3V equals 700 power units
	float power = ceil(700/255 * colour_strength);
	int power_int = (int) power;
	switch (LED) {
		case RED:
			TIM1->CCR3 = power_int;
			break;
		case GREEN:
			TIM1->CCR1 = power_int;
			break;
		case BLUE:
			TIM1->CCR2 = power_int;
			break;
		default:
			break;
	}
	return;
}


void setRGBLEDColour(int temperature)
{
	int interval_index = findTemperatureInterval(temperature);
	if(interval_index !=-1)
	{
		setSingleLEDColour(RED, temperatureInterval[interval_index].colour_combination[0]);
		setSingleLEDColour(GREEN, temperatureInterval[interval_index].colour_combination[1]);
		setSingleLEDColour(BLUE, temperatureInterval[interval_index].colour_combination[2]);
	}

}






