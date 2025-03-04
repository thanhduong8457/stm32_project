#include "delay.h"

void init_delay(void)
{
  #ifndef REGISTER
	RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM6, ENABLE);
  
  TIM_TimeBaseInitTypeDef  TIMER6;

  TIMER6.TIM_Prescaler = (1-1);
  TIMER6.TIM_CounterMode = TIM_CounterMode_Up;
  TIMER6.TIM_Period = 0xffff;
  TIM_TimeBaseInit(TIM6, &TIMER6);
  TIM_Cmd(TIM6, ENABLE);

  #else
  /*Enable clock timer6*/
  RCC->AHB1ENR = (1<<4);
  /*GPIOD config*/
  TIM6->CR1 |= (1<<1);
  TIM6->PSC = (84-1);
  TIM6->CR1 &= ~(1<<1);
  TIM6->CR1 |= (1<<0);

  #endif   
}

void delay_ms(uint32_t u32DelayInMs)
{
	
	while (u32DelayInMs) {
    #ifndef REGISTER
		TIM_SetCounter(TIM6, 0);
		while (TIM_GetCounter(TIM6) < 2000) {
		}
    #else
    TIM6->CNT = 0;
		while (TIM6->CNT < 2000) {
		}
    #endif
    --u32DelayInMs;
	}
}

void delay_us(uint32_t u32DelayInUs)
{
	
	while (u32DelayInUs) {
    #ifndef REGISTER
		TIM_SetCounter(TIM6, 0);
		while (TIM_GetCounter(TIM6) < 2) {
		}
    #else
    TIM6->CNT = 0;
		while (TIM6->CNT < 2) {
		}
    #endif
    --u32DelayInUs;
	}
}
