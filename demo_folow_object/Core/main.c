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
	
	init_delay();
	init_interrupt();
	init_uart();
	init_pwm();

	int temp = 0;
	int revert = 0;

	while(1)
	{
		// printf("the value of distance is: %lf\n", distance);
		// printf("the value of theta is: %lf\n", theta);
		
		// printf("the value of uk_dis is: %lf\n", uk_dis);
		// printf("the value of uk_theta is: %lf\n", uk_theta);	
		//PWM_generate(uk_dis, uk_theta);

		// PWM_generate(uk_dis, uk_theta);

		// printf("hello world %d \n", temp);	

		if(revert == 0)
		{
			temp++;
		}
		else
		{
			temp--;
		}

		if(temp>=99)
		{
			revert = 1;
		}
		else if(temp <= 0 )
		{
			revert = 0;
		}
		
		TIM_SetCompare1(TIM4, 100);
		TIM_SetCompare2(TIM4, 100);
		TIM_SetCompare3(TIM4, 100);
		TIM_SetCompare4(TIM4, 100);
		
		delay_ms(100);
	}

	return 0;
}

