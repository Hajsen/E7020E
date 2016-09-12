/*
 * termistor.c
 *
 *  Created on: Mar 3, 2016
 *      Author: davbom-2
 */
#ifndef stm32f4xx_h
#define stm32f4xx_h
#include "stm32f4xx.h"
#endif
#include "termistor.h"

#define NR_OF_TEMP_SAMPLES 25

//returns the temperature from the thermistor in Celcius [C]
int getTemperature()
{
	int temp_D = adc_convert();
	int temp_C = toCelciusValue(temp_D);
	return temp_C;
}


int toCelciusValue(int digital_adc_value)
{

	float T_c = 0.01; 	// 10 mV/C
	float V_0c = 0.5;	// 500 mV
	float temp_C = ((3.3/4096)*(float)digital_adc_value - V_0c)/(T_c);
	return (int)temp_C;

}

// Outputs average temperature from ten temperature values.
int getSampledTemperature()
{
	int sum=0;
	int i;
	for(i=0; i< NR_OF_TEMP_SAMPLES; i++)
	{
		sum = sum + getTemperature();
	}
	return sum/NR_OF_TEMP_SAMPLES;
}


//Returns the digital value of the ADC
int adc_convert()
{
 //ADC_SoftwareStartConv(ADC1);//Start the conversion
 // Enable the selected ADC conversion for regular group
 ADC1->CR2 |= 0x40000000;	//  ADC_CR2_EXTEN_0;

 while(!ADC_GetFlagStatusADC1());//Processing the conversion
 return ADC_GetConversionValueADC1(); //Return the converted data
}

uint16_t ADC_GetConversionValueADC1()
{
  /* Return the selected ADC conversion value */
  return (uint16_t) ADC1->DR;
}

int ADC_GetFlagStatusADC1()
{
  int bitstatus = 0;

  /* Check the status of the specified ADC flag */
  if ((ADC1->SR & ADC_SR_EOC) != 0)
  {
    /* ADC_FLAG is set */
    bitstatus = 1;
  }
  else
  {
    /* ADC_FLAG is reset */
    bitstatus = 0;
  }
  /* Return the ADC_FLAG status */
  return  bitstatus;
}

//enables ADC1
void adc_configure()
{

 //Clock configuration
 //Enable clock for ADC1
 RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
 //Enable clock for GPIO C
 RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;

 //set PC4 to analog function.
 GPIOC->MODER |= GPIO_MODER_MODER4;

 // The conversion is continuous, the input data is converted more than once
 ADC1->CR2 |= ADC_CR2_CONT;

 //Enable ADC conversion
 ADC1->CR2 |= ADC_CR2_ADON;

 //set adc sample time to 144cycles at channel 14
 ADC1->SMPR1 |= 0x6 << 12; //ADC_SMPR1_SMP10


 //1st conversion in regular sequence
 ADC1->SQR3 |= 0xE;		//ADC_SQR3_SQ1_0;

}


