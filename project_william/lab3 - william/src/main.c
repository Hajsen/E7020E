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
	//RCC->AHB1RSTR |= RCC_AHB1RSTR_GPIOARST; // Reset
	//GPIOA RCC->AHB1RSTR = 0; // Exit reset state
	//RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; // Enable GPIOA clock

	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; // Enable GPIOA clock
	//*(volatile uint32_t*)(0x40020000) |= (1<<10);
	GPIOA->MODER |= GPIO_MODER_MODER5_0; // Enable Output on A5 (LED2 on Nucleo F401RE board)
	GPIOA->OTYPER |= 0;//GPIO_OTYPER_OT_5; //output push pull




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

  /* Infinite loop */
  while (1)
  {
	  GPIOA->ODR ^= (1 << (5)); // toggle LED
	  volatile int x = 0;
	  while (x < 5000000) { x++; }

  }
}
