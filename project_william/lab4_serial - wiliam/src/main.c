/**
*****************************************************************************
**
**  File        : main.c
**
**  Abstract    : main function.
**
**  Functions   : main
**
**  Environment : Atollic TrueSTUDIO(R)
**                STMicroelectronics STM32F4xx Standard Peripherals Library
**
**  Distribution: The file is distributed "as is", without any warranty
**                of any kind.
**
**  (c)Copyright Atollic AB.
**  You may use this file as-is or modify it according to the needs of your
**  project. This file may only be built (assembled or compiled and linked)
**  using the Atollic TrueSTUDIO(R) product. The use of this file together
**  with other tools than Atollic TrueSTUDIO(R) is not permitted.
**
*****************************************************************************
*/

/* Includes */
#include "stm32f4xx.h"
#include "stm32f401_discovery.h"
#include "main.h"
#include <string.h>

int startupLock = 0;
char input[16];
int inputBufferSize = 0;
int inputTooLong = FALSE;

/* Private macro */
/* Private variables */
/* Private function prototypes */
/* Private functions */

/**
**===========================================================================
**
**  Abstract: main program
**
**===========================================================================
*/
void TIM2_IRQHandler(void) {
	GPIOA->ODR ^= (1 << (5)); // toggle LED
	TIM2->SR &= ~1; //reset status on tim2
}

void TIM5_IRQHandler(void){
	startupLock += 1;
	if (startupLock == 2){
		disableLEDTimer();
		disableLED();
		NVIC_DisableIRQ(TIM5_IRQn);
	}
	TIM5->SR &= ~1;
}

void USART2_IRQHandler(void){
	char received = USART2->DR;
	switch(received){
		case 0xD:
			receiveChar(received);
			processInput();
			break;
			// Not
		case 0x8:
			backspace();
			break;
		default:
			receiveChar(received);
			break;
	}
}

int main(void)
{
	/**
	*  IMPORTANT NOTE!
	*  The symbol VECT_TAB_SRAM needs to be defined when building the project
	*  if code has been located to RAM and interrupts are used.
	*  Otherwise the interrupt table located in flash will be used.
	*  See also the <system_*.c> file and how the SystemInit() function updates
	*  SCB->VTOR register.
	*  E.g.  SCB->VTOR = 0x20000000;
	*/
	/* TODO - Add your application code here */
	initializeLED();
	powerEnable();
	ledSettings();
	startup();
	usart2GPIOSettings();
	usart2Settings();



	/* Infinite loop */
	volatile int i;
	while (1){
		i++;
	}
}

void receiveChar(char character){
	if(inputBufferSize<16){
		input[inputBufferSize] = character;
		while((USART2->SR & (USART_FLAG_TXE)) == RESET);
		USART2->DR = character;
		inputBufferSize += 1;
	}
	else{
		inputTooLong = TRUE;
	}
}

void backspace(){
	if(inputBufferSize > 0){
		input[inputBufferSize] = NULL;
		inputBufferSize -= 1;
		inputTooLong = FALSE;
		while((USART2->SR & (USART_FLAG_TXE)) == RESET);
		USART2->DR = 0x8;
	}
}

void processInput(){
	// Command too long
	if(inputTooLong){
		echo("ERROR, String too long");
	}
	// On command
	else if(input[0] == 'o' && input[1] == 'n' && strlen(input) == 3){
		disableLEDTimer();
		enableLED();
		echo("status on");
	}
	// Off command
	else if(input[0] == 'o' && input[1] == 'f' && input[2] == 'f' && strlen(input) == 4){
		disableLEDTimer();
		disableLED();
		echo("status off");
	}
	// Blink Command
	else if(input[0] == 'b' && input[1] == 'l' && input[2] == 'i' && input[3] == 'n' && input[4] == 'k' && input[5] == ' ' && strlen(input) > 7){
		// if conversion failed -1 will be returned
		long period = convertStringToLong(input);
		if(period == -1){
			char string[40];
			strcpy(string, "ERROR, could not parse ");
			strcat(string, input);
			echo(string);
		}
		else{
			enableLEDTimer();
			setLEDPeriod(period);
			char string[40];
			strcpy(string, "Status ");
			strcat(string, input);
			echo(string);
		}
	}
	// No command
	else{
		char string[40];
		strcpy(string, "ERROR, could not parse ");
		strcat(string, input);
		echo(string);
	}

	newLine();

	//clear input buffert
	memset(&input[0], 0, strlen(input));
	inputTooLong = FALSE;
	inputBufferSize = 0;
}

long convertStringToLong(char input[16]){
	volatile int inputInt;
	volatile long q = 0;
	volatile int i = 0;
	for(i = 6;i<strlen(input)-1;i++){
		inputInt = input[i];
		if((inputInt > 57) || (inputInt < 48)){
			return -1;
		}
		q = q * 10 + (input[i] - 48);
	}
	return q;
}

void echo(char string[16]){
	/*//new line
	while((USART2->SR & (USART_FLAG_TXE)) == RESET);
	USART2->DR = 0xA;
	//start of text
	while((USART2->SR & (USART_FLAG_TXE)) == RESET);
	USART2->DR = 0x02;*/
	newLine();

	int i;
	for(i = 0; i < strlen(string); i++){
		while((USART2->SR & (USART_FLAG_TXE)) == RESET);
		USART2->DR = string[i];
	}
}

void newLine(){
	// '\r'
	while((USART2->SR & (USART_FLAG_TXE)) == RESET);
	USART2->DR = 0x0D;
	// '\n'
	while((USART2->SR & (USART_FLAG_TXE)) == RESET);
	USART2->DR = 0x0A;

}

// NON WORKING FUNCTION
/*
int strToInteger(char string[9]){
	int i;
	for(i = 0; i < strlen(string);i++){
		if( 47 > (int)string[i] && (int)string[i] > 58){
			echo(string[i]);
		}
		else{
			echo("BAJS");
			echo(string);
		}
	}
	return 1;
}
*/

void startup(){
	// setup startup timer
	tim5Settings();
	tim2Settings();

	// Enable interrupts
	NVICSetup();
}

void setLEDPeriod(int period){
	TIM2->ARR = 500*period;
	TIM2->CNT = 0;

}

void enableLEDTimer(){
	TIM2->CR1 |= 1;
	TIM2->CNT = 0;
}

void disableLEDTimer(){
	TIM2->CR1 &= ~0x0000000000000001;
	GPIOA->ODR &= ~(1 << (5));
}

void disableLED(){
	GPIOA->ODR &= ~(1 << (5));
}

void enableLED(){
	GPIOA->ODR |= (1 << (5));
}
void initializeLED(){
	/* Initialize LEDs */
	STM_EVAL_LEDInit(LED3);
	STM_EVAL_LEDInit(LED4);
	STM_EVAL_LEDInit(LED5);
	STM_EVAL_LEDInit(LED6);

	/* Turn on LEDs */
	STM_EVAL_LEDOn(LED3);
	STM_EVAL_LEDOn(LED4);
	STM_EVAL_LEDOn(LED5);
	STM_EVAL_LEDOn(LED6);
}

void enableExternalClock(){
	//ENABLES THE Clock
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;

	//SETS MODER 8 TO alternate output mode
	GPIOC->MODER |= GPIO_MODER_MODER9_1;

	//Set port output speed to high speed
	GPIOC->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR9;
}

void NVICSetup(){
	//interrupt for led timer
	NVIC_EnableIRQ(TIM2_IRQn);
	NVIC_SetPriority(TIM2_IRQn, 2);
	//interrupt for startup timer
	NVIC_EnableIRQ(TIM5_IRQn);
	NVIC_SetPriority(TIM5_IRQn, 1);
	//interrupt for receive (USART2)
	NVIC_EnableIRQ(USART2_IRQn);
	NVIC_SetPriority(USART2_IRQn, 0);
}

void tim2Settings(){
	// ----------  TIM2 SETTINGS  ----------
	/* enable timer 2 */
	TIM2->CR1 |= 1;
	/* prescalar of clock clock_freq / TIM_PSC */
	TIM2->PSC = 84;
	// Set blinking period
	setLEDPeriod(500);
	/* DMA interrupt enabled for tim2 */
	TIM2->DIER |= 1;
	/* set counter */
	TIM2->CNT = 0;
	//reset status on tim5
	//TIM2->SR &= ~1;
}

void tim5Settings(){
	// ----------  TIM5 SETTINGS  ----------
	/* enable timer 5 */
	TIM5->CR1 |= 1;
	/* prescalar of clock clock_freq / TIM_PSC */
	TIM5->PSC = 84;
	//interrupt in 1.5 seconds
	TIM5->ARR = 1000 * 1400;
	/* DMA interrupt enabled for tim5 */
	TIM5->DIER |= 1;
	/* set counter */
	TIM5->CNT = 0;
	//reset status on tim5
	//TIM5->SR &= ~1;
}


void powerEnable(){
	//usart2 power on
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
	//gpioa enable
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	/* enable power tim2 */
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	/* enable power tim5 */
	RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;
	//Set MCO2 pin to output System clock
	RCC->CFGR &= ~RCC_CFGR_MCO2;
}

void usart2GPIOSettings(){
	// ----------  USART GPIO SETTINGS  ----------
	//alternate function on gpio on pa2 (for output)
	GPIOA->AFR[0] |= (GPIO_AF_USART2<<8);
	//alternate function on gpio on pa3 (for input)
	GPIOA->AFR[0] |= (GPIO_AF_USART2<<12);
	// Alternate Function for PA2 (Output)
	GPIOA->MODER |= GPIO_MODER_MODER2_1;
	// Alternate Function for PA3 (Input)
	GPIOA->MODER |= GPIO_MODER_MODER3_1;
}

void usart2Settings(){
	// ----------  USART SETTINGS  ----------
	// USART M bit sets 8 bits data or 9 bits data, 0 => 8 bits of data
	USART2->CR1 &= ~USART_CR1_M;
	// USART stop bits, 00 => 1 stop bit
	USART2->CR2 &= ~USART_CR2_STOP;
	//Baud rate of 19200
	USART2->BRR |= 0x883;
	// Transmit Enable
	USART2->CR1 |= USART_CR1_TE;
	//receive enable
	USART2->CR1 |= USART_CR1_RE;
	//enable interrupt receive
	USART2->CR1 |= USART_CR1_RXNEIE;
	// USART Enable
	USART2->CR1 |= USART_CR1_UE;
}

void ledSettings(){
	// ----------  LED SETTINGS  ----------
	GPIOA->MODER |= GPIO_MODER_MODER5_0; // Enable Output on A5 (LED2 on Nucleo F401RE board)
	GPIOA->OTYPER |= 0;//GPIO_OTYPER_OT_5; //output push pull
}


/*
 * Callback used by stm32f401_discovery_audio_codec.c.
 * Refer to stm32f401_discovery_audio_codec.h for more info.
 */

void EVAL_AUDIO_TransferComplete_CallBack(uint32_t pBuffer, uint32_t Size){
	/* TODO, implement your code here */
	return;
}

/*
 * Callback used by stm32f401_discovery_audio_codec.c.
 * Refer to stm32f401_discovery_audio_codec.h for more info.
 */
uint16_t EVAL_AUDIO_GetSampleCallBack(void){
	/* TODO, implement your code here */
	return -1;
}


/* OLD
 *
 * 	volatile char received;
	for(;;) {
		// waits for input
		//while((USART2->SR & USART_FLAG_RXNE) == RESET);
		//received = USART2->DR;
		// waits for output
		//while((USART2->SR & (USART_FLAG_TXE)) == RESET);
		//USART2->DR = received;
	}
 */
