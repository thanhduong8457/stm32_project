#include "pwm.h"

void init_pwm(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef timerInit;
	TIM_OCInitTypeDef pwmInit;
	
	RCC_APB2PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;
	
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	timerInit.TIM_CounterMode = TIM_CounterMode_Up;
	timerInit.TIM_Prescaler = 168 - 1;
	timerInit.TIM_Period = 100 - 1;

	TIM_TimeBaseInit(TIM4, &timerInit);
	
	TIM_Cmd(TIM4, ENABLE);

	pwmInit.TIM_OCMode = TIM_OCMode_PWM1;
	pwmInit.TIM_OCPolarity = TIM_OCPolarity_High;
	pwmInit.TIM_Pulse = 0;
	pwmInit.TIM_OutputState = TIM_OutputState_Enable;

	TIM_OC1Init(TIM4, &pwmInit);
	TIM_OC2Init(TIM4, &pwmInit);
	TIM_OC3Init(TIM4, &pwmInit);
	TIM_OC4Init(TIM4, &pwmInit);
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
