
/*
 * UART.c
 *
 *  Created on: May 3, 2016
 *      Author: AnhTuan
 */

#include "include.h"
#include "MYUART.h"

static uint16_t		BUFF_SIZE	=	2;
static uint16_t		BUFF_SIZE_USART_1	=	1;
static uint8_t    rxbuff[512];// maximum 512 byte 
static uint8_t    Rx_USART1_Buffer[2];
/* Flag is to indicate type of Package*/
volatile bool Flag_Request_App=false;

static bool Flag_package_Relative_Moving =false;
static bool Flag_package_Relative_joint=false;
static bool Flag_package_Write_pos=false;
static bool Flag_package_User_program_header=false;
static bool Flag_package_User_program_core=false;
static bool Flag_package_Moves_through=false;
static bool Flag_package_Move_Invert=false;
static bool Flag_package_Moves_direct=false;
static bool Flag_package_Write_QEI=false;
static bool Flag_package_Write_Angle=false;
static bool Flag_package_COPY=false;
static bool Flag_package_Clear_Servo_Flag=false;
static bool Flag_package_acceleration_mcu=false;

static int32_t temp;// variable for convert byte[] to int 
static uint16_t joint;

static bool Flag_Servo_Temp[6];// bien luu lai trang thai truoc khi servo pause 

/*private function*/
unsigned short  GetCRC(uint8_t  message[], unsigned short lenght );//CRC Modbus standard
unsigned short crc16(int8_t *data_p, unsigned short length);
void USARTx1_DMA_RX_IRQHandler(void)// USART Slave Send To Master
{
	 /* Clear the DMA1_Streamx TCIFx pending bit */
  DMA_ClearITPendingBit(USARTx1_RX_DMA_STREAM, USARTx1_RX_DMA_FLAG_TCIF);
	DMA_Cmd(USARTx1_RX_DMA_STREAM, DISABLE);	
	if(Rx_USART1_Buffer[0]==50)
	{
	  UARTprint(USARTx,"Slave control Ok**");
		Flag_Slave_Busy=false;
	}
  else if	(Rx_USART1_Buffer[0]==51)
	{
		Flag_Slave_Receive_Finish=true;
	}				
 //Re enable DMA
	USARTx1_RX_DMA_STREAM->NDTR = (uint32_t)BUFF_SIZE_USART_1;
	DMA_Cmd(USARTx1_RX_DMA_STREAM, ENABLE);	
}

void USARTx_DMA_IRQHandler(void) // bao day va copy du lieu ra 
{
  /* Clear the DMA1_Streamx TCIFx pending bit */
  DMA_ClearITPendingBit(USARTx_DMA_Stream, USARTx_DMA_FLAG);
	DMA_Cmd(USARTx_DMA_Stream, DISABLE);	
if(true==Flag_package_Relative_Moving)
{
	if(system_GetState()==detect_possition)
	{
		if(false==Flag_Slave_Busy)
		{
		uint16_t i;
		int Delta_Q_Move_Temp[6]={0,0,0,0,0,0};
		uint32_t temp_speed=(int32_t)(rxbuff[24] << 24) | (int32_t)(rxbuff[25] << 16) | (int32_t)(rxbuff[26] << 8) | (int32_t)(rxbuff[27]);	
		for(i=0;i<6;i++)
		{
			if(true==servo_Enable_Flag[i])
			{
				temp = (int32_t)(rxbuff[0+4*i] << 24) | (int32_t)(rxbuff[1+4*i] << 16) | (int32_t)(rxbuff[2+4*i] << 8) | (int32_t)(rxbuff[3+4*i]);
				if(((Get_QEI(i+1)+temp )>(High_limit[i])*(Index_Gear[i]))|((Get_QEI(i+1)+temp )<(low_limit[i])*(Index_Gear[i])))
				{
						UARTprint(USARTx,"Joint x Over Limit Moving");					
				}			
				else
				{
				Delta_Q_Move_Temp[i]=temp;			
				}
		  }
		}		
		UARTprint(USARTx,"Robot Moving");					
		Planing_Path_Free(Delta_Q_Move_Temp,temp_speed);		
    }
		else
		{
			UARTprint(USARTx,"Wait for Motor Stop ***");
		}
	}
	else
	{
		UARTprint(USARTx,"Please Transfer to Mode Dectect Possition***");
	}
	  Flag_package_Relative_Moving=false;	
		BUFF_SIZE=2;	
}

else if(true==Flag_package_acceleration_mcu)
{
	if(system_GetState()==detect_possition)
	{
		if(false==Flag_Slave_Busy)
		{
		int tempx=(int32_t)(rxbuff[0] << 24) | (int32_t)(rxbuff[1] << 16) | (int32_t)(rxbuff[2] << 8) | (int32_t)(rxbuff[3]);
		int tempy=(int32_t)(rxbuff[4] << 24) | (int32_t)(rxbuff[5] << 16) | (int32_t)(rxbuff[6] << 8) | (int32_t)(rxbuff[7]);
		/*
		(90*80*10000)/360*(1000 range of tempx)
		*/		
		tempx=tempx* 200-Get_QEI(4);
		
		//Creat_Pulse(tempx/Servo_Gear[3],50000/Servo_Gear[3],3);		
		//Planing_Path_Free( Delta_Q_Move_Temp,temp_speed);		
		tempy=tempy* 180-Get_QEI(5);
		//Creat_Pulse(tempy/Servo_Gear[4],50000/Servo_Gear[4],4);		
   }
		else
		{
			UARTprint(USARTx,"Wait for Motor Stop ***");
		}
	}
	else
	{
		UARTprint(USARTx,"Please Transfer to Mode Dectect Possition***");
	}
	  Flag_package_acceleration_mcu=false;	
		BUFF_SIZE=2;	
}
else if(true==Flag_package_Relative_joint)
{
int Delta_Q_Move_Temp[6]={0,0,0,0,0,0};
    Flag_package_Relative_joint=false;
	if(system_GetState()==detect_possition)
	{
		  joint=(uint16_t)rxbuff[0];
			if((false==Flag_Slave_Busy) & (true==servo_Enable_Flag[joint]))
			{
				uint32_t temp_speed = (uint32_t)(rxbuff[5] << 24) | (uint32_t)(rxbuff[6] << 16) | (uint32_t)(rxbuff[7] << 8) | (uint32_t)(rxbuff[8]);
				temp = (int32_t)(rxbuff[1] << 24) | (int32_t)(rxbuff[2] << 16) | (int32_t)(rxbuff[3] << 8) | (int32_t)(rxbuff[4]);				
				if(((Get_QEI(joint+1)+temp )>(High_limit[joint])*(Index_Gear[joint]))|((Get_QEI(joint+1)+temp )<(low_limit[joint])*(Index_Gear[joint])))
				{
						UARTprint(USARTx,"Joint x Over Limit Moving");					
				}			
				else
				{
				Delta_Q_Move_Temp[joint]=temp;
				UARTprint(USARTx,"Robot Moving");				
       	Planing_Path_Free( Delta_Q_Move_Temp,temp_speed);					
				}
			}
			else if( false==servo_Enable_Flag[joint])
			{
				UARTprint(USARTx,"Joint is disabled");
			}
				else
			{
				UARTprint(USARTx,"Wait for Robot Stop ***");
			}			
	}
		else
	{
		UARTprint(USARTx,"Please Transfer to Mode Dectect Possition***");
	}

		BUFF_SIZE=2;			
}
else if(true==Flag_package_Write_pos)
{
Flag_package_Write_pos=false;
	if(system_GetState()==detect_possition)
	{
		uint16_t i, temp_id;
		temp_id=(uint16_t) (rxbuff[0]<<8) | (uint16_t)rxbuff[1];
		if(temp_id < (buffer_point-1))
		{
		for(i=0;i<6;i++) //0 --> 5 for posstion and  6 for speed
		{
				temp = (int32_t)(rxbuff[2+0+4*i] << 24) | (int32_t)(rxbuff[2+1+4*i] << 16) | (int32_t)(rxbuff[2+2+4*i] << 8) | (int32_t)(rxbuff[2+3+4*i]);			
				Servo_Pluse_buffer[temp_id][i]=temp;
		}
			UARTprint(USARTx,"Write Pos Done ");
	  }
		else
		{
			Print(USARTx,"ERROR - ID < Buffer_point:=  ");
			UARTPutnlong(USARTx,buffer_point);
		  USART_SendData(USARTx,(uint16_t)10);
			while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);			
		}
	}
		else
	{
		UARTprint(USARTx,"Please Transfer to Mode Dectect Possition***");
	}
		BUFF_SIZE=2;			
}
else if(true==Flag_package_Write_QEI)
{
	uint16_t i;
	Flag_package_Write_QEI=false;
	if(system_GetState()==detect_possition)
	{
		for(i=0;i<6;i++) //0 --> 5 for posstion each QEI  
		{
				temp = (int32_t)(rxbuff[0+4*i] << 24) | (int32_t)(rxbuff[1+4*i] << 16) | (int32_t)(rxbuff[2+4*i] << 8) | (int32_t)(rxbuff[3+4*i]);
				Set_QEI( (i+1), temp );		
		}
			UARTprint(USARTx,"Write QEI DONE *** ");
	}
		else
	{
		UARTprint(USARTx,"Please Transfer to Mode Dectect Possition***");
	}		
		BUFF_SIZE=2;					
}
else if(true==Flag_package_Write_Angle)
{
	// Nen luu goc sau do convert to QEI and save to QEI register 
	uint16_t i,j;
	int32_t inver_temp[6];
	uint8_t Buffer[4];//={0x00,0x00,0xB4,0xC2};
	float Angle_Temp;
	float Angle_value[6];
	Flag_package_Write_Angle=false;
	if(system_GetState()==detect_possition)
	{
		UARTprint(USARTx,"Write Angle &QEI *** ");
		for(j=0;j<6;j++)
		{
				for(i=0;i<4;i++)
				{
					Buffer[i]=rxbuff[i+4*j];
				}
				memcpy(&Angle_Temp, Buffer,4);
				Set_QEI(j+1, (int)(Angle_Temp*Index_Gear[j]));				
				//Angle_value[j]=Angle_Temp;			
				//Q_angle_old[j]=Angle_Temp;
	  }
		Make_DK_Current_QEI();
	//	Make_DK(Angle_value[0],Angle_value[1],Angle_value[2],Angle_value[3],Angle_value[4],Angle_value[5]);	
	}
		else
	{
		UARTprint(USARTx,"Please Transfer to Mode Dectect Possition***");
	}		
		BUFF_SIZE=2;					
}
else if(true==Flag_package_Move_Invert)
{
	uint16_t i,j;
	uint8_t Buffer[4];
	float Pos_Temp[3];
	Flag_package_Move_Invert=false;
	if(system_GetState()==detect_possition)
	{
		if(false==Flag_Slave_Busy)
		{
		for(j=0;j<3;j++)
		{
				for(i=0;i<4;i++)
				{
					Buffer[i]=rxbuff[i+4*j];
				}
				memcpy(&Pos_Temp[j], Buffer,4);
	  }
		UARTprint(USARTx,"Start Invert**");			
		//Planing_Path_Strain_Line_Tracking(Pos_Temp[0],Pos_Temp[1],Pos_Temp[2]);
		Rasp_Move_invert_delta(Pos_Temp[0],Pos_Temp[1],Pos_Temp[2]);
	  }
		else
		{
			UARTprint(USARTx,"Wait for Robot Stop ***");
		}
	}
		else
	{
		UARTprint(USARTx,"Please Transfer to Mode Dectect Possition***");
	}		
		BUFF_SIZE=2;					
}
else if(true==Flag_package_COPY)
{
Flag_package_COPY=false;
	if(system_GetState()==detect_possition)
	{
		   uint16_t i;
		   uint16_t A =((uint16_t)(rxbuff[0] << 8) | (uint16_t)(rxbuff[1] << 0));
			 uint16_t B =((uint16_t)(rxbuff[2] << 8) | (uint16_t)(rxbuff[3] << 0));
		   for(i=0;i<6;i++)
					{
					Servo_Pluse_buffer[B][i] = Servo_Pluse_buffer[A][i];
					}		
			UARTprint(USARTx,"Copy DONE **");
	}
		else
	{
		UARTprint(USARTx,"Please Transfer to Mode Dectect Possition***");
	}
		BUFF_SIZE=2;			
}
else if(true==Flag_package_Clear_Servo_Flag)
{
Flag_package_Clear_Servo_Flag=false;
// Kiem tra lai .Vi New version Ko con dung Servo_flag    if(false==Flag_Slave_Busy)
	if(system_GetState()==detect_possition)
	{
	 uint16_t A =((uint16_t)(rxbuff[0] << 8) | (uint16_t)(rxbuff[1] << 0));
		if(A==7)
		{
				Servo_Flag[0] = Servo_Flag[1]=Servo_Flag[2]=Servo_Flag[3]=Servo_Flag[4]=Servo_Flag[5]=false;
						UARTprint(USARTx,"Clear ALL Servo_Flag : ");
		}
		else
		{
			Servo_Flag[A-1]=false;
			Print(USARTx,"Clear Servo_Flag : ");
		  UARTPutnlong(USARTx,A);	
	  	USART_SendData(USARTx,(uint16_t)10); 
		  while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);	
		}
	}
	else
	{
		UARTprint(USARTx,"Please Transfer to Mode Dectect Possition***");
	}
		BUFF_SIZE=2;			
}
else if(true==Flag_package_User_program_core)
{
  uint16_t crc;
  Flag_package_User_program_core=false;
	Print(USARTx,"Lenght Package : ");					
  UARTPutnlong(USARTx,BUFF_SIZE);
	USART_SendData(USARTx,(uint16_t)10);
	while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);	
	if(system_GetState()==detect_possition)
	{
			if(false==Flag_Slave_Busy)	
			{
				//Check CRC 				
				crc=GetCRC(rxbuff,BUFF_SIZE);
//				Print(USARTx," CRC ");					
//				UARTPutnlong(USARTx,crc);
//				Print(USARTx," CRC low ");					
//				UARTPutnlong(USARTx,rxbuff[BUFF_SIZE-1]);
//				Print(USARTx," CRC high ");					
//				UARTPutnlong(USARTx,rxbuff[BUFF_SIZE-2]);				
					if(((uint8_t)((crc>>8)& 0xFF)==rxbuff[BUFF_SIZE-1])&((uint8_t)(crc & 0xFF)==rxbuff[BUFF_SIZE-2]))// Check sum
					{
						UARTprint(USARTx,"Receive Success");
						Write_User_Program(rxbuff,BUFF_SIZE-2);
					}
					else
					{
						UARTprint(USARTx," Receive Fail---> CRC Error");							
					}				
			}
	else
	{
		UARTprint(USARTx,"Please Wait for Robot Stop or You Stop Robot***");
		
	}
}
	else
	{
		UARTprint(USARTx,"Please Transfer to Mode Dectect Possition***");		
	}
	BUFF_SIZE=2;			
}
else if(true==Flag_package_Moves_direct)
{
	UARTprint(USARTx,"Moves_Direct : ");	
  Flag_package_Moves_direct=false;
	if(system_GetState()==detect_possition)
	{
			if(false==Flag_Slave_Busy)
			{			
				uint16_t  temp_A= (uint16_t)(rxbuff[0] << 8) | (uint16_t)(rxbuff[1] ); 
				uint32_t  temp_speed=(int32_t)(rxbuff[2] << 24) | (int32_t)(rxbuff[3] << 16) | (int32_t)(rxbuff[4] << 8) | (int32_t)(rxbuff[5]);
				UARTprint(USARTx,"Robot Moving : ");	
				Move_Current_Point(temp_A,temp_speed);	
			}
			else
			{
				UARTprint(USARTx,"Please Wait for Robot Stop ***");	
			}
   }
	else
	{
		UARTprint(USARTx,"Please Transfer to Mode Dectect Possition***");		
	}
	BUFF_SIZE=2;			
}
else if(true==Flag_package_Moves_through)
{
	UARTprint(USARTx,"Moves_through : ");	
  Flag_package_Moves_through=false;
	if(system_GetState()==detect_possition)
	{
			if(false==Flag_Slave_Busy)
			{	
	    /* Warning USART Interrupt priority have to lower USB OTG .Unless USB OTG can't operation send data */				
				uint16_t  temp_A= (uint16_t)(rxbuff[0] << 8) | (uint16_t)(rxbuff[1] ); 
				uint16_t  temp_B= (uint16_t)(rxbuff[2] << 8) | (uint16_t)(rxbuff[3] ); 
				uint32_t  temp_speed=(int32_t)(rxbuff[4] << 24) | (int32_t)(rxbuff[5] << 16) | (int32_t)(rxbuff[6] << 8) | (int32_t)(rxbuff[7]);
				UARTprint(USARTx,"Robot Moving : ");	
				//Move_Through(temp_A,temp_B,temp_speed);	 oldest
				Rasp_Move_throught(temp_A,temp_B,temp_speed);	
				//Creat_Move_Through(temp_A,temp_B,temp_speed);	
				UARTprint(USARTx,"break : ");	
			}
			else
			{
				UARTprint(USARTx,"Please Wait for Robot Stop ***");	
			}
    }
	else
	{
		UARTprint(USARTx,"Please Transfer to Mode Dectect Possition***");		
	}
	
	BUFF_SIZE=2;			
}
else if(true==Flag_package_User_program_header)
{
		        Flag_package_User_program_header=false;
						Flag_package_User_program_core=true;		
						BUFF_SIZE=(uint16_t)(rxbuff[0] << 8) | (uint16_t)(rxbuff[1]);	
}
else
	{
				switch(rxbuff[0])
				{				
				case 4://Ralative Moving for all joint ,Package include 30 byte
				 if((uint8_t)(255-4)==rxbuff[1])// check again byte[1] ==255-4
					{
					Flag_package_Relative_Moving=true;	
					}
					BUFF_SIZE=28;						
					break;
				case 5: // relative moving for each joint 11 byte
					if(	(uint8_t)(255-5)==rxbuff[1])// check again 
           {
					Flag_package_Relative_joint=true;
					 }
					BUFF_SIZE=9;						 
					break;				 
				case 7: // write pos 32 byte 
					if(	(uint8_t)(255-7)==rxbuff[1])
							{
					    Flag_package_Write_pos=true;	
							}
					BUFF_SIZE=26;	
					break;
				case 10:// User program
						if(	(uint8_t)(255-10)==rxbuff[1])
						{										
							Flag_package_User_program_header=true;
						}
							BUFF_SIZE=2;
					break;
				case 24:// Move Direct to Pos A Ex: MOVE A  500000 // 8 byte
						if(	(uint8_t)(255-24)==rxbuff[1])
						{										
							Flag_package_Moves_direct=true;
						}
							BUFF_SIZE=6;
					break;												
				case 25://MOVES THROUGH A TO B // 10 BYTE
						if(	(uint8_t)(255-25)==rxbuff[1])
						{										
							Flag_package_Moves_through=true;
						}
							BUFF_SIZE=8;
					break;
				case 29: // write value of QEI 26 byte
					if(	(uint8_t)(255-29)==rxbuff[1])// check again 
           {
					  Flag_package_Write_QEI=true;
					 }
					BUFF_SIZE=24;						 
					break;
				case 30: // Copy value of Possition A save into B
					if(	(uint8_t)(255-30)==rxbuff[1])// check again 
           {
					Flag_package_COPY=true;
					 }
					BUFF_SIZE=4;						 
					break;
				case 32: // Clear Flag of Servo in the case of Error ,Servo_Flag unable reset therefore need manual reset
					if(	(uint8_t)(255-32)==rxbuff[1])// check again 
           {
					Flag_package_Clear_Servo_Flag=true;
					 }
					BUFF_SIZE=2;						 
					break;		
					 
				case 35: // acceleration 
					if(	(uint8_t)(255-35)==rxbuff[1])// check again 
           {
					Flag_package_acceleration_mcu=true;
					 }
					BUFF_SIZE=12;						 
					break;
				case 40:// function send position					
		   	if(	(uint8_t)(255-40)==rxbuff[1])//
							{
									//	UARTprint(USARTx,"Send position  **** ");		
										UART_Send_Possition(USARTx,Get_QEI(1),Get_QEI(2),Get_QEI(3),Get_QEI(4),Get_QEI(5),Get_QEI(6));								
							}						
						break;
				case 41:					
					if(	(uint8_t)(255-41)==rxbuff[1])// check again 
           {
					Flag_package_Write_Angle=true;
					 }
					BUFF_SIZE=24;						 
					break;
				case 42:					
					if(	(uint8_t)(255-42)==rxbuff[1])// check again 
           {
					Flag_package_Move_Invert=true;
					 }
					BUFF_SIZE=12;						 
					break;
				case 3: // save 1 point 
				if(	(uint8_t)(255-3)==rxbuff[1])// check again byte[1] ==255-3
					{
							if(system_GetState()==detect_possition)
								{
									uint16_t i;
								 if(pointer_openloop<buffer_point)
								{
									// Save Possition of QEI into Servo_Pluse_buffer
									for( i=0;i<6;i++)
													{
														Servo_Pluse_buffer[pointer_openloop][i]=Get_QEI(i+1);						
													}	
													Print(USARTx,"Save into Point : ");	
													UARTPutnlong(USARTx,pointer_openloop);
													pointer_openloop++;
													Print(USARTx,"Pointer = ");
													UARTPutnlong(USARTx,pointer_openloop);
													USART_SendData(USARTx,(uint16_t)10); 
													while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);	
								}
									else
									{
											UARTprint(USARTx,"Buffer Fulled ****");	
									}
								}
								else
								{
									UARTprint(USARTx,"Please Transfer to Mode Dectect Possition***");
								}
					}							
					break;
					
				case 12://detect possition mode
					if(	(uint8_t)(255-12)==rxbuff[1])// check again byte[1] ==255-10
							{
									if(system_GetState()!=detect_possition)
									{			
												UARTprint(USARTx,"Change To detect_possition Mode **** ");
												system_SetState(detect_possition);													
									}
									else
									{
										UARTprint(USARTx,"Currently , Robot in mode: detect_possition **** ");
									}
							}						
						break;
					case 31://RESET pointer of User program
					if(	(uint8_t)(255-31)==rxbuff[1])//
							{
	
										// Reset User Pointer 	
										user_pointer=startAddress_User_Program;
										UARTprint(USARTx,"Reset User pointer  **** ");								
							}						
						break;
					case 14:// openloop mode 
						if(	(uint8_t)(255-14)==rxbuff[1])// check again byte[1] ==255-10
						{
						if(system_GetState()!=open_loop)
						{
							if ((Servo_Flag[0] | Servo_Flag[1] | Servo_Flag[2]| Servo_Flag[3] | Servo_Flag[4] | Servo_Flag[5])== false)	
								{			
									UARTprint(USARTx,"Change To Open loop Mode **** ");
									system_SetState(open_loop);	
								}
								else
								{
									UARTprint(USARTx,"Please Wait for Robot Stop Or Stop Robot ***");	
								}	
						}
						else
						{
							UARTprint(USARTx,"Currently , Robot in mode: Open loop **** ");
						}
					  }						
						break;
				  case 15: 
						if(	(uint8_t)(255-15)==rxbuff[1])// check again byte[1] ==255-10
						{
						if(system_GetState()!=inverse_kinematics)
						{
						UARTprint(USARTx,"Change To inverse_kinematics Mode **** ");
						system_SetState(inverse_kinematics);	
						}
						else
						{
							UARTprint(USARTx,"Currently , Robot in mode:inverse_kinematics **** ");
						}
					  }						
						break;									
					case 16:// Display value of possition
								if(	(uint8_t)(255-16)==rxbuff[1])// check again byte[1] ==255-10
									{						
								   	//Display_possition();
										Make_DK_Current_QEI();
									}							
						break;						
					case 17 :// save all data into flash
					if(	(uint8_t)(255-17)==rxbuff[1])// check again byte[1] ==255-10			
					{
						if(system_GetState() == detect_possition)
						{	
						    	Save_Pos();
//								if ((Servo_Flag[0] | Servo_Flag[1] | Servo_Flag[2]| Servo_Flag[3] | Servo_Flag[4] | Servo_Flag[5])== false)	
//								{			
//										Save_Pos();
//								}
//								else
//								{
//									UARTprint(USARTx,"Please Wait for Robot Stop Or Stop Robot ***");	
//								}							

						}
						else
						{
							UARTprint(USARTx,"Please Transfer to Mode Dectect Possition***");		
						}
					}	
					break;
					case 19 :// Home
					if(	(uint8_t)(255-19)==rxbuff[1])// check again byte[1] ==255-10
					{
								if ((Servo_Flag[0] | Servo_Flag[1] | Servo_Flag[2]| Servo_Flag[3] | Servo_Flag[4] | Servo_Flag[5])== false)	
								{			
									UARTprint(USARTx," Transfer state Home");	
									system_SetState(home);
								}
								else
								{
									UARTprint(USARTx,"Please Wait for Robot Stop Or Stop Robot ***");	
								}							
					}						
						break;
					
					case 20://Shutdown robot 
					if(	(uint8_t)(255-20)==rxbuff[1])// check again byte[1] ==255-10
					{
						// Disable all Servo 
						GPIO_WriteBit(Servo_OUT_Enable_Port_1,Servo_OUT_Enable_Pin_1,1);
						GPIO_WriteBit(Servo_OUT_Enable_Port_2,Servo_OUT_Enable_Pin_2,1);
						GPIO_WriteBit(Servo_OUT_Enable_Port_3,Servo_OUT_Enable_Pin_3,1);
						GPIO_WriteBit(Servo_OUT_Enable_Port_4,Servo_OUT_Enable_Pin_4,1);
						GPIO_WriteBit(Servo_OUT_Enable_Port_5,Servo_OUT_Enable_Pin_5,1);
						GPIO_WriteBit(Servo_OUT_Enable_Port_6,Servo_OUT_Enable_Pin_6,1);
						UARTprint(USARTx,"Shutdown ****");
						Save_Pos();
						system_SetState(waitting);			
					}	
		     break;					
				 case 18://clear QEI
					if(	(uint8_t)(255-18)==rxbuff[1])// check again byte[1] ==255-10
					{	
							if ((Servo_Flag[0] | Servo_Flag[1] | Servo_Flag[2]| Servo_Flag[3] | Servo_Flag[4] | Servo_Flag[5])== false)	
								{			
									UARTprint(USARTx,"Clear BUFFER QEI ****");	
									Reset_QEI(0);	
								}
								else
								{
									UARTprint(USARTx,"Please Wait for Robot Stop Or Stop Robot ***");	
								}							
					}						
						break;						
						case 8:// Increase pointer_openloop 1 index
					if(	(uint8_t)(255-8)==rxbuff[1])// check again byte[1] ==255-10
					{					
						if(pointer_openloop<(buffer_point-1))
						{
						pointer_openloop++;
						Print(USARTx,"Pointer = ");
						UARTPutnlong(USARTx,pointer_openloop);
						USART_SendData(USARTx,(uint16_t)10); 
						while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);	
						}
						else
							{
								Print(USARTx,"Buffer = ");
								UARTPutnlong(USARTx,buffer_point);
						    USART_SendData(USARTx,(uint16_t)10); 
							}
						}
						break;
					case 9:// Decrease pointer_openloop 1 index
					if(	(uint8_t)(255-9)==rxbuff[1])// check again byte[1] ==255-9
					{							
						if(pointer_openloop>0)
						{
						pointer_openloop--;
						Print(USARTx,"Pointer = ");
						UARTPutnlong(USARTx,pointer_openloop);
						USART_SendData(USARTx,(uint16_t)10); 
						while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);	
						}
						else
						{
							 UARTprint(USARTx,"Pointer buffer >0 ***");
						}
					}
						break;
					case 21://Load data from flash save into Ram 
					if(	(uint8_t)(255-21)==rxbuff[1])
					{							
						Load_possition();
					}
					break;
					
					case 22://Load data from flash save into Ram 
					if(	(uint8_t)(255-22)==rxbuff[1])
					{							
						QEI_Display=true;
					}
					break;
					
					case 23://Load data from flash save into Ram 
					if(	(uint8_t)(255-23)==rxbuff[1])
					{							
						QEI_Display=false;
					}					
						break;	
					case 26://Robot Pause
					if(	(uint8_t)(255-26)==rxbuff[1])
					{
						uint8_t i;
						for(i=0;i<6;i++)
						{
							if( ServoPulse[i] != 0)// if generator pulled all pulse then(in the case decelerate ) then Flag_Servo_Temp =false 
								//unless generator unable pull pulse for next step  
							{
							Flag_Servo_Temp[i]=Servo_Flag[i];
							}
							else
							{
								Flag_Servo_Temp[i]=false;								
							}
						}            						
							Flag_Robot_Run=false;					
					   	UARTprint(USARTx,"Robot Pause ");
					}
					break;
					
					case 28://Robot Run
					if(	(uint8_t)(255-28)==rxbuff[1])
					{	

								uint8_t i;
								for(i=0;i<6;i++)
								{
								Servo_Flag[i]=	Flag_Servo_Temp[i];
								}						
							Flag_Robot_Run=true;
					  	UARTprint(USARTx,"Robot Run  ");
					}					
					break;	
					case 33://Display Servo_Flag 
			    if(	(uint8_t)(255-33)==rxbuff[1])
					{	
				  	uint8_t i;						
						Print(USARTx,"* Servo Flag :  ");
								for(i=0;i<6;i++)
								{
									UARTPutnlong(USARTx,(uint32_t)Servo_Flag[i]);	
									USART_SendData(USARTx,(uint16_t)' '); 
									while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);												
								}	
									USART_SendData(USARTx,(uint16_t)10); 
									while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);									
					}	
						
					break;						
					
				 case 2://restart
					if(	(uint8_t)(255-2)==rxbuff[1])
					{		
					  system_SetState(read_enable);
					}
						break;
					
					case 1:
					if(	(uint8_t)(255-1)==rxbuff[1])
					{	
					  GPIO_SetBits(Role_Port,Role_Pin);
						GPIO_SetBits(Clear_Error_Port,Clear_Error_Pin);
					}
						break;
					
					case 0:
					if(	(uint8_t)(255-0)==rxbuff[1])
					{	
					  GPIO_ResetBits(Role_Port,Role_Pin);
						GPIO_ResetBits(Clear_Error_Port,Clear_Error_Pin);
					}					
					break;
					
					case 6:
					  if(	(uint8_t)(255-6)==rxbuff[1])// check again byte[1] ==255-6
						{							
							Display_Ram();
						}
						break;
						
					  default :	
						UARTprint(USARTx,"ERROR USART ");					
						BUFF_SIZE=2;	
						break;	         
	          }
					}
	//Re enable DMA
	USARTx_DMA_Stream->NDTR = (uint32_t)BUFF_SIZE;
	DMA_Cmd(USARTx_DMA_Stream, ENABLE);
}
	

 unsigned short  GetCRC(uint8_t  message[], unsigned short lenght )//CRC Modbus standard
        {
					  int i,j;					
						unsigned short CRCFull = 0xFFFF;
            short CRCLSB;

            for (i = 0; i < lenght - 2; i++)
            {
                CRCFull = (unsigned short)(CRCFull ^ message[i]);

                for ( j = 0; j < 8; j++)
                {
                    CRCLSB = (short)(CRCFull & 0x0001);
                    CRCFull = (unsigned short)((CRCFull >> 1) & 0x7FFF);

                    if (CRCLSB == 1)
                        CRCFull = (unsigned short)(CRCFull ^ 0xA001);
                }
            }
						
				return CRCFull;

       } //make CRC 
				
#define POLY 0x8408
/*
//                                      16   12   5
// this is the CCITT CRC 16 polynomial X  + X  + X  + 1.
// This works out to be 0x1021, but the way the algorithm works
// lets us use 0x8408 (the reverse of the bit pattern).  The high
// bit is always assumed to be set, thus we only use 16 bits to
// represent the 17 bit value.
*/

unsigned short crc16(int8_t *data_p, unsigned short length)
{
      unsigned char i;
      unsigned int data;
      unsigned int crc = 0xffff;

      if (length == 0)
            return (~crc);

      do
      {
            for (i=0, data=(unsigned int)0xff & *data_p++;
                 i < 8; 
                 i++, data >>= 1)
            {
                  if ((crc & 0x0001) ^ (data & 0x0001))
                        crc = (crc >> 1) ^ POLY;
                  else  crc >>= 1;
            }
      } while (--length);

      crc = ~crc;
      data = crc;
      crc = (crc << 8) | (data >> 8 & 0xff);

      return (crc);
} 

void USART_DMA (void)
{
	USART_InitTypeDef USART_InitStructure;   
  GPIO_InitTypeDef  GPIO_InitStructure; 
  DMA_InitTypeDef   DMA_InitStructure;
  NVIC_InitTypeDef  NVIC_InitStructure;
  
  /* Enable DMA1 clock */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
  /* DMA1 Stream5 Channel4 for USART2 Rx configuration */			
  DMA_InitStructure.DMA_Channel = USARTx_DMA_Channel;  
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USARTx->DR;
  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)rxbuff;// con tro du lieu 
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;// gui du lieu tu ngoai vi vo bo nho
  DMA_InitStructure.DMA_BufferSize = (uint32_t)BUFF_SIZE; 
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;// khong tang con tro cho ngoai vi 
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;// chep du lieu vao memory thi phai tang con tro 
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//cho phep chep du lieu kieu byte /SPI co the la 16bit 
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;//DMA_Mode_Circular la sau khi con tro chep du bang buffsize thi no se quay ve vi tri con tro 0 
	// normal chi cho chay 1 lan roi thoi ,phai enable lai thi DMA moi chay lai
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;// muc uu tien DMA
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;// Bao khi day 1/2 buffer
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;// ko dung 
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single; // ko dung
  DMA_Init(USARTx_DMA_Stream, &DMA_InitStructure);
  DMA_Cmd(USARTx_DMA_Stream, ENABLE);
 /* Enable DMA Interrupt to the highest priority */
 	/* 	USB OTG only operation with out Interrupt  */
  NVIC_SetPriorityGrouping(3);
  NVIC_InitStructure.NVIC_IRQChannel = USARTx_DMA_IRQ;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 7;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  /* Transfer complete interrupt mask */
  DMA_ITConfig(USARTx_DMA_Stream, DMA_IT_TC, ENABLE);
	  /* Enable GPIO clock */
  RCC_AHB1PeriphClockCmd(USARTx_TX_GPIO_CLK | USARTx_RX_GPIO_CLK, ENABLE);
  /* Enable USART clock */
  USARTx_CLK_INIT(USARTx_CLK, ENABLE);
  /* Connect USART pins to AF */
  GPIO_PinAFConfig(USARTx_TX_GPIO_PORT, USARTx_TX_SOURCE, USARTx_TX_AF);
  GPIO_PinAFConfig(USARTx_RX_GPIO_PORT, USARTx_RX_SOURCE, USARTx_RX_AF);
  
  /* Configure USART Tx and Rx as alternate function push-pull */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	
  GPIO_InitStructure.GPIO_Pin = USARTx_TX_PIN;
  GPIO_Init(USARTx_TX_GPIO_PORT, &GPIO_InitStructure); 
  GPIO_InitStructure.GPIO_Pin = USARTx_RX_PIN;
  GPIO_Init(USARTx_RX_GPIO_PORT, &GPIO_InitStructure);
	
  /* Enable the USART OverSampling by 8 */
  USART_OverSampling8Cmd(USARTx, ENABLE);           
  /* USARTx configuration */
  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;// Warning if parity=even must write 9 bit-- if setting 8 Bit--. error
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

  USART_Init(USARTx, &USART_InitStructure);
	
  /* Enable USART DMA */
  USART_DMACmd(USARTx, USART_DMAReq_Rx, ENABLE);

  /* Enable USART */
  USART_Cmd(USARTx, ENABLE); 
}

void USART_Config(void)
{
  USART_InitTypeDef USART_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure; 
	DMA_InitTypeDef  DMA_InitStructure;

  /* Peripheral Clock Enable -------------------------------------------------*/
  /* Enable GPIO clock */
  RCC_AHB1PeriphClockCmd(USARTx1_TX_GPIO_CLK | USARTx1_RX_GPIO_CLK, ENABLE);
  
  /* Enable USART clock */
  USARTx1_CLK_INIT(USARTx1_CLK, ENABLE);
  
  /* Enable the DMA clock */
  RCC_AHB1PeriphClockCmd(USARTx1_DMAx_CLK, ENABLE);
  
  /* USARTx1 GPIO configuration -----------------------------------------------*/ 
  /* Connect USART pins to AF7 */
  GPIO_PinAFConfig(USARTx1_TX_GPIO_PORT, USARTx1_TX_SOURCE, USARTx1_TX_AF);
  GPIO_PinAFConfig(USARTx1_RX_GPIO_PORT, USARTx1_RX_SOURCE, USARTx1_RX_AF);
  
  /* Configure USART Tx and Rx as alternate function push-pull */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  
  GPIO_InitStructure.GPIO_Pin = USARTx1_TX_PIN;
  GPIO_Init(USARTx1_TX_GPIO_PORT, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = USARTx1_RX_PIN;
  GPIO_Init(USARTx1_RX_GPIO_PORT, &GPIO_InitStructure);
 
  /* USARTx1 configuration ----------------------------------------------------*/
  /* Enable the USART OverSampling by 8 */
  USART_OverSampling8Cmd(USARTx1, ENABLE); 
  
  /* USARTx1 configured as follows:
        - BaudRate = 5250000 baud
		   - Maximum BaudRate that can be achieved when using the Oversampling by 8
		     is: (USART APB Clock / 8) 
			 Example: 
			    - (USART3 APB1 Clock / 8) = (42 MHz / 8) = 5250000 baud
			    - (USART1 APB2 Clock / 8) = (84 MHz / 8) = 10500000 baud
		   - Maximum BaudRate that can be achieved when using the Oversampling by 16
		     is: (USART APB Clock / 16) 
			 Example: (USART3 APB1 Clock / 16) = (42 MHz / 16) = 2625000 baud
			 Example: (USART1 APB2 Clock / 16) = (84 MHz / 16) = 5250000 baud
        - Word Length = 8 Bits
        - one Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
  */ 
  USART_InitStructure.USART_BaudRate = 5250000;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  /* When using Parity the word length must be configured to 9 bits */
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USARTx1, &USART_InitStructure);

/* Configure DMA controller to manage USART TX and RX DMA request ----------*/ 

/* Configure DMA Initialization Structure */
DMA_InitStructure.DMA_BufferSize = BUFF_SIZE_USART_1 ;
DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable ;
DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full ;
DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single ;
DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
DMA_InitStructure.DMA_PeripheralBaseAddr =(uint32_t) (&(USARTx1->DR)) ;
DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
DMA_InitStructure.DMA_Priority = DMA_Priority_High;
///* Configure TX DMA */
//DMA_InitStructure.DMA_Channel = USARTx1_TX_DMA_CHANNEL ;
//DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral ;
//DMA_InitStructure.DMA_Memory0BaseAddr =(uint32_t)aTxBuffer ;
//DMA_Init(USARTx1_TX_DMA_STREAM,&DMA_InitStructure);
  /* Configure RX DMA */
  DMA_InitStructure.DMA_Channel = USARTx1_RX_DMA_CHANNEL ;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory ;
  DMA_InitStructure.DMA_Memory0BaseAddr =(uint32_t)Rx_USART1_Buffer ; 
  DMA_Init(USARTx1_RX_DMA_STREAM,&DMA_InitStructure);
	

	/* Enable DMA Interrupt */
  NVIC_SetPriorityGrouping(3);
  NVIC_InitStructure.NVIC_IRQChannel = USARTx1_DMA_RX_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  /* Transfer complete interrupt mask */
  DMA_ITConfig(USARTx1_RX_DMA_STREAM, DMA_IT_TC, ENABLE);
	DMA_Cmd(USARTx1_RX_DMA_STREAM, ENABLE);
	  /* Enable USART DMA */
  USART_DMACmd(USARTx1, USART_DMAReq_Rx, ENABLE);     
  /* Enable USART */
  USART_Cmd(USARTx1, ENABLE);
	
}

void ConfigUART4(void)
{

  USART_InitTypeDef USART_InitStructure;   
  GPIO_InitTypeDef GPIO_InitStructure; 
   
  /* Enable GPIO clock */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
  /* Enable UART clock */;
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);

  /* Connect UART4 and USART2 pins to AF2 */  
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_UART4);
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_UART4); 
	

  /* GPIO Configuration for USART Tx */
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10 ;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  /* GPIO Configuration for USART Rx */
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_11 ;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  
       
  /* USARTx configured as follow:
        - BaudRate = 9600 baud  
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
  */
  USART_InitStructure.USART_BaudRate = 9600;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

  USART_Init(UART4, &USART_InitStructure);
  /* Enable USART */
  USART_Cmd(UART4, ENABLE); 
}

void ConfigUSART2(void)
{

  USART_InitTypeDef USART_InitStructure;   
  GPIO_InitTypeDef GPIO_InitStructure; 
   
  /* Enable GPIO clock */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
  /* Enable UART clock */;
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

  /* Connect UART4 and USART2 pins to AF2 */  
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_USART2);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource6, GPIO_AF_USART2); 
	

  /* GPIO Configuration for USART Tx */
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_5 ;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

  /* GPIO Configuration for USART Rx */
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6 ;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  
       
  /* USARTx configured as follow:
        - BaudRate = 9600 baud  
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
  */
  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;


  USART_Init(USART2, &USART_InitStructure);

  /* Enable USART */
  USART_Cmd(USART2, ENABLE);
  
}




void UARTPut(USART_TypeDef* UART_Base, int32_t num2send)
{

	int i;
	int32_t temp=num2send;
	for(i=4;i>0;i--)
	{
		USART_SendData(UART_Base, (char)((temp&0xff000000)>>24));
		temp=temp<<8;
	}

}
//void UARTprint(USART_TypeDef* UART_Base, char *send)
//{

//	int i;
//	USART_SendData(USARTx,(uint16_t)10); 
//	while (USART_GetFlagStatus(UART_Base, USART_FLAG_TXE) == RESET);	
//	for(i=0;i< strlen (send);i++)
//	{
//		USART_SendData(UART_Base, send[i]);
//		while (USART_GetFlagStatus(UART_Base, USART_FLAG_TXE) == RESET);
//	}
//	 USART_SendData(UART_Base,(uint16_t)10); 
//	 while (USART_GetFlagStatus(UART_Base, USART_FLAG_TXE) == RESET);
//}

void UARTprint(USART_TypeDef* UART_Base,char* send)
{
	int i;
	USART_SendData(USARTx,(uint16_t)10); 
	while (USART_GetFlagStatus(UART_Base, USART_FLAG_TXE) == RESET);	
	for(i=0;i< strlen (send);i++)
	{
		USART_SendData(UART_Base, (uint16_t)send[i]);
		while (USART_GetFlagStatus(UART_Base, USART_FLAG_TXE) == RESET);
	}
	 USART_SendData(UART_Base,(uint16_t)10); 
	 while (USART_GetFlagStatus(UART_Base, USART_FLAG_TXE) == RESET);
}
void Print(USART_TypeDef* UART_Base, char *send)
{
	int i;
	USART_SendData(USARTx,(uint16_t)10); 
	while (USART_GetFlagStatus(UART_Base, USART_FLAG_TXE) == RESET);	
	for(i=0;i< strlen (send);i++)
	{
		USART_SendData(UART_Base, send[i]);
		while (USART_GetFlagStatus(UART_Base, USART_FLAG_TXE) == RESET);
	}
		USART_SendData(USARTx,(uint16_t)' ');
		while (USART_GetFlagStatus(UART_Base, USART_FLAG_TXE) == RESET);
}
void UART_Send_Possition(USART_TypeDef* UART_Base, int32_t j1,int32_t j2,int32_t j3,int32_t j4, int32_t j5, int32_t j6)
{
	int i,j=0;
	int temp;
	int8_t send[52];
	send[j++]=40;//4 Byte for checking
	send[j++]=215;
	send[j++]=40;
	send[j++]=215;
	
	temp=Get_QEI(1);
	send[j++]=(int8_t)(temp>>24);
	send[j++]=(int8_t)(temp>>16);
	send[j++]=(int8_t)(temp>>8);
	send[j++]=(int8_t) temp;
	temp=Get_QEI(2);
	send[j++]=(int8_t)(temp>>24);
	send[j++]=(int8_t)(temp>>16);
	send[j++]=(int8_t)(temp>>8);
	send[j++]=(int8_t)temp;
	temp=Get_QEI(3);
	send[j++]=(int8_t)(temp>>24);
	send[j++]=(int8_t)(temp>>16);
	send[j++]=(int8_t)(temp>>8);
	send[j++]=(int8_t)temp;
	temp=Get_QEI(4);
	send[j++]=(int8_t)(temp>>24);
	send[j++]=(int8_t)(temp>>16);
	send[j++]=(int8_t)(temp>>8);
	send[j++]=(int8_t)temp;
	temp=Get_QEI(5);
	send[j++]=(int8_t)(temp>>24);
	send[j++]=(int8_t)(temp>>16);
	send[j++]=(int8_t)(temp>>8);
	send[j++]=(int8_t)temp;
	temp=Get_QEI(6);
	send[j++]=(int8_t)(temp>>24);
	send[j++]=(int8_t)(temp>>16);
	send[j++]=(int8_t)(temp>>8);
	send[j++]=(int8_t)temp;
// send Speed 	
	temp=Get_Qei_Speed(1);
	send[j++]=(int8_t)(temp>>24);
	send[j++]=(int8_t)(temp>>16);
	send[j++]=(int8_t)(temp>>8);
	send[j++]=(int8_t)temp;	
	temp=Get_Qei_Speed(2);
	send[j++]=(int8_t)(temp>>24);
	send[j++]=(int8_t)(temp>>16);
	send[j++]=(int8_t)(temp>>8);
	send[j++]=(int8_t)temp;	
	temp=Get_Qei_Speed(3);
	send[j++]=(int8_t)(temp>>24);
	send[j++]=(int8_t)(temp>>16);
	send[j++]=(int8_t)(temp>>8);
	send[j++]=(int8_t)temp;	
	temp=Get_Qei_Speed(4);
	send[j++]=(int8_t)(temp>>24);
	send[j++]=(int8_t)(temp>>16);
	send[j++]=(int8_t)(temp>>8);
	send[j++]=(int8_t)temp;
	temp=Get_Qei_Speed(5);
	send[j++]=(int8_t)(temp>>24);
	send[j++]=(int8_t)(temp>>16);
	send[j++]=(int8_t)(temp>>8);
	send[j++]=(int8_t)temp;
	temp=Get_Qei_Speed(6);
	send[j++]=(int8_t)(temp>>24);
	send[j++]=(int8_t)(temp>>16);
	send[j++]=(int8_t)(temp>>8);
	send[j++]=(int8_t)temp;	
	for(i=0;i< 52;i++)
	{
		USART_SendData(UART_Base,send[i]);
		while (USART_GetFlagStatus(UART_Base, USART_FLAG_TXE) == RESET);
	}
}
void UARTPutnlong(USART_TypeDef*  UART_Base, long Num)
{
	unsigned long temp = 1;
	long NumTemp;
	NumTemp = Num;
	if (Num == 0)
	{
		USART_SendData(UART_Base, 48);
		while (USART_GetFlagStatus(UART_Base, USART_FLAG_TXE) == RESET);
	}
	else
	{
		if (Num < 0)
		{
			USART_SendData(UART_Base, '-');
      while (USART_GetFlagStatus(UART_Base, USART_FLAG_TXE) == RESET);
			Num *= -1;
		}
		while (NumTemp)
		{
			NumTemp /= 10;
			temp *= 10;
		}
		temp /= 10;
		while (temp)
		{
			USART_SendData(UART_Base,(Num / temp) % 10 + 48);
			while (USART_GetFlagStatus(UART_Base, USART_FLAG_TXE) == RESET);
			temp /= 10;
		}
	}
		//		USART_SendData(UART_Base,(uint16_t)10); 
    //  while (USART_GetFlagStatus(UART_Base, USART_FLAG_TXE) == RESET);
}




void UARTFloatx(USART_TypeDef* UART_Base ,float fnum2send)
{
	
	long NumTemp = fnum2send * 1000000;
	long Num = NumTemp;
	unsigned long temp = 1;
	if (Num == 0)
	{
		USART_SendData( UART_Base,(uint16_t) 48 );
	
	}
	else
	{
		if (Num < 0)
		{
			USART_SendData( UART_Base, (uint16_t)'-');

			Num *= -1;
		}
		while (NumTemp)
		{
			NumTemp /= 10;
			temp *= 10;
		}
		temp /= 10;
		while (temp)
		{
			USART_SendData( UART_Base, (uint16_t)((Num / temp) % 10 + 48) );
			while (USART_GetFlagStatus(UART_Base, USART_FLAG_TXE) == RESET);	
			temp /= 10;
		}
	}
}


void UART_Print_Matrix(USART_TypeDef* UART_Base,real_T a[4][4])
{
	int i, j;	
	USART_SendData(UART_Base,(uint16_t)10); 
	for ( i = 0; i < 4 ; ++i )
	{
		for ( j = 0; j < 4 ; ++j )
		{
			UART_Print_Double( UART_Base, a[i][j]);
			USART_SendData( UART_Base, (uint16_t)' ' );
			while (USART_GetFlagStatus(UART_Base, USART_FLAG_TXE) == RESET);	
		}
		USART_SendData(UART_Base,(uint16_t)10); 
		while (USART_GetFlagStatus(UART_Base, USART_FLAG_TXE) == RESET);
	}
//			USART_SendData(UART_Base,(uint16_t)10); 
//			while (USART_GetFlagStatus(UART_Base, USART_FLAG_TXE) == RESET);

}

void UART_Print_Double(USART_TypeDef* UART_Base ,double fnum2send)
{
		double temp;
	uint16_t i,j=0,k=0;
	
	if(fnum2send<0)
	{
		fnum2send=-fnum2send;
		USART_SendData( UART_Base, (uint16_t)'-');
		while (USART_GetFlagStatus(UART_Base, USART_FLAG_TXE) == RESET);	
	}
	else
	{
		USART_SendData( UART_Base, (uint16_t)' ');
		while (USART_GetFlagStatus(UART_Base, USART_FLAG_TXE) == RESET);	
	}
	temp=fnum2send;
	
	while(temp >= 1.0)
	{
				 j++;
				 temp=temp/10;
	}
	temp=fnum2send;
	if(j>=1)
		{
				for( i=j;i>0;i--)
				{
					k=(uint16_t)(temp/pow(10.0,i-1));		
					USART_SendData( UART_Base, (uint16_t)(k+ 48));
					while (USART_GetFlagStatus(UART_Base, USART_FLAG_TXE) == RESET);	
					temp=temp -(double)(k*pow(10.0,i-1));
				}
		}
	else
		{
			USART_SendData( UART_Base, (uint16_t)'0');
			while (USART_GetFlagStatus(UART_Base, USART_FLAG_TXE) == RESET);	
		}		
		
		USART_SendData( UART_Base, (uint16_t)'.');
		while (USART_GetFlagStatus(UART_Base, USART_FLAG_TXE) == RESET);	
// print 5 number 	
	for(i=0;i<5;i++)
	{
		temp=temp*10;
		k=(uint16_t)(temp);
		USART_SendData( UART_Base, (uint16_t)(k+ 48)) ;
		while (USART_GetFlagStatus(UART_Base, USART_FLAG_TXE) == RESET);	
		temp=temp -(double)k;
	}	
}

void RCC_Configuration(void)
{
  /* --------------------------- System Clocks Configuration -----------------*/
  /* UART4 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
 
  /* GPIOC clock enable */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
 
  /* DMA1 clock enable */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
}
 
/**************************************************************************************/
 
void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
 
  /*-------------------------- GPIO Configuration ----------------------------*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11; // PC.10 UART4_TX, potential clash SCLK CS43L22
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
 
  /* Connect USART pins to AF */
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_UART4);
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_UART4);
}
 
/**************************************************************************************/
 
void UART4_Configuration(void)
{
  USART_InitTypeDef USART_InitStructure;
 
  /* USARTx configuration ------------------------------------------------------*/
  /* USARTx configured as follow:
        - BaudRate = 115200 baud
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
  */
  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
 
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
 
  USART_Init(UART4, &USART_InitStructure);
 
  USART_Cmd(UART4, ENABLE);
}
 
/**************************************************************************************/

 
void DMA_Configuration(void)
{
  DMA_InitTypeDef  DMA_InitStructure;
 
  DMA_DeInit(DMA1_Stream4);
 
  DMA_InitStructure.DMA_Channel = DMA_Channel_4;
  DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral; // Transmit
  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)aTxBuffer;
  DMA_InitStructure.DMA_BufferSize = (uint16_t)sizeof(aTxBuffer) ;
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&UART4->DR;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Enable;
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
 
  DMA_Init(DMA1_Stream4, &DMA_InitStructure);
  /* Enable DMA Stream Transfer Complete interrupt */
//  DMA_ITConfig(DMA1_Stream4, DMA_IT_TC, ENABLE); 
 
//  /* Enable the USART Tx DMA request */
//  USART_DMACmd(UART4, USART_DMAReq_Tx, ENABLE);
 
//  /* Enable the DMA Tx Stream */
//  DMA_Cmd(DMA1_Stream4, ENABLE);
}
 
/**************************************************************************************/
 
//void DMA1_Stream4_IRQHandler(void)
//{
//  /* Test on DMA Stream Transfer Complete interrupt */
//  if (DMA_GetITStatus(DMA1_Stream4, DMA_IT_TCIF4))
//  {
//    /* Clear DMA Stream Transfer Complete interrupt pending bit */
//    //DMA_ClearITPendingBit(DMA1_Stream4, DMA_IT_TCIF4);
//  }
//}
 
/**************************************************************************************/
 
void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
 
  /* Configure the Priority Group to 2 bits */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
 
  /* Enable the UART4 RX DMA Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream4_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}
/**************************************************************************************/
 



void USART_Config1(void)
{
	  RCC_Configuration();
 
  NVIC_Configuration();
 
  GPIO_Configuration();
 
  UART4_Configuration();
 
  DMA_Configuration();
 
}





