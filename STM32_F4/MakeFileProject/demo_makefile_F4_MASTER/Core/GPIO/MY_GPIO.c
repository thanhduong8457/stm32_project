/*
 * MY_GPIO.c
 *
 *  Created on: May 4, 2016
 *      Author: AnhTuan
 */

#include "include.h"

void Driver_GPIO_Config(void)
{		
   GPIO_InitTypeDef  GPIO_InitStructure;
  /* GPIO Periph clock enable */
		 RCC_AHB1PeriphClockCmd(Servo_Ready_Clk_1, ENABLE);
		 RCC_AHB1PeriphClockCmd(Servo_Ready_Clk_2, ENABLE);
		 RCC_AHB1PeriphClockCmd(Servo_Ready_Clk_3, ENABLE);
		 RCC_AHB1PeriphClockCmd(Servo_Ready_Clk_4, ENABLE);
		 RCC_AHB1PeriphClockCmd(Servo_Ready_Clk_5, ENABLE);
		 RCC_AHB1PeriphClockCmd(Servo_Ready_Clk_6, ENABLE);
	
		RCC_AHB1PeriphClockCmd(Servo_In_Enable_Clk_1, ENABLE);
		RCC_AHB1PeriphClockCmd(Servo_In_Enable_Clk_2, ENABLE);
		RCC_AHB1PeriphClockCmd(Servo_In_Enable_Clk_3, ENABLE);
		RCC_AHB1PeriphClockCmd(Servo_In_Enable_Clk_4, ENABLE);
		RCC_AHB1PeriphClockCmd(Servo_In_Enable_Clk_5, ENABLE);
		RCC_AHB1PeriphClockCmd(Servo_In_Enable_Clk_6, ENABLE);
	
		RCC_AHB1PeriphClockCmd(Servo_OUT_Enable_Clk_1, ENABLE);
		RCC_AHB1PeriphClockCmd(Servo_OUT_Enable_Clk_2, ENABLE);
		RCC_AHB1PeriphClockCmd(Servo_OUT_Enable_Clk_3, ENABLE);
		RCC_AHB1PeriphClockCmd(Servo_OUT_Enable_Clk_4, ENABLE);
		RCC_AHB1PeriphClockCmd(Servo_OUT_Enable_Clk_5, ENABLE);
		RCC_AHB1PeriphClockCmd(Servo_OUT_Enable_Clk_6, ENABLE);
		
		RCC_AHB1PeriphClockCmd(LED_OUT_Clk, ENABLE);
		RCC_AHB1PeriphClockCmd(Clear_Error_Clk, ENABLE);
		RCC_AHB1PeriphClockCmd(SW_SL0_Clk, ENABLE);
		RCC_AHB1PeriphClockCmd(SW_SL1_Clk, ENABLE);
		RCC_AHB1PeriphClockCmd(Role_Clk, ENABLE);
	
//Config GPIO Mode OUTPUT 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
//Pin Servo_OUT_Enable
	GPIO_InitStructure.GPIO_Pin =  Servo_OUT_Enable_Pin_1 ;
  GPIO_Init(Servo_OUT_Enable_Port_1 , &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin =  Servo_OUT_Enable_Pin_2 ;
  GPIO_Init(Servo_OUT_Enable_Port_2 , &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin =  Servo_OUT_Enable_Pin_3 ;
  GPIO_Init(Servo_OUT_Enable_Port_3 , &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin =  Servo_OUT_Enable_Pin_4 ;
  GPIO_Init(Servo_OUT_Enable_Port_4 , &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin =  Servo_OUT_Enable_Pin_5 ;
  GPIO_Init(Servo_OUT_Enable_Port_5 , &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin =  Servo_OUT_Enable_Pin_6 ;
  GPIO_Init(Servo_OUT_Enable_Port_6 , &GPIO_InitStructure);
	
	//////***********/
	GPIO_InitStructure.GPIO_Pin = Servo_In_Enable_Pin_3;
  GPIO_Init(Servo_In_Enable_Port_3, &GPIO_InitStructure);
	
//Pin LED_OUT & Clear_Error &Role
	
	GPIO_InitStructure.GPIO_Pin =  LED_OUT_Pin  ;
  GPIO_Init(LED_OUT_Port , &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin =  Role_Pin ;
  GPIO_Init(Role_Port , &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin =  Clear_Error_Pin ;
  GPIO_Init(Clear_Error_Port , &GPIO_InitStructure);
	
		
//	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_5 ;
//  GPIO_Init(GPIOD , &GPIO_InitStructure);
//	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 ;
//  GPIO_Init(GPIOD , &GPIO_InitStructure);
	
/****************************************************************/
	
	//Config GPIO Mode INPUT 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	
//Pin Servo_Ready_Pin_	
//	GPIO_InitStructure.GPIO_Pin = Servo_Ready_Pin_1; // Use for USART2 Tx
//  GPIO_Init(Servo_Ready_Port_1, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = Servo_Ready_Pin_2;
  GPIO_Init(Servo_Ready_Port_2, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = Servo_Ready_Pin_3;
  GPIO_Init(Servo_Ready_Port_3, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = Servo_Ready_Pin_4;
  GPIO_Init(Servo_Ready_Port_4, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = Servo_Ready_Pin_5;
  GPIO_Init(Servo_Ready_Port_5, &GPIO_InitStructure);
	
//	GPIO_InitStructure.GPIO_Pin = Servo_Ready_Pin_6;// ERROR
//  GPIO_Init(Servo_Ready_Port_6, &GPIO_InitStructure);
	
	//Pin In_Enable_Pin	
	GPIO_InitStructure.GPIO_Pin = Servo_In_Enable_Pin_1;
  GPIO_Init(Servo_In_Enable_Port_1, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = Servo_In_Enable_Pin_2;
  GPIO_Init(Servo_In_Enable_Port_2, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = Servo_In_Enable_Pin_3;
  GPIO_Init(Servo_In_Enable_Port_3, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = Servo_In_Enable_Pin_4;
  GPIO_Init(Servo_In_Enable_Port_4, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = Servo_In_Enable_Pin_5;
  GPIO_Init(Servo_In_Enable_Port_5, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = Servo_In_Enable_Pin_6;
  GPIO_Init(Servo_In_Enable_Port_6, &GPIO_InitStructure);
	
	//Pin Switch 
	GPIO_InitStructure.GPIO_Pin = SW_SL0_Pin ;
  GPIO_Init(SW_SL0_Port, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = SW_SL1_Pin ;
  GPIO_Init(SW_SL1_Port, &GPIO_InitStructure);
	
}

