/*
 * Driver_Interface.c
 *
 *  Created on: Jul 31, 2016
 *      Author: AnhTuan
 */

#include "include.h"



 void USART_DRIVER_1_Config(void)
{
  USART_InitTypeDef USART_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  
  /* Enable GPIO clock */
  RCC_AHB1PeriphClockCmd(USART_DRIVER_1_TX_GPIO_CLK | USART_DRIVER_1_RX_GPIO_CLK, ENABLE);
  
  /* Enable USART clock */
  USART_DRIVER_1_CLK_INIT(USART_DRIVER_1_CLK, ENABLE);
  
  /* Connect USART pins to AF */
  GPIO_PinAFConfig(USART_DRIVER_1_TX_GPIO_PORT, USART_DRIVER_1_TX_SOURCE, USART_DRIVER_1_TX_AF);
  GPIO_PinAFConfig(USART_DRIVER_1_RX_GPIO_PORT, USART_DRIVER_1_RX_SOURCE, USART_DRIVER_1_RX_AF);
  
  /* Configure USART Tx and Rx as alternate function push-pull */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Pin = USART_DRIVER_1_TX_PIN;
  GPIO_Init(USART_DRIVER_1_TX_GPIO_PORT, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = USART_DRIVER_1_RX_PIN;
  GPIO_Init(USART_DRIVER_1_RX_GPIO_PORT, &GPIO_InitStructure);

  /* Enable the USART OverSampling by 8 */
  USART_OverSampling8Cmd(USART_DRIVER_1, ENABLE);  

  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_9b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_Even;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USART_DRIVER_1, &USART_InitStructure);
  
//  /* NVIC configuration */	
//	NVIC_SetPriorityGrouping(3);
//  /* Enable the USART_DRIVER_1 Interrupt */
//  NVIC_InitStructure.NVIC_IRQChannel = USART_DRIVER_1_IRQn;
//  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//  NVIC_Init(&NVIC_InitStructure);
//	
 //ENABLE UASRT RX INTERRUPT
	//USART_ITConfig(USART_DRIVER_1, USART_IT_RXNE, ENABLE);
	//ENABLE UASRT TX INTERRUPT
  //USART_ITConfig(USART_DRIVER_1, USART_IT_TXE, ENABLE);
  /* Enable USART */
  USART_Cmd(USART_DRIVER_1, ENABLE);	
}


void Driver_Setting_All(float Acc[6],float Speed[6])
{
	int i;
	USART_TypeDef* Driver_USART[6] ={ USART1,USART2,USART3,UART4,UART5,USART6};

	for( i=0;i<6;i++)
	{
		Driver_Send_Setting(Driver_USART[i],Acc[i],Acc[i],Speed[i]);
	}	
}

void Driver_Send_Setting(USART_TypeDef* UART_Base, float acc,float dec,float speed)
{
						int i;
	          int8_t send[48];
						float test ;
//						char arr[sizeof(float)];// Convert double to array byte
//				   	memcpy(arr,&test,sizeof(float));
	
						char arr[4];// Convert float to array 4 byte
	          test=acc;
				   	memcpy(arr,&test,4);
            //acc
            send[0] = 0xC1;
            send[1] = 0x0B;
            send[2] = 0x00;
            send[3] = 0x01;
            send[4] = 0x00;
            send[5] = 0xC0;
            send[6] = 0x80;
            send[10]= arr[1];
            send[9] = arr[0];
            send[8] = arr[3];
            send[7] = arr[2];
            send[11] = 0;
						//Checksum calculate
            for (i = 0; i < 11; i++)
            {
                send[11] += send[i];
            }
            //dec
						test=dec;
				   	memcpy(arr,&test,4);
            send[0 + 12] = 0xC2;
            send[1 + 12] = 0x0B;
            send[2 + 12] = 0x00;
            send[3 + 12] = 0x01;
            send[4 + 12] = 0x00;
            send[5 + 12] = 0xC0;
            send[6 + 12] = 0x82;
            send[10 + 12] = arr[1];
            send[9 + 12] = arr[0];
            send[8 + 12] = arr[3];
            send[7 + 12] = arr[2];
            send[11 + 12] = 0;
            for ( i = 0; i < 11; i++)
            {
                send[11 + 12] += send[i + 12];
            }
            //dec skill
						test=acc*2;
				   	memcpy(arr,&test,4);
            send[0 + 24] = 0xC3;
            send[1 + 24] = 0x0B;
            send[2 + 24] = 0x00;
            send[3 + 24] = 0x01;
            send[4 + 24] = 0x00;
            send[5 + 24] = 0xC0;
            send[6 + 24] = 0x84;
            send[10 + 24]= arr[1];
            send[9 + 24] = arr[0];
            send[8 + 24] = arr[3];
            send[7 + 24] = arr[2];
            send[11 + 24] = 0;

            for ( i = 0; i < 11; i++)
            {
                send[11 + 24] += send[i + 24];
            }
						//Speed
						test=speed;
				   	memcpy(arr,&test,4);
            send[0 + 36] = 0xC4;
            send[1 + 36] = 0x0B;
            send[2 + 36] = 0x00;
            send[3 + 36] = 0x01;
            send[4 + 36] = 0x00;
            send[5 + 36] = 0xC0;
            send[6 + 36] = 0x86;
            send[10 + 36]= arr[1];
            send[9 + 36] = arr[0];
            send[8 + 36] = arr[3];
            send[7 + 36] = arr[2];
            send[11 + 36] = 0;

            for ( i = 0; i < 11; i++)
            {
                send[11 + 36] += send[i + 36];
            }
						
           	for(i=0;i< 48;i++)
						{
								USART_SendData(UART_Base, send[i]);
								while (USART_GetFlagStatus(UART_Base, USART_FLAG_TXE) == RESET);
						}
}