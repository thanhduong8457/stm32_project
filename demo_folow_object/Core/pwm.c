#include "pwm.h"

void init_pwm(void)
{
	GPIO_InitTypeDef gpioInit;
	TIM_TimeBaseInitTypeDef timInit;
	TIM_OCInitTypeDef pwmInit;
	
	/* timer 4 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	timInit.TIM_Prescaler = 42 - 1;
	timInit.TIM_Period = 100 - 1;
	TIM_TimeBaseInit(TIM4, &timInit);
	
	pwmInit.TIM_OCMode = TIM_OCMode_PWM1;
	pwmInit.TIM_OCPolarity = TIM_OCPolarity_High;
	pwmInit.TIM_Pulse = 0;
	pwmInit.TIM_OutputState = TIM_OutputState_Enable;
	
	TIM_OC1Init(TIM4, &pwmInit);
	
	pwmInit.TIM_OCMode = TIM_OCMode_PWM1;
	pwmInit.TIM_OCPolarity = TIM_OCPolarity_High;
	pwmInit.TIM_Pulse = 0;
	pwmInit.TIM_OutputState = TIM_OutputState_Enable;
	
	TIM_OC2Init(TIM4, &pwmInit);
	
	pwmInit.TIM_OCMode = TIM_OCMode_PWM1;
	pwmInit.TIM_OCPolarity = TIM_OCPolarity_High;
	pwmInit.TIM_Pulse = 0;
	pwmInit.TIM_OutputState = TIM_OutputState_Enable;
	
	TIM_OC3Init(TIM4, &pwmInit);

	pwmInit.TIM_OCMode = TIM_OCMode_PWM1;
	pwmInit.TIM_OCPolarity = TIM_OCPolarity_High;
	pwmInit.TIM_Pulse = 0;
	pwmInit.TIM_OutputState = TIM_OutputState_Enable;
	
	TIM_OC4Init(TIM4, &pwmInit);
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	
	gpioInit.GPIO_Mode = GPIO_Mode_AF;
	gpioInit.GPIO_OType = GPIO_OType_PP;
	gpioInit.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	gpioInit.GPIO_PuPd = GPIO_PuPd_NOPULL;
	gpioInit.GPIO_Speed = GPIO_High_Speed;
	
	GPIO_Init(GPIOD, &gpioInit);
	
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource13, GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_TIM4);
	
	TIM_Cmd(TIM4, ENABLE);
}

void PWM_generate(double uk_dis,double uk_theta)
{
	pwm_left(uk_dis + uk_theta);
	pwm_right(uk_dis - uk_theta);
}

void pwm_left(double value)
{
	if(value<0)
	{
		TIM4->CCR1 = (int)(value*100);
		TIM4->CCR2 = 0;
	}
	else if(value>0)
	{
		TIM4->CCR1 = 0;
		TIM4->CCR2 = (int)(value*100);
	}
}

void pwm_right(double value)
{
	if(value<0)
	{
		TIM4->CCR3 = (int)(value*100);
		TIM4->CCR4 = 0;
	}
	else if(value>0)
	{
		TIM4->CCR3 = 0;
		TIM4->CCR4 = (int)(value*100);
	}
}
