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
int main(void)
{
  int i = 0;

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

  /* ENABLES THE Clock */
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;

  /* SETS MODER 8 TO alternate output mode */
  GPIOC->MODER |= GPIO_MODER_MODER9_1;

  /* Set port output speed to high speed */
  GPIOC->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR9;

  /* Set MCO2 pin to output System clock */
  RCC->CFGR &= ~RCC_CFGR_MCO2;

	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; // Enable GPIOA clock
	//*(volatile uint32_t*)(0x40020000) |= (1<<10);
	GPIOA->MODER |= GPIO_MODER_MODER5_0; // Enable Output on A5 (LED2 on Nucleo F401RE board)
	GPIOA->OTYPER |= 0;//GPIO_OTYPER_OT_5; //output push pull
	GPIOA->ODR ^= (1 << (5)); // toggle LED
  /* Infinite loop */
  while (1)
  {
	i++;
  }
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
