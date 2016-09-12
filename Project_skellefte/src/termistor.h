/*
 * termistor.h
 *
 *  Created on: Mar 3, 2016
 *      Author: davbom-2
 */

#ifndef TERMISTOR_H_
#define TERMISTOR_H_
#include <stdint.h>

int getTemperature();
int toCelciusValue(int digital_adc_value);
int adc_convert();
uint16_t ADC_GetConversionValueADC1();
int ADC_GetFlagStatusADC1();
void adc_configure();
int getSampledTemperature();

#endif /* TERMISTOR_H_ */
