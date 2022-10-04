/*
 *  system_manager.c
 *
 *  Created on: May 23, 2016
 *  Author: AnhTuan
 */
 
 #include "include.h"			
 
 volatile static system_state e_SystemState=initial_system;
 volatile bool servo_Enable_Flag[6]={true,true,true,true,true,true};
 volatile uint32_t user_pointer=startAddress_User_Program;
 volatile  bool QEI_Display=false;
 
 static uint32_t Loop_pointer;
 static uint32_t for_address_stack;// command FOR I 0 TO 5 
 static uint16_t for__start_index_stack;
 static uint16_t for__end_index_stack;

 

 system_state system_GetState(void)
{
	return e_SystemState;
}

void system_SetState(system_state SysState)
{
	e_SystemState = SysState;
	
}
 
void System_Error( char* error)
{
	while(1)
	{
		UARTprint(USARTx,error);
	  delay_ms(50000);
	}
}
void System_Initial(void)
{
	uint8_t rx_buffer[125];
	int i;
	uint16_t j;
	double Angle_value[6]={50.0,-44.0,-38.0,-180.0,83.0,50.0};
	int Delta_Q_Move[6]={15000,0,100000,10000,10000,0};
	//Driver_GPIO_Config(); // Setup Mode Input Or Output for task Read status Servo_Ready
												// Enable_Motor Switch ,Switch select mode 
												// & Write Enable_Servo ,Role (for Tool),Clear_error , Led_Out	
	ConfigQEI();          // Setup QEI for task read pulse from Encoder 
	//Driver_EXTI_Config(); //  Setup External Interrupt for task take event edge rising 
												// from Sensor Limit ,InPossition_Pin ,Event Error from Driver 
												// Button Press ,Power Off 
	USART_DMA();
	USART_Config();
//	UARTprint(USARTx,"read");
	//delay_ms(10000);
	TM_USB_VCP_Init();

//while(TM_USB_VCP_BufferFull()==0);
////UARTprint(USARTx,"full");

//	for(i=0;i<128;i++)
//		{
//		 UARTPut(USARTx,(uint8_t)TM_INT_USB_VCP_ReceiveBuffer[i]);
//		 while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);			
//		}

//	UARTprint(USARTx,"read");

//			
//	for(i=0;i<5;i++)
//	{
//	UARTprint(USARTx,"read");
//	delay_ms(50000);
//	}

//	while(1)
	{
	//UARTprint(USARTx,"start send ");
//	Rasp_Move_throught(1,6,50000);
	//delay_ms(30000);
	}
	
//	Make_DK(10.0,10.0,10.0,10.0,10.0,10.0);
////	for( i=0;i<500;i++)
//	{
	Move_Through_Tracking(1,6);		
//	while(Flag_Slave_Busy==true)
//	{}
//		delay_ms(20000);
//	}
//	while(1);
//SPI_Master_Config();
//Config_Create_Pluse();// Setup Generator Pulse 
//if(GPIO_ReadInputDataBit(SW_SL0_Port,SW_SL0_Pin))// LED indicator ,If work in LAB then ON else then OFF
//{
//	GPIO_ResetBits(LED_OUT_Port,LED_OUT_Pin);
//}
//else
//{
//	GPIO_SetBits(LED_OUT_Port,LED_OUT_Pin);
//}

	//Get_possition();			// Get possition from Flash Write into QEI register 
	//Load_possition();			// Load possition save into Ram 
	//Driver_Send_Setting(USART_DRIVER_1,5000,50,12500);


//Make_DK(0.0,-90.0,0.0,0.0,-90.0,-180.0);
//Move_Through(9,0,1);
//while(1);
//while(1)
//{
//if(Flag_Slave_Busy==false)
//Move_Current_Point(1,100);
//	//delay_ms(20000);
//}
//while(1);

// For simulation 
//for (i=0;i<6;i++)
//{
//	Set_QEI(i+1, (int)(Q_angle_old[i]*Index_Gear[i]));
//}
	system_SetState(read_enable);
}
 
void Read_Enable(void)
{
	if(GPIO_ReadInputDataBit(Servo_In_Enable_Port_1,Servo_In_Enable_Pin_1))
	{
		UARTprint(USARTx,"read 1 ok");
		servo_Enable_Flag[0]=true;
	}
		if(GPIO_ReadInputDataBit(Servo_In_Enable_Port_2,Servo_In_Enable_Pin_2))
	{
		UARTprint(USARTx,"read 2 ok");
		servo_Enable_Flag[1]=true;
	}
		if(GPIO_ReadInputDataBit(Servo_In_Enable_Port_3,Servo_In_Enable_Pin_3))
	{
		UARTprint(USARTx,"read 3 ok");
		servo_Enable_Flag[2]=true;
	}
		if(GPIO_ReadInputDataBit(Servo_In_Enable_Port_4,Servo_In_Enable_Pin_4))
	{
		UARTprint(USARTx,"read 4 ok");
		servo_Enable_Flag[3]=true;
	}
		if(GPIO_ReadInputDataBit(Servo_In_Enable_Port_5,Servo_In_Enable_Pin_5))
	{
		UARTprint(USARTx,"read 5 ok");
		servo_Enable_Flag[4]=true;
	}
		if(GPIO_ReadInputDataBit(Servo_In_Enable_Port_6,Servo_In_Enable_Pin_6))
	{
		UARTprint(USARTx,"read 6 ok");
		servo_Enable_Flag[5]=true;
	}
	
	// transfer to state clear_error 
	 system_SetState(clear_error);
	 UARTprint(USARTx,"Read_Enable");
}
void Clear_Error(void)
{
//	UARTprint(USARTx,"Clear Error");
//	GPIO_WriteBit(Clear_Error_Port,Clear_Error_Pin,0);
//	delay_ms(500);// delay 50 us 
//	GPIO_WriteBit(Clear_Error_Port,Clear_Error_Pin,1);
//	delay_ms(500);// delay 50 us 
// transfer to state enable_servo 
	system_SetState(enable_servo);
}
void Enable_Servo(void)//Enable Servo 
{
		if(true==servo_Enable_Flag[0])
		{
			GPIO_WriteBit(Servo_OUT_Enable_Port_1,Servo_OUT_Enable_Pin_1,0);
		}
		else
		{
			GPIO_WriteBit(Servo_OUT_Enable_Port_1,Servo_OUT_Enable_Pin_1,1);
		}
		
		if(true==servo_Enable_Flag[1])
		{
			GPIO_WriteBit(Servo_OUT_Enable_Port_2,Servo_OUT_Enable_Pin_2,0);
		}
		else
		{
			GPIO_WriteBit(Servo_OUT_Enable_Port_2,Servo_OUT_Enable_Pin_2,1);
		}
		if(true==servo_Enable_Flag[2])
		{
			GPIO_WriteBit(Servo_OUT_Enable_Port_3,Servo_OUT_Enable_Pin_3,0);
		}
		else
		{
			GPIO_WriteBit(Servo_OUT_Enable_Port_3,Servo_OUT_Enable_Pin_3,1);
		}
		if(true==servo_Enable_Flag[3])
		{
			GPIO_WriteBit(Servo_OUT_Enable_Port_4,Servo_OUT_Enable_Pin_4,0);
		}
		else
		{
			GPIO_WriteBit(Servo_OUT_Enable_Port_4,Servo_OUT_Enable_Pin_4,1);
		}
		if(true==servo_Enable_Flag[4])
		{
			GPIO_WriteBit(Servo_OUT_Enable_Port_5,Servo_OUT_Enable_Pin_5,0);
		}
		else
		{
			GPIO_WriteBit(Servo_OUT_Enable_Port_5,Servo_OUT_Enable_Pin_5,1);
		}

		if(true==servo_Enable_Flag[5])
		{
			GPIO_WriteBit(Servo_OUT_Enable_Port_6,Servo_OUT_Enable_Pin_6,0);
		}
		else
		{
			GPIO_WriteBit(Servo_OUT_Enable_Port_6,Servo_OUT_Enable_Pin_6,1);
		}
				
	// transfer to state waiting_servo_ready 
	 system_SetState(waiting_servo_ready);
	 UARTprint(USARTx,"Enable_Servo");
	
}
void Waiting_Servo_Ready(void)// Waiting for servo Ready 
{
	int i=0;
		for(i=0;i<1;i++)
	{
		delay_ms(10000);//delay (10000) == 1s
		UARTprint(USARTx,"Waiting_Servo_Ready");
	}
	// transfer to state waiting_servo_ready 
	system_SetState(check_servo_ready);
}
void Check_Servo_Ready(void)
{
	   system_SetState(initial_success);
//	//(GPIO_ReadInputDataBit(Servo_Ready_Port_1,Servo_Ready_Pin_1)==servo_Enable_Flag[0]) //PA2 USED FOR USART
//	//&(GPIO_ReadInputDataBit(Servo_Ready_Port_6,Servo_Ready_Pin_6)==servo_Enable_Flag[5])
//	if((GPIO_ReadInputDataBit(Servo_Ready_Port_2,Servo_Ready_Pin_2)==servo_Enable_Flag[1])
//		&(GPIO_ReadInputDataBit(Servo_Ready_Port_3,Servo_Ready_Pin_3)==servo_Enable_Flag[2])	  
//	  &(GPIO_ReadInputDataBit(Servo_Ready_Port_4,Servo_Ready_Pin_4)==servo_Enable_Flag[3])
//	  &(GPIO_ReadInputDataBit(Servo_Ready_Port_5,Servo_Ready_Pin_5)==servo_Enable_Flag[4])
//	  )
//		{
//		 // transfer to state initial_success 
//	   system_SetState(initial_success);			
//		}
//		else
//		{
//		 // transfer to state error 
//	   system_SetState(error);		
//		}
			UARTprint(USARTx,"Check_Servo_Ready");
}

void Initial_Success(void)
{
	//buzzer success
	// transfer to state initial_success 
	system_SetState(waitting);
	UARTprint(USARTx,"Initial_Success");

}
void Error(void)
{
	//buzzer error	
	UARTprint(USARTx,"SYSTEM ERROR ");
  delay_ms(10000);//delay (10000) == 1s
} 
void Home(void)
{
	//Home 
	int i; int delta_Q[6];	
	for(i=0;i<6;i++)
	{
		delta_Q[i]=0- Get_QEI(i+1);
	}
	Planing_Path_Free( delta_Q,50);		
	if(true==Flag_Slave_Busy)
	{		
		UARTprint(USARTx,"Homing");
	}
	//
	while (true==Flag_Slave_Busy)// wait for until Robot complete Home
	{
	  delay_ms(10);
	}
	 UARTprint(USARTx,"Home done");		
	 //transfer to state waitting
	 system_SetState(waitting);
}
void Waitting(void)
{
	UARTprint(USARTx,"Waitting");
	while(system_GetState()==waitting)
	{
	delay_ms(100);
	//UARTprint(USARTx,"Waitting");
	}
}
void Open_loop(void)
 {	
	 			while(open_loop==system_GetState())
				{				
			  switch(Read_Byte(user_pointer))
				{
					case 10 :	// Function Ex: MOVES A B SPEED 	9 byte
									Move_Through(Read_HalfWord(user_pointer+1),Read_HalfWord(user_pointer+3),Read_Word(user_pointer+5));					
						      user_pointer+=9;
						break;
					case 11://  Function Ex: TOOL ON/OFF
						if(1==Read_Byte(user_pointer+1))
						{
						UARTprint(USARTx,"TOOL OPEN "); 
						GPIO_SetBits(Role_Port,Role_Pin);
						}
						else
						{
						UARTprint(USARTx,"TOOL CLOSE "); 
						GPIO_ResetBits(Role_Port,Role_Pin);
						}
						delay_ms(10000);//delay (10000) == 1s
						user_pointer+=2;
						break;
					case 12: //  Function Ex: LOOP
						user_pointer+=1;// save possition of Loop
						Loop_pointer=user_pointer;
						break;
					case 13: //  Function Ex: ENDLOOP
						user_pointer=Loop_pointer;// progarm will jump to Loop possition
						break;
					case 14: //  Function Ex: MOVE PoSX SPEED 7 byte 	
						Move_Current_Point(Read_HalfWord(user_pointer+1),Read_Word(user_pointer+3));
				    waitting_moving();
					  user_pointer+=7;
						break;
					 case 15: //  Function Ex: DELAY 10000 5 byte 	
					 {
						 uint32_t i,j;
						 j=Read_Word(user_pointer+1);//delay_ms(10) =1ms
						 Print(USARTx,"Delay "); 
						 UARTPutnlong(USARTx,j);
					   USART_SendData(USARTx,(uint16_t)10);
						 while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);	
						 for(i=0;i <j;i++)
						 {
							 delay_ms(10);
						 }
					  user_pointer+=5;
					 }
					 break;
					 case 16://FOR I 1 TO 5 ;
					 {
						 for__start_index_stack=Read_HalfWord(user_pointer+1);
						 for__end_index_stack=Read_HalfWord(user_pointer+3);
						 for_address_stack=user_pointer+5;			//save poisstion need to loop 		 
					 }
						  Print(USARTx,"for__start_index_stack ");
	 	        	UARTPutnlong(USARTx,for__start_index_stack);
		          USART_SendData(USARTx,(uint16_t)10);
              while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);					 
					    user_pointer+=5;// function 5 byte 
						break;
					 case 17: //ENDFOR
					 {
						 if(for__end_index_stack > for__start_index_stack)
						 {
							 for__start_index_stack++;
						 }
						 if(for__end_index_stack < for__start_index_stack)
						 {
							 for__start_index_stack--;
						 }
						 
						 if(for__end_index_stack == for__start_index_stack)
						 {
						   user_pointer+=1;// out of for 
						 }
						 else
						 {
							 user_pointer=for_address_stack; // come back loop
						 }					 

						 Print(USARTx,"for__start_index_stack ");
	 	        	UARTPutnlong(USARTx,for__start_index_stack);
		          USART_SendData(USARTx,(uint16_t)10);
              while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);	 						 
					 } 
						 break;
					 	case 18://13 byte  INV_MOVE_STRAIGHT 1 1 1;
						{
						uint16_t i,j;
						uint8_t Buffer[4];
						float Pos_Temp[3];
						for(j=0;j<3;j++)
							{
									for(i=0;i<4;i++)
									{
										Buffer[i]=Read_Byte(user_pointer+1+i+4*j);
									}
									memcpy(&Pos_Temp[j], Buffer,4);
							}
							UARTprint(USARTx,"Start Invert**");			
							Planing_Path_Strain_Line_Tracking(Pos_Temp[0],Pos_Temp[1],Pos_Temp[2]);
																										
					  user_pointer+=13;// function 13 byte 
						}
						break;
					default :						
							UARTprint(USARTx,"USER PROGRAM FINISH ");
					    delay_ms(50000);
              Display_QEI();
							break;
				}
			}	
 
 } 
 
 void Detect_position(void)
 {
	 Display_QEI();
	 if(Flag_Request_App==true)
	 {
		UART_Send_Possition(USARTx,Get_QEI(1),Get_QEI(2),Get_QEI(3),Get_QEI(4), Get_QEI(5),Get_QEI(6));
	  	Flag_Request_App=false;
	 }
 
 }
 
 void Inverse_Kinematic(void)
 {
 }

 void Display_QEI(void)
 {
	 if(true==QEI_Display)
	 {
	 	UARTPutnlong(USARTx,Get_QEI(1));
		USART_SendData(USARTx,(uint16_t)' '); 
		while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);	 	
		UARTPutnlong(USARTx,Get_QEI(2));
		USART_SendData(USARTx,(uint16_t)' '); 
		while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);	 	
		UARTPutnlong(USARTx,Get_QEI(3));
		USART_SendData(USARTx,(uint16_t)' '); 
		while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);	 	
		UARTPutnlong(USARTx,Get_QEI(4));
		USART_SendData(USARTx,(uint16_t)' ');      
		while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);	 	
		UARTPutnlong(USARTx,Get_QEI(5));
		USART_SendData(USARTx,(uint16_t)' '); 
    	while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);	 	
	  	UARTPutnlong(USARTx,Get_QEI(6));
		USART_SendData(USARTx,(uint16_t)' '); 
	  	while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);	 	
	  	USART_SendData(USARTx,(uint16_t)10); 
    	delay_ms(3000); 
	 }
 }
 