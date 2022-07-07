#include "stm32f4xx.h"
#include "stdio.h"
#include "pwm.h"
#include "delay.h"
#include "interrupt.h"
#include "uart.h"

double distance;
double theta;
double uk_dis;
double uk_theta;

int main(void)
{
	SystemInit();
	init_delay();
	init_interrupt();
	init_uart();
	init_pwm();

	while(1)
	{
		printf("the value of distance is: %lf\n", distance);
		printf("the value of theta is: %lf\n", theta);
		
		printf("the value of uk_dis is: %lf\n", uk_dis);
		printf("the value of uk_theta is: %lf\n", uk_theta);

		PWM_generate(uk_dis, uk_theta);
		delay_us(100);
	}

	return 0;
}

