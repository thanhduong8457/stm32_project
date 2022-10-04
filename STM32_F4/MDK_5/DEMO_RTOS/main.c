/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/
 
#include "RTE_Components.h"
#include  CMSIS_device_header
#include "cmsis_os2.h"

#include "stm32f4xx.h"
#include "gpio.h"
#include "delay.h"
#include "interrupt.h"
#include "uart.h"

#define step_per_degree 200/360
#define gear_ratio 4

volatile int theta_1;
volatile int theta_2;
volatile int theta_3;

int last_theta_1;
int last_theta_2;
int last_theta_3;

int denta_1;
int denta_2;
int denta_3;
 
/*----------------------------------------------------------------------------
 * Application main thread
 *---------------------------------------------------------------------------*/
 
__NO_RETURN static void control_theta_1 (void *argument) {
  	(void)argument;
  	// ...
  	for (;;) 
	{
		if((theta_1 - last_theta_1)>0)	step(GPIOD, GPIO_Pin_10, GPIO_Pin_11, 1, (theta_1 - last_theta_1)*step_per_degree*gear_ratio);
		else if ((theta_1 - last_theta_1)<0) step(GPIOD, GPIO_Pin_10, GPIO_Pin_11, 0, -(theta_1 - last_theta_1)*step_per_degree*gear_ratio);
		last_theta_1 = theta_1;
	}
}

__NO_RETURN static void control_theta_2 (void *argument) {
  (void)argument;
  // ...
  for (;;) 
	{
		if((theta_2 - last_theta_2)>0)	step(GPIOD, GPIO_Pin_12, GPIO_Pin_13, 1, (theta_2 - last_theta_2)*step_per_degree*gear_ratio);
		else if ((theta_2 - last_theta_2)<0) step(GPIOD, GPIO_Pin_12, GPIO_Pin_13, 0, -(theta_2 - last_theta_2)*step_per_degree*gear_ratio);
		last_theta_2 = theta_2;
	}
}

__NO_RETURN static void control_theta_3 (void *argument) {
  (void)argument;
  // ...
  for (;;) 
	{
		if((theta_3 - last_theta_3))	step(GPIOD, GPIO_Pin_14, GPIO_Pin_15, 1, (theta_3 - last_theta_3)*step_per_degree*gear_ratio);
		else if ((theta_3 - last_theta_3)<0) step(GPIOD, GPIO_Pin_14, GPIO_Pin_15, 0, -(theta_3 - last_theta_3)*step_per_degree*gear_ratio);
		last_theta_3 = theta_3;
	}
}

int main(void){
  	// System Initialization
  	SystemCoreClockUpdate();
 
	init_gpio();
	init_delay();
	init_interrupt();
	init_uart();
	
  	osKernelInitialize();                 // Initialize CMSIS-RTOS
  	osThreadNew(control_theta_1, NULL, NULL);    // Create application main thread
	osThreadNew(control_theta_2, NULL, NULL);    // Create application main thread
	osThreadNew(control_theta_3, NULL, NULL);    // Create application main thread
  	osKernelStart();                      // Start thread execution
	
  	for (;;) 
	{
	}
}
