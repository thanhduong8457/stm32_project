/*
 *  external_interrupt.c
 *
 *  Created on: May 29, 2016
 *  Author: AnhTuan
 */

#include "include.h"
void EXTI0_IRQHandler(void) //		PE0 In_Possition 4
{
	if (EXTI_GetITStatus(EXTI_Line0) != RESET) {

		if(	GPIO_ReadInputDataBit(Servo_InPossition_Port_4,Servo_InPossition_Pin_4))// Important Check again
		{	
		Servo_Flag[3] = false;
		UARTprint(USARTx, "In_Possition 4");
		}
		EXTI_ClearITPendingBit(EXTI_Line0);
	}
}
void EXTI1_IRQHandler(void) //	  PD1 In_Possition 5
{
	if (EXTI_GetITStatus(EXTI_Line1) != RESET) {
		if(	GPIO_ReadInputDataBit(Servo_InPossition_Port_5,Servo_InPossition_Pin_5))// Important Check again
		{	
		//Servo_Flag[4] = false;
		UARTprint(USARTx, "In_Possition 5");
		}
		EXTI_ClearITPendingBit(EXTI_Line1);
	}
}
void EXTI2_IRQHandler(void) //		PC2 ERROR 
{
	if (EXTI_GetITStatus(EXTI_Line2) != RESET)
		{
		// Disable all Servo 
		GPIO_WriteBit(Servo_OUT_Enable_Port_1,Servo_OUT_Enable_Pin_1,1);
		GPIO_WriteBit(Servo_OUT_Enable_Port_2,Servo_OUT_Enable_Pin_2,1);
		GPIO_WriteBit(Servo_OUT_Enable_Port_3,Servo_OUT_Enable_Pin_3,1);
		GPIO_WriteBit(Servo_OUT_Enable_Port_4,Servo_OUT_Enable_Pin_4,1);
		GPIO_WriteBit(Servo_OUT_Enable_Port_5,Servo_OUT_Enable_Pin_5,1);
		GPIO_WriteBit(Servo_OUT_Enable_Port_6,Servo_OUT_Enable_Pin_6,1);	
		system_SetState(error);
		EXTI_ClearITPendingBit(EXTI_Line2);
	}
}
void EXTI3_IRQHandler(void) //		PC3 In_Possition 1
{
	if (EXTI_GetITStatus(EXTI_Line3) != RESET) 
		{
		if(GPIO_ReadInputDataBit(Servo_InPossition_Port_1,Servo_InPossition_Pin_1))// Important Check again
		{

		}
		EXTI_ClearITPendingBit(EXTI_Line3);
	}
}
void EXTI4_IRQHandler(void) //		PB4 SENSOR 3
{
	if (EXTI_GetITStatus(EXTI_Line4) != RESET) {

		UARTprint(USARTx, "4");
		EXTI_ClearITPendingBit(EXTI_Line4);
	}
}

void EXTI9_5_IRQHandler(void) {
	if (EXTI_GetITStatus(EXTI_Line5) != RESET) //	  PB5 SENSOR 4
	{

		UARTprint(USARTx, "5");

		EXTI_ClearITPendingBit(EXTI_Line5);
	 }
		else if (EXTI_GetITStatus(EXTI_Line6) != RESET) //  	PE6 SENSOR 1
			{

		UARTprint(USARTx, "6");

		EXTI_ClearITPendingBit(EXTI_Line6);
	   } 
		else if (EXTI_GetITStatus(EXTI_Line7) != RESET) //		PE7 In_Possition 2
		{
		if(	GPIO_ReadInputDataBit(Servo_InPossition_Port_2,Servo_InPossition_Pin_2))// Important Check again
		{	
		Servo_Flag[1] = false;
		UARTprint(USARTx, "In_Possition 2");
		}
		EXTI_ClearITPendingBit(EXTI_Line7);
	 } 
		else if (EXTI_GetITStatus(EXTI_Line8) != RESET) //		PB8 BUTTON
		{
	   //Save_Pos();
		//Flag_Slave_Receive_Finish=true;
	  UARTprint(USARTx, "PB8 BUTTON");
   	EXTI_ClearITPendingBit(EXTI_Line8);
	  } 
		else if (EXTI_GetITStatus(EXTI_Line9) != RESET) //		PB9 POWER CHECK 
		{
		if(	GPIO_ReadInputDataBit(Servo_POWER_CHECK_Port ,Servo_POWER_CHECK_Pin))// Important Check again
		{
		Save_Pos();
		UARTprint(USARTx, "POWER CHECK ");
		}
		EXTI_ClearITPendingBit(EXTI_Line9);
	  }
}
void EXTI15_10_IRQHandler(void) {

	if (EXTI_GetITStatus(EXTI_Line10) != RESET) //	  PC10 In_Possition 6
		{
		if(	GPIO_ReadInputDataBit(Servo_InPossition_Port_6,Servo_InPossition_Pin_6))// Important Check again
		{	
		Servo_Flag[5] = false;
		UARTprint(USARTx, "In_Possition 6");
		}
		EXTI_ClearITPendingBit(EXTI_Line10);
	}
		else if (EXTI_GetITStatus(EXTI_Line11) != RESET) //		PC11 SENSOR 6
		{
		UARTprint(USARTx, "11");
		EXTI_ClearITPendingBit(EXTI_Line11);
	   }
	else if (EXTI_GetITStatus(EXTI_Line12) != RESET) //	  PC12 SENSOR 5
	{
		UARTprint(USARTx, "12");
		EXTI_ClearITPendingBit(EXTI_Line12);
	}
	else if (EXTI_GetITStatus(EXTI_Line13) != RESET) //  	PE13 In_Possition 3
	{
		if(	GPIO_ReadInputDataBit(Servo_InPossition_Port_3,Servo_InPossition_Pin_3))// Important Check again
			{	
			Servo_Flag[2] = false;
			UARTprint(USARTx, "In_Possition 3");
			}
			EXTI_ClearITPendingBit(EXTI_Line13);
	}
	else if (EXTI_GetITStatus(EXTI_Line14) != RESET) //	  PC14 SENSOR 2
		{
		UARTprint(USARTx, "14");
		EXTI_ClearITPendingBit(EXTI_Line14);
	  } 
	else if (EXTI_GetITStatus(EXTI_Line15) != RESET) //	 LINE 15
		{
		UARTprint(USARTx, "15");
		EXTI_ClearITPendingBit(EXTI_Line15);
	  }
}


void EXTILine3_Config(void) {

	EXTI_InitTypeDef EXTI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	/* Enable GPIOA clock */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	/* Enable SYSCFG clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	/* Configure PA3 pin as input floating */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* Connect EXTI Line3 to PA3 pin */
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource3);

	/* Configure EXTI Line3 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line3;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	/* Enable and set EXTI Line3 Interrupt to the lowest priority */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

}

void  Driver_EXTI_Config(void)
	{
	EXTI_InitTypeDef EXTI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	/*
	 PE0 In_Possition 4
	 PD1 In_Possition 5
	 PC2 ERROR 
	 PC3 In_Possition 1
	 PB4 SENSOR 3
	 PB5 SENSOR 4
	 PE6 SENSOR 1
	 PE7 In_Possition 2
	 PB8 BUTTON
	 PB9 POWER CHECK 
	 PC10 In_Possition 6
	 PC11 SENSOR 6
	 PC12 SENSOR 5
	 PE13 In_Possition 3
	 PC14 SENSOR 2
	 */
	/* Enable GPIO clock */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

	/* Enable SYSCFG clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	/* Configure  SENSOR */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

	/* Configure PE6 SENSOR 1*/
	GPIO_InitStructure.GPIO_Pin = Servo_SensorLimit_Pin_1;
	GPIO_Init(Servo_SensorLimit_Port_1, &GPIO_InitStructure);

	SYSCFG_EXTILineConfig(Servo_SensorLimit_PortSource_1, Servo_SensorLimit_PinSource_1);

	/* Configure PC14 SENSOR 2*/

	GPIO_InitStructure.GPIO_Pin = Servo_SensorLimit_Pin_2;
	GPIO_Init(Servo_SensorLimit_Port_2, &GPIO_InitStructure);

	SYSCFG_EXTILineConfig(Servo_SensorLimit_PortSource_2, Servo_SensorLimit_PinSource_2);

	/* Configure PB4 SENSOR 3*/

	GPIO_InitStructure.GPIO_Pin = Servo_SensorLimit_Pin_3;
	GPIO_Init(Servo_SensorLimit_Port_3, &GPIO_InitStructure);

	SYSCFG_EXTILineConfig(Servo_SensorLimit_PortSource_3, Servo_SensorLimit_PinSource_3);;

	/* Configure PB5 SENSOR 4*/

	GPIO_InitStructure.GPIO_Pin = Servo_SensorLimit_Pin_4;
	GPIO_Init(Servo_SensorLimit_Port_4, &GPIO_InitStructure);

	SYSCFG_EXTILineConfig(Servo_SensorLimit_PortSource_4, Servo_SensorLimit_PinSource_4);

	/* Configure PC12 SENSOR 5*/

	GPIO_InitStructure.GPIO_Pin = Servo_SensorLimit_Pin_5;
	GPIO_Init(Servo_SensorLimit_Port_5, &GPIO_InitStructure);

	SYSCFG_EXTILineConfig(Servo_SensorLimit_PortSource_5, Servo_SensorLimit_PinSource_5);

	/* Configure PC11 SENSOR 6*/ // For usart
//	GPIO_InitStructure.GPIO_Pin = Servo_SensorLimit_Pin_6;
//	GPIO_Init(Servo_SensorLimit_Port_6, &GPIO_InitStructure);
//	SYSCFG_EXTILineConfig(Servo_SensorLimit_PortSource_6, Servo_SensorLimit_PinSource_6);

	/* Configure PC3 In_Possition 1*/

	GPIO_InitStructure.GPIO_Pin = Servo_InPossition_Pin_1;
	GPIO_Init(Servo_InPossition_Port_1, &GPIO_InitStructure);
	SYSCFG_EXTILineConfig(Servo_InPossition_PortSource_1, Servo_InPossition_PinSource_1);

	/* Configure PE7 In_Possition 2*/

	GPIO_InitStructure.GPIO_Pin = Servo_InPossition_Pin_2;
	GPIO_Init(Servo_InPossition_Port_2, &GPIO_InitStructure);
	SYSCFG_EXTILineConfig(Servo_InPossition_PortSource_2, Servo_InPossition_PinSource_2);

	/* Configure PE13 In_Possition 3*/

	GPIO_InitStructure.GPIO_Pin = Servo_InPossition_Pin_3;
	GPIO_Init(Servo_InPossition_Port_3, &GPIO_InitStructure);
	SYSCFG_EXTILineConfig(Servo_InPossition_PortSource_3, Servo_InPossition_PinSource_3);

	/* Configure PE0 In_Possition 4*/

	GPIO_InitStructure.GPIO_Pin = Servo_InPossition_Pin_4;
	GPIO_Init(Servo_InPossition_Port_4, &GPIO_InitStructure);
	SYSCFG_EXTILineConfig(Servo_InPossition_PortSource_4, Servo_InPossition_PinSource_4);

	/* Configure PD1 In_Possition 5*/

	GPIO_InitStructure.GPIO_Pin = Servo_InPossition_Pin_5;
	GPIO_Init(Servo_InPossition_Port_5, &GPIO_InitStructure);
	SYSCFG_EXTILineConfig(Servo_InPossition_PortSource_5, Servo_InPossition_PinSource_5);
	
	/* Configure PC10 In_Possition 6*/ // For usart

//	GPIO_InitStructure.GPIO_Pin = Servo_InPossition_Pin_6;
//	GPIO_Init(Servo_InPossition_Port_6, &GPIO_InitStructure);
//	SYSCFG_EXTILineConfig(Servo_InPossition_PortSource_6, Servo_InPossition_PinSource_6);

	/* Configure PC2 ERROR*/

	GPIO_InitStructure.GPIO_Pin = Servo_ERROR_Pin;
	GPIO_Init(Servo_ERROR_Port, &GPIO_InitStructure);
	SYSCFG_EXTILineConfig(Servo_ERROR_PortSource, Servo_ERROR_PinSource);

	/* Configure PB9 POWER CHECK */
	GPIO_InitStructure.GPIO_Pin = Servo_POWER_CHECK_Pin;
	GPIO_Init(Servo_POWER_CHECK_Port, &GPIO_InitStructure);
	SYSCFG_EXTILineConfig(Servo_POWER_CHECK_PortSource, Servo_POWER_CHECK_PinSource);
	/* Configure PB8 BUTTON*/
	GPIO_InitStructure.GPIO_Pin = Servo_BUTTON_Pin;
	GPIO_Init(Servo_BUTTON_Port, &GPIO_InitStructure);
	SYSCFG_EXTILineConfig(Servo_BUTTON_PortSource, Servo_BUTTON_PinSource);
//  LINE 15 FREE 

	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
		/*
	 PE0 In_Possition 4
	 PD1 In_Possition 5
	 PC2 ERROR 
	 PC3 In_Possition 1
	 PB4 SENSOR 3
	 PB5 SENSOR 4
	 PE6 SENSOR 1
	 PE7 In_Possition 2
	 PB8 BUTTON
	 PB9 POWER CHECK 
	 PC10 In_Possition 6
	 PC11 SENSOR 6
	 PC12 SENSOR 5
	 PE13 In_Possition 3
	 PC14 SENSOR 2
	 */
//	EXTI_InitStructure.EXTI_Line = EXTI_Line3;//Inpossition 1
//	EXTI_Init(&EXTI_InitStructure);
//	EXTI_InitStructure.EXTI_Line = EXTI_Line7;//Inpossition 2
//	EXTI_Init(&EXTI_InitStructure);
//	EXTI_InitStructure.EXTI_Line = EXTI_Line13;//Inpossition 3
//	EXTI_Init(&EXTI_InitStructure);
//	EXTI_InitStructure.EXTI_Line = EXTI_Line0;//Inpossition 4
//	EXTI_Init(&EXTI_InitStructure);
//	EXTI_InitStructure.EXTI_Line = EXTI_Line1;//Inpossition 5
//	EXTI_Init(&EXTI_InitStructure);
//	EXTI_InitStructure.EXTI_Line = EXTI_Line10;//Inpossition 6
//	EXTI_Init(&EXTI_InitStructure);
	
//	EXTI_InitStructure.EXTI_Line = EXTI_Line2;
//	EXTI_Init(&EXTI_InitStructure);
//	EXTI_InitStructure.EXTI_Line = EXTI_Line4;
//	EXTI_Init(&EXTI_InitStructure);
//	EXTI_InitStructure.EXTI_Line = EXTI_Line5;
//	EXTI_Init(&EXTI_InitStructure);
//	EXTI_InitStructure.EXTI_Line = EXTI_Line6;
//	EXTI_Init(&EXTI_InitStructure);
  EXTI_InitStructure.EXTI_Line = EXTI_Line8;//button PB8
  EXTI_Init(&EXTI_InitStructure);
//	EXTI_InitStructure.EXTI_Line = EXTI_Line9;
//	EXTI_Init(&EXTI_InitStructure);
//	EXTI_InitStructure.EXTI_Line = EXTI_Line11;
//	EXTI_Init(&EXTI_InitStructure);
//	EXTI_InitStructure.EXTI_Line = EXTI_Line12;
//	EXTI_Init(&EXTI_InitStructure);
//	EXTI_InitStructure.EXTI_Line = EXTI_Line14;
//	EXTI_Init(&EXTI_InitStructure);

	/* Enable and set Interrupt to the lowest priority */
	NVIC_SetPriorityGrouping(3);
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x03;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_Init(&NVIC_InitStructure);
	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
	NVIC_Init(&NVIC_InitStructure);
	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;
	NVIC_Init(&NVIC_InitStructure);
	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;
	NVIC_Init(&NVIC_InitStructure);
	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;
	NVIC_Init(&NVIC_InitStructure);
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_Init(&NVIC_InitStructure);
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_Init(&NVIC_InitStructure);

}

