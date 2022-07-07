#include "interrupt.h"

const double Ts = 0.001;

double kp_dis, ki_dis, kd_dis;
static double uk1_dis, ek_dis, ek1_dis, ek2_dis;
const double uk_min_dis = -1;
const double uk_max_dis = 1;

double kp_theta, ki_theta, kd_theta;
static double uk1_theta, ek_theta, ek1_theta, ek2_theta;
const double uk_min_theta = -1;
const double uk_max_theta = 1;

void init_interrupt(void)
{	
	// Enable clock for TIM2
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	// TIM2 initialization for overflow every 500ms
	// Update Event (Hz) = SystemCoreClock / ((TIM_Prescaler + 1) * (TIM_Period + 1)*( RCR+1))
	// Update Event (Hz) = 72MHz / ((3599 + 1) * (9999 + 1)*(0+1)) = 2Hz (0.5s)
	//Thiet lap he so chia cho bo TIMER
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	// Thiet lap cho TIMER dem len
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	//Thiet lap gia tri cua chu ky
	TIM_TimeBaseInitStruct.TIM_Period = 9999;
	//thiet lap gia tri cho bo chia
	TIM_TimeBaseInitStruct.TIM_Prescaler = 3599;
	//Thiet lap gia tri RCR
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
	// Thiet lap che do TimeBase
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);
	
	// Enable TIM2 interrupt
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	// Start TIM2
	TIM_Cmd(TIM2, ENABLE);

	NVIC_InitTypeDef  NVIC_InitStructure;
	
  	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);
}

void TIM2_IRQHandler(void)
{
	// Checks whether the TIM2 interrupt has occurred or not
	if (TIM_GetITStatus(TIM2, TIM_IT_Update))
	{
		pid_control();

		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}

void pid_control(void)
{
	ek2_dis = ek1_dis;
	ek1_dis = ek_dis;
	ek_dis = SET_POINT_DISTANCE - distance;
	uk1_dis = uk_dis;
	uk_dis = uk1_dis + kp_dis*(ek_dis - ek1_dis) + ki_dis*Ts/2*(ek_dis + ek1_dis) + kd_dis/Ts*(ek_dis - 2*ek1_dis + ek2_dis);
	if(uk_dis>uk_max_dis) uk_dis = uk_max_dis;
	if(uk_dis<uk_min_dis) uk_dis = uk_min_dis;

	ek2_theta = ek1_theta;
	ek1_theta = ek_theta;
	ek_theta = SET_POINT_THETA - theta;
	uk1_theta = uk_theta;
	uk_theta = uk1_theta + kp_theta*(ek_theta - ek1_theta) + ki_theta*Ts/2*(ek_theta + ek1_theta) + kd_theta/Ts*(ek_theta - 2*ek1_theta + ek2_theta);
	if(uk_theta>uk_max_theta) uk_theta = uk_max_theta;
	if(uk_theta<uk_min_theta) uk_theta = uk_min_theta;
}
