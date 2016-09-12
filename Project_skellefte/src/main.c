//#include "stm32f4xx_adc.h"
//#include "stm32f4xx_gpio.h"
//#include "stm32f4xx_rcc.h"

#ifndef st32f4xx_h
#define stm32f4xx_h
#include "termistor.h"
#include "pwm_control.h"
#include "stm32f4xx.h"
#include "usart.h"
#include "motiondetector.h"
#include "transceiver.h"
#include "remoteSwitchController.h"
#endif


int ConvertedValue = 0; //Converted value readed from ADC

void Delay(__IO uint32_t nCount);

int main(void)
{
	strcpy(input,"");
	Configure_PA5();
	initTimerMotionDetector();

	runUSARTStartUp();
	initPWMGPIO();
	init_RGBColours();
	initPWMTimer();
	adc_configure();//Start configuration
	TM_Delay_Init();
	initTransceiverGPIO();

	//setLEDOffState();
	while(1)  //Loop forever
	{
		setRGBLEDColour(getSampledTemperature());

	}

}

void TIM2_IRQHandler(void)
{
	if(TIM2->SR & TIM_SR_UIF) // if UIF flag is set
	{
		TIM2->SR &= ~TIM_SR_UIF; // clear UIF flag
		//GPIOA->ODR ^= 1 << 5; // toggle LED state
	}
	if(startupCount<6)
	{
		startupCount++;
	}
//	__disable_irq();

//	__enable_irq();
}

void USART6_IRQHandler(void)
{

	if(USART6->SR & USART_SR_RXNE)
	{
		USART6->SR &= ~USART_SR_RXNE;

		int param;
		int status;
		char switch_id[1];
		char symbol[1];
		int data = USART6->DR;
		if (data!=13)
		{
			if(!(strlen(input)==0 && data ==0x20))
			{
				sprintf(symbol, "%c", data);
				strcat(input, symbol);
				USART6->DR = data;
				//returnsymbol = 13
			}

		}
		else
		{
			if(strlen(input) > 16)
			{
				send_USART_String("\r\n ERROR, string to long. \r\n");
			}
			else if (!strncmp(input, "on",2))
			{
				setLEDOnState();
			}
			else if(!strncmp(input,"off", 3))
			{
				//disable blink.
				setLEDOffState();
			}
			else if(!strncmp(input,"blink",5) && sscanf(&input[6], "%d", &param))
			{
				setLEDBlinkState(param);
			}
			else if(!strncmp(input,"temp", 4))
			{
				//Get temp
				getUSARTTemperature();
			}
			else if(!strncmp(input,"On", 2) && sscanf(&input[3], "%s", &switch_id))
			{

				OrderTransceiver(switch_id, 1);
				char output1[50];
				strcpy (output1, "Remote switch ");
				strcat (output1, switch_id);
				strcat (output1, " turned on. \r\n");
				send_USART_String(output1);
			}
			else if(!strncmp(input,"Off", 3) && sscanf(&input[4], "%s", &switch_id))
			{
				OrderTransceiver(switch_id, 0);
				char output1[50];
				strcpy (output1, "Remote switch ");
				strcat (output1, switch_id);
				strcat (output1, " turned off. \r\n");
				send_USART_String(output1);
			}
			else if(!strncmp(input,"setMotionLog", 12) && sscanf(&input[13], "%d", &status))
			{
				char output1[50];
				//strcpy(output1, "Motion detector logging status");
				sprintf(output1, "Motion detector logging status %d \r\n", status);
				setMotionLogStatus(status);
				send_USART_String(output1);
			}
			else
			{
				char output1[50];
				strcpy(output1, "\r\n ERROR, could not parse ");
				strcat(output1, input);
				strcat(output1, " \r\n");
				send_USART_String(output1);
			}

			memset(input,0,strlen(input));

		}
	}

}

// timeout timer for motion.
void TIM3_IRQHandler(void)
{
	//toggle led for testing purpose

	if(TIM3->SR & TIM_SR_UIF) // if UIF flag is set
		{
			TIM3->SR &= ~TIM_SR_UIF; // clear UIF flag
			setMotionRemoteSwitch("D", OFF);
			//GPIOA->ODR &= ~(1 << 5); // toggle LED state
		}
}



void Delay(__IO uint32_t nCount)
{
	  while(nCount--)
	  {


	  }
}


