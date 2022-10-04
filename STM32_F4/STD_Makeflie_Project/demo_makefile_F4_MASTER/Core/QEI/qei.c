/*
 * QEI.c
 *
 *  Created on: May 3, 2016
 *      Author: AnhTuan
 */
#include "include.h"

//* Private variables ------------------------------------------------------*/
static volatile int32_t Encoder_Pulse_2=0;
static volatile int32_t Encoder_Pulse_3=0;
static volatile int32_t Encoder_Pulse_4=0;
static volatile int32_t Encoder_Pulse_6=0;
	
void TIM1_UP_TIM10_IRQHandler(void)
{
 if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)
   {

		if((TIM1->CR1)&0x0010) //CR1 REGISTER 16 bit
			Encoder_Pulse_3-= 65536;
		else
			Encoder_Pulse_3+= 65536;	
				
    TIM_ClearITPendingBit(TIM1, TIM_IT_Update);	

  }

}
void TIM8_UP_TIM13_IRQHandler(void)
{	
 if (TIM_GetITStatus(TIM8, TIM_IT_Update) != RESET)
   {
  	if((TIM8->CR1)&0x0010) 
			Encoder_Pulse_6-= 65536;
		else
			Encoder_Pulse_6+= 65536;	
				
    TIM_ClearITPendingBit(TIM8, TIM_IT_Update);	

  }

}
 void TIM3_IRQHandler(void)
{
  if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
  {
		if((TIM3->CR1)&0x0010)
			Encoder_Pulse_2-= 65536;
		else
			Encoder_Pulse_2+= 65536;	
//		UARTPutnlong(USARTx,Get_QEI(2));
//		USART_SendData(USARTx,(uint16_t)10); 
    TIM_ClearITPendingBit(TIM3, TIM_IT_Update);	
  }
}
 void TIM4_IRQHandler(void)
{
  if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
  {
		if((TIM4->CR1)&0x0010) 
			Encoder_Pulse_4-= 65536;
		else
			Encoder_Pulse_4+= 65536;				
    TIM_ClearITPendingBit(TIM4, TIM_IT_Update);	
  }
}

int32_t Get_QEI(int joint )
{
	switch( joint)
	{
		case 1:
			return (TIM_GetCounter (TIM5));
	//		break;
		case 2:
			return (Encoder_Pulse_2+ TIM_GetCounter (TIM3));
			//	break;
		case 3:
			return (Encoder_Pulse_3 + TIM_GetCounter (TIM1));
			//	break;
		case 4:
			return (Encoder_Pulse_4 + TIM_GetCounter (TIM4));
			//	break;
		case 5:
			return ( TIM_GetCounter (TIM2));
			//	break;
		case 6:
		 return (Encoder_Pulse_6 + TIM_GetCounter (TIM8));
			//	break;				
}
}



void Set_QEI( int32_t joint,int32_t value )
{
	switch( joint)
	{
		case 1://32 bit
			TIM_SetCounter (TIM5, value);//32 bit no problem 
				break;
		case 2:
			 Encoder_Pulse_2=value-23456;
			 TIM_Cmd(TIM3, DISABLE);
	     TIM_ITConfig(TIM3, TIM_IT_Update, DISABLE);
			 TIM_SetCounter (TIM3, 23456);
			 TIM_ClearFlag(TIM3,TIM_FLAG_Update); 
			 TIM_Cmd(TIM3, ENABLE);
		   TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
				break;
		case 3:
			Encoder_Pulse_3=value-12345;
			 TIM_Cmd(TIM1, DISABLE);
			 TIM_ITConfig(TIM1, TIM_IT_Update, DISABLE);
			 TIM_SetCounter (TIM1, 12345);
			 TIM_ClearFlag(TIM1,TIM_FLAG_Update); 
			 TIM_Cmd(TIM1, ENABLE);
				TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);
				break;
		case 4:
			 Encoder_Pulse_4=value-34567;
			 TIM_Cmd(TIM4, DISABLE);
		  	TIM_ITConfig(TIM4, TIM_IT_Update, DISABLE);
			 TIM_SetCounter (TIM4, 34567);
			 TIM_ClearFlag(TIM4,TIM_FLAG_Update); 
			 TIM_Cmd(TIM4, ENABLE);
		   TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
				break;
		case 5://32 bit
			 TIM_SetCounter (TIM2, value);
				break;
		case 6:
			 Encoder_Pulse_6=value-45678;
			 TIM_Cmd(TIM8, DISABLE);
			 TIM_ITConfig(TIM8, TIM_IT_Update, DISABLE);	
			 TIM_SetCounter (TIM8, 45678);
			 TIM_ClearFlag(TIM8,TIM_FLAG_Update); 
			 TIM_Cmd(TIM8, ENABLE);
		   TIM_ITConfig(TIM8, TIM_IT_Update, ENABLE);	
				break;				
}
}
void Reset_QEI(int joint )
{
	switch( joint)
	{
		case 0:
			TIM_Cmd(TIM1, DISABLE);
			TIM_Cmd(TIM8, DISABLE);
			TIM_Cmd(TIM3, DISABLE);
			TIM_Cmd(TIM4, DISABLE);
			TIM_Cmd(TIM2, DISABLE);
			TIM_Cmd(TIM5, DISABLE);
			TIM_ClearFlag(TIM1,TIM_FLAG_Update); 
			TIM_ClearFlag(TIM8,TIM_FLAG_Update); 
			TIM_ClearFlag(TIM3,TIM_FLAG_Update); 
			TIM_ClearFlag(TIM4,TIM_FLAG_Update);
			TIM_SetCounter (TIM1, 12345);// In the case of Error  overload 0>>65536
			TIM_SetCounter (TIM2, 0);
			TIM_SetCounter (TIM3, 23456);
			TIM_SetCounter (TIM4, 34567);
			TIM_SetCounter (TIM5, 0);
			TIM_SetCounter (TIM8, 45678);
			Encoder_Pulse_2=-23456;
			Encoder_Pulse_3=-12345;
			Encoder_Pulse_4=-34567;
			Encoder_Pulse_6=-45678;
			TIM_Cmd(TIM1, ENABLE);
			TIM_Cmd(TIM8, ENABLE);
			TIM_Cmd(TIM3, ENABLE);
			TIM_Cmd(TIM4, ENABLE);
			TIM_Cmd(TIM2, ENABLE);
			TIM_Cmd(TIM5, ENABLE);
			TIM_ClearFlag(TIM1,TIM_FLAG_Update); 
			TIM_ClearFlag(TIM8,TIM_FLAG_Update); 
			TIM_ClearFlag(TIM3,TIM_FLAG_Update); 
			TIM_ClearFlag(TIM4,TIM_FLAG_Update);
			TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);
			TIM_ITConfig(TIM8, TIM_IT_Update, ENABLE);	
			TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
			TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
			
		case 1:
			 TIM_SetCounter (TIM5, 0);//32bit
				break;
		case 2:
			 TIM_Cmd(TIM3, DISABLE);
			 TIM_SetCounter (TIM3, 0);
			 TIM_ClearFlag(TIM3,TIM_FLAG_Update); 
			 TIM_Cmd(TIM3, ENABLE);
			 Encoder_Pulse_2=0;
				break;
		case 3:
			 TIM_Cmd(TIM1, DISABLE);
			 TIM_SetCounter (TIM1, 0);
			 TIM_ClearFlag(TIM1,TIM_FLAG_Update); 
			 TIM_Cmd(TIM1, ENABLE);
			 Encoder_Pulse_3=0;
				break;
		case 4:
			 TIM_Cmd(TIM4, DISABLE);
			 TIM_SetCounter (TIM4, 0);
			 TIM_ClearFlag(TIM4,TIM_FLAG_Update); 
			 TIM_Cmd(TIM4, ENABLE);
			 Encoder_Pulse_4=0;
				break;
		case 5:
			 TIM_SetCounter (TIM2, 0);//32bit
				break;
		case 6:
			 TIM_Cmd(TIM8, DISABLE);
			 TIM_SetCounter (TIM8, 0);
			 TIM_ClearFlag(TIM8,TIM_FLAG_Update); 
			 TIM_Cmd(TIM8, ENABLE);
		   Encoder_Pulse_6=0;
				break;				
}}
 void ConfigQEI(void)
 {
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  // turn on the clocks for each of the ports needed
  RCC_AHB1PeriphClockCmd (ENC1A_GPIO_CLK, ENABLE);
  RCC_AHB1PeriphClockCmd (ENC1B_GPIO_CLK, ENABLE);
  RCC_AHB1PeriphClockCmd (ENC2A_GPIO_CLK, ENABLE);
  RCC_AHB1PeriphClockCmd (ENC2B_GPIO_CLK, ENABLE);
	RCC_AHB1PeriphClockCmd (ENC3A_GPIO_CLK, ENABLE);
  RCC_AHB1PeriphClockCmd (ENC3B_GPIO_CLK, ENABLE);
	RCC_AHB1PeriphClockCmd (ENC4A_GPIO_CLK, ENABLE);
  RCC_AHB1PeriphClockCmd (ENC4B_GPIO_CLK, ENABLE);
	RCC_AHB1PeriphClockCmd (ENC5A_GPIO_CLK, ENABLE);
  RCC_AHB1PeriphClockCmd (ENC5B_GPIO_CLK, ENABLE);
	RCC_AHB1PeriphClockCmd (ENC6A_GPIO_CLK, ENABLE);
  RCC_AHB1PeriphClockCmd (ENC6B_GPIO_CLK, ENABLE);
  // now configure the pins themselves
	// init for all pin 
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
   // motor1 
    GPIO_InitStructure.GPIO_Pin = ENC1A_PIN;
    GPIO_Init (ENC1A_GPIO_PORT, &GPIO_InitStructure);
		GPIO_InitStructure.GPIO_Pin = ENC1B_PIN;
    GPIO_Init (ENC1B_GPIO_PORT, &GPIO_InitStructure);
	 // motor2 
    GPIO_InitStructure.GPIO_Pin = ENC2A_PIN;
    GPIO_Init (ENC2A_GPIO_PORT, &GPIO_InitStructure);
		GPIO_InitStructure.GPIO_Pin = ENC2B_PIN;
    GPIO_Init (ENC2B_GPIO_PORT, &GPIO_InitStructure);
	 // motor3 
    GPIO_InitStructure.GPIO_Pin = ENC3A_PIN;
    GPIO_Init (ENC3A_GPIO_PORT, &GPIO_InitStructure);
		GPIO_InitStructure.GPIO_Pin = ENC3B_PIN;
    GPIO_Init (ENC3B_GPIO_PORT, &GPIO_InitStructure);
	 // motor4 
    GPIO_InitStructure.GPIO_Pin = ENC4A_PIN;
    GPIO_Init (ENC4A_GPIO_PORT, &GPIO_InitStructure);
		GPIO_InitStructure.GPIO_Pin = ENC4B_PIN;
    GPIO_Init (ENC4B_GPIO_PORT, &GPIO_InitStructure);
		// motor5 
    GPIO_InitStructure.GPIO_Pin = ENC5A_PIN;
    GPIO_Init (ENC5A_GPIO_PORT, &GPIO_InitStructure);
		GPIO_InitStructure.GPIO_Pin = ENC5B_PIN;
    GPIO_Init (ENC5B_GPIO_PORT, &GPIO_InitStructure);
		// motor6 
    GPIO_InitStructure.GPIO_Pin = ENC6A_PIN;
    GPIO_Init (ENC6A_GPIO_PORT, &GPIO_InitStructure);
		GPIO_InitStructure.GPIO_Pin = ENC6B_PIN;
    GPIO_Init (ENC6B_GPIO_PORT, &GPIO_InitStructure);
	
	


  // Connect the pins to their Alternate Functions
  GPIO_PinAFConfig (ENC1A_GPIO_PORT, ENC1A_SOURCE, ENC1A_AF);
  GPIO_PinAFConfig (ENC1B_GPIO_PORT, ENC1B_SOURCE, ENC1B_AF);
	
  GPIO_PinAFConfig (ENC2A_GPIO_PORT, ENC2A_SOURCE, ENC2A_AF);
  GPIO_PinAFConfig (ENC2B_GPIO_PORT, ENC2B_SOURCE, ENC2B_AF);

  GPIO_PinAFConfig (ENC3A_GPIO_PORT, ENC3A_SOURCE, ENC3A_AF);
  GPIO_PinAFConfig (ENC3B_GPIO_PORT, ENC3B_SOURCE, ENC3B_AF);
	
	GPIO_PinAFConfig (ENC4A_GPIO_PORT, ENC4A_SOURCE, ENC4A_AF);
  GPIO_PinAFConfig (ENC4B_GPIO_PORT, ENC4B_SOURCE, ENC4B_AF);
	
	GPIO_PinAFConfig (ENC5A_GPIO_PORT, ENC5A_SOURCE, ENC5A_AF);
  GPIO_PinAFConfig (ENC5B_GPIO_PORT, ENC5B_SOURCE, ENC5B_AF);
	
	GPIO_PinAFConfig (ENC6A_GPIO_PORT, ENC6A_SOURCE, ENC6A_AF);
  GPIO_PinAFConfig (ENC6B_GPIO_PORT, ENC6B_SOURCE, ENC6B_AF);

	
	
	
  // Timer peripheral clock enable
  RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM2, ENABLE);
  RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM3, ENABLE);
	RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM4, ENABLE);
  RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM5, ENABLE);
	
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_TIM1, ENABLE);
  RCC_APB2PeriphClockCmd (RCC_APB2Periph_TIM8, ENABLE);
	

  // set them up as encoder inputs
  // set both inputs to rising polarity to let it use both edges
  TIM_EncoderInterfaceConfig (TIM1, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
  TIM_SetAutoreload (TIM1, 0xffff);
	
	TIM_EncoderInterfaceConfig (TIM2, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
  TIM_SetAutoreload (TIM2, 0xffffffff);
	
	TIM_EncoderInterfaceConfig (TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
	TIM_SetAutoreload (TIM3, 0xffff);

	TIM_EncoderInterfaceConfig (TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
	TIM_SetAutoreload (TIM4, 0xffff);
	
	TIM_EncoderInterfaceConfig (TIM5, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
  TIM_SetAutoreload (TIM5, 0xffffffff);
	
	TIM_EncoderInterfaceConfig (TIM8, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
  TIM_SetAutoreload (TIM8, 0xffff);


  /* Time base configuration */
  TIM_TimeBaseStructure.TIM_Period = 65536-1;  // (2^16-1)
  TIM_TimeBaseStructure.TIM_Prescaler = 1-1; // luu y mode QEI  clock cua chan encoder dua ve khong phai clock he thong
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	
  TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure);
	
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	//Timer 2 5 32bit not use interrupt for my application  
	//Config Timer 3 4 1 8
	/* NVIC configuration */	
  /* Configure the Priority Group to 0 bits */
	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	//Config QEI with highest interuppt
	NVIC_SetPriorityGrouping(3);
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_TIM10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM8_UP_TIM13_IRQn ;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
//Must first clear configuration the update flag
//TIM_ClearFlag(TIM1,TIM_FLAG_Update); 
//TIM_ClearFlag(TIM8,TIM_FLAG_Update); 
//TIM_ClearFlag(TIM3,TIM_FLAG_Update); 
//TIM_ClearFlag(TIM4,TIM_FLAG_Update);

	/* TIM IT enable */
//TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);
//TIM_ITConfig(TIM8, TIM_IT_Update, ENABLE);	
//TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
//TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);


  /* TIMx enable counter */
//  TIM_Cmd(TIM1, ENABLE);
//	TIM_Cmd(TIM8, ENABLE);
//	TIM_Cmd(TIM3, ENABLE);
//	TIM_Cmd(TIM4, ENABLE);
//	TIM_Cmd(TIM2, ENABLE);
//	TIM_Cmd(TIM5, ENABLE);
//	delay_ms(1000);
  Reset_QEI(0);
}

