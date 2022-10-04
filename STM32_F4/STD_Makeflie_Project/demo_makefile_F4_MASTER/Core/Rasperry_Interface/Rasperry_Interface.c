/*
 * Rasperry_Interface.c
 *
 *  Created on: Nov 9, 2016
 *      Author: AnhTuan
 */

#include "include.h"
static uint8_t TX_Rasp_Buffer[30] ;

bool Rasp_Feedback_check(uint8_t id )
{
	int i;
while(TM_USB_VCP_BufferFull()==0)
  {
		UARTprint(USARTx,"waiting");	
		// delay_ms(1);
	} //buffer emty
	
		if(TM_INT_USB_VCP_ReceiveBuffer[0] == id)
		{
			UARTprint(USARTx,"ok");
			DiscardIn_Out_Buffer();
			return true;
		}
		else
		{	
				for(i=0;i<USB_VCP_RECEIVE_BUFFER_LENGTH;i++)
				{
				 UARTPut(USARTx,(uint8_t)TM_INT_USB_VCP_ReceiveBuffer[i]);
				 while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);			
				}
				UARTprint(USARTx,"error");	
				while(1);
			System_Error("Rasppery feedback error");//While loop 
			system_SetState(error);
			return false;
		}
}
void Rasp_Move_invert_delta(double x,double y,double z )
{
	char Arr_delta_invert[sizeof(double)]; // array for convert from double to array and tranmist to Slave
	int i,pointer;
	if((x==0) &(y==0)&(z==0))
	{
		UARTprint(USARTx," Delta =0 , Finish procees ");
		 return;
	}
	TX_Rasp_Buffer[0]=(uint8_t)100 ;// ID Function Check Raspberry idel 
	DiscardIn_Out_Buffer();
	TM_USB_VCP_Send(TX_Rasp_Buffer,30);
	/* wait respone and check id package */
  Rasp_Feedback_check(TX_Rasp_Buffer[0]);
	UARTprint(USARTx,"Raspberry Ready ");
	//Send QEI value 	
	TX_Rasp_Buffer[0]=1 ;// ID Function QEI
	for(i=0;i<6;i++)
	{
		TX_Rasp_Buffer[2+i*4]=(uint8_t)(Get_QEI(i+1)>>24);
		TX_Rasp_Buffer[3+i*4]=(uint8_t)(Get_QEI(i+1)>>16);
		TX_Rasp_Buffer[4+i*4]=(uint8_t)(Get_QEI(i+1)>>8);
		TX_Rasp_Buffer[5+i*4]=(uint8_t)(Get_QEI(i+1));
	}
	DiscardIn_Out_Buffer();	
	TM_USB_VCP_Send(TX_Rasp_Buffer,30);
  /* wait respone and check id package */
  Rasp_Feedback_check(TX_Rasp_Buffer[0]);
	UARTprint(USARTx,"Send QEI finish ");		
   // Send Delta x y z
	pointer=0;
	TX_Rasp_Buffer[pointer++]=(uint8_t)4;  // Id send invert delta 	
	//x
	memcpy(&Arr_delta_invert,&x,sizeof(x));
	TX_Rasp_Buffer[pointer++]=Arr_delta_invert[0];
	TX_Rasp_Buffer[pointer++]=Arr_delta_invert[1];
	TX_Rasp_Buffer[pointer++]=Arr_delta_invert[2];
	TX_Rasp_Buffer[pointer++]=Arr_delta_invert[3];
	TX_Rasp_Buffer[pointer++]=Arr_delta_invert[4];
	TX_Rasp_Buffer[pointer++]=Arr_delta_invert[5];
	TX_Rasp_Buffer[pointer++]=Arr_delta_invert[6];
	TX_Rasp_Buffer[pointer++]=Arr_delta_invert[7];
	//y
	memcpy(&Arr_delta_invert,&y,sizeof(y));
	TX_Rasp_Buffer[pointer++]=Arr_delta_invert[0];
	TX_Rasp_Buffer[pointer++]=Arr_delta_invert[1];
	TX_Rasp_Buffer[pointer++]=Arr_delta_invert[2];
	TX_Rasp_Buffer[pointer++]=Arr_delta_invert[3];
	TX_Rasp_Buffer[pointer++]=Arr_delta_invert[4];
	TX_Rasp_Buffer[pointer++]=Arr_delta_invert[5];
	TX_Rasp_Buffer[pointer++]=Arr_delta_invert[6];
	TX_Rasp_Buffer[pointer++]=Arr_delta_invert[7];
  //z
	memcpy(&Arr_delta_invert,&z,sizeof(z));
	TX_Rasp_Buffer[pointer++]=Arr_delta_invert[0];
	TX_Rasp_Buffer[pointer++]=Arr_delta_invert[1];
	TX_Rasp_Buffer[pointer++]=Arr_delta_invert[2];
	TX_Rasp_Buffer[pointer++]=Arr_delta_invert[3];
	TX_Rasp_Buffer[pointer++]=Arr_delta_invert[4];
	TX_Rasp_Buffer[pointer++]=Arr_delta_invert[5];
	TX_Rasp_Buffer[pointer++]=Arr_delta_invert[6];
	TX_Rasp_Buffer[pointer++]=Arr_delta_invert[7];
	
	DiscardIn_Out_Buffer();	
	TM_USB_VCP_Send(TX_Rasp_Buffer,30);
  /* wait respone and check id package */
  Rasp_Feedback_check(TX_Rasp_Buffer[0]);
	UARTprint(USARTx,"Send delta finish ");
	



}

void Rasp_Move_throught(int start_point,int end_point,int speed )
{
	/* Warning USART Interrupt priority have to lower USB OTG .Unless USB OTG can't operation send data 
	In file USB_BSP.c
	NVIC_InitStructure.NVIC_IRQChannel = OTG_FS_IRQn;  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = USB_VCP_NVIC_PRIORITY;
	*/
//	TX_Rasp_Buffer[0]=(uint8_t)48; 
//	TX_Rasp_Buffer[1]=(uint8_t)49; 
//	TX_Rasp_Buffer[2]=(uint8_t)50; 
//	TX_Rasp_Buffer[3]=(uint8_t)51; 

//	TX_Rasp_Buffer[4]=(uint8_t)52; 
//	TX_Rasp_Buffer[5]=(uint8_t)53; 
//	TX_Rasp_Buffer[6]=(uint8_t)54; 
//	TX_Rasp_Buffer[7]=(uint8_t)55; 	
	int i,j;
	if(start_point==end_point)
	{
		//just move to start point after that finish processing
		 return;
	}
	TX_Rasp_Buffer[0]=(uint8_t)100 ;// ID Function Check Raspberry idel 
	DiscardIn_Out_Buffer();
	TM_USB_VCP_Send(TX_Rasp_Buffer,30);
	/* wait respone and check id package */
  Rasp_Feedback_check(TX_Rasp_Buffer[0]);
	UARTprint(USARTx,"Raspberry Ready ");
	
	TX_Rasp_Buffer[0]=0 ;// ID Function Move through parameter
	TX_Rasp_Buffer[1]=(uint8_t)start_point; 
	TX_Rasp_Buffer[2]=(uint8_t)end_point; 	
	TX_Rasp_Buffer[3] = (uint8_t)(speed >> 24);  //Speed
	TX_Rasp_Buffer[4] = (uint8_t)(speed >> 16);
	TX_Rasp_Buffer[5] = (uint8_t)(speed >> 8);
	TX_Rasp_Buffer[6] = (uint8_t)(speed );
	DiscardIn_Out_Buffer();	
	TM_USB_VCP_Send(TX_Rasp_Buffer,30);
			//return;
  /* wait respone and check id package */
  Rasp_Feedback_check(0);
	UARTprint(USARTx,"Move Through send data start ");
	
	//Send QEI value 	
	TX_Rasp_Buffer[0]=1 ;// ID Function QEI
	for(i=0;i<6;i++)
	{
		TX_Rasp_Buffer[2+i*4]=(uint8_t)(Get_QEI(i+1)>>24);
		TX_Rasp_Buffer[3+i*4]=(uint8_t)(Get_QEI(i+1)>>16);
		TX_Rasp_Buffer[4+i*4]=(uint8_t)(Get_QEI(i+1)>>8);
		TX_Rasp_Buffer[5+i*4]=(uint8_t)(Get_QEI(i+1));
	}
	DiscardIn_Out_Buffer();	
	TM_USB_VCP_Send(TX_Rasp_Buffer,30);
  /* wait respone and check id package */
  Rasp_Feedback_check(TX_Rasp_Buffer[0]);
	UARTprint(USARTx,"Send QEI finish ");
	
	
	// Send Servo_Pluse_buffer
	TX_Rasp_Buffer[0]=2 ;// ID Function Servo_Pluse_buffer
	if(start_point<end_point)
	{
		for(i =start_point;i<=end_point;i++)
		{
			TX_Rasp_Buffer[1]=(uint8_t)i;
			for( j=0;j<6;j++)
			{
				TX_Rasp_Buffer[2+j*4]=(uint8_t)(Servo_Pluse_buffer[i][j]>>24);
				TX_Rasp_Buffer[3+j*4]=(uint8_t)(Servo_Pluse_buffer[i][j]>>16);
				TX_Rasp_Buffer[4+j*4]=(uint8_t)(Servo_Pluse_buffer[i][j]>>8);
				TX_Rasp_Buffer[5+j*4]=(uint8_t)(Servo_Pluse_buffer[i][j]);
			}
				DiscardIn_Out_Buffer();	
	      TM_USB_VCP_Send(TX_Rasp_Buffer,30);
			/* wait respone and check id package */
      Rasp_Feedback_check(TX_Rasp_Buffer[0]);	
    	UARTprint(USARTx,"pass 0 ");			
		}
	}
	else if (start_point >end_point)
	{
	 for(i =end_point;i<=start_point;i++)
		{
			TX_Rasp_Buffer[1]=(uint8_t)i;
			for( j=0;j<6;j++)
			{
				TX_Rasp_Buffer[2+j*4]=(uint8_t)(Servo_Pluse_buffer[i][j]>>24);
				TX_Rasp_Buffer[3+j*4]=(uint8_t)(Servo_Pluse_buffer[i][j]>>16);
				TX_Rasp_Buffer[4+j*4]=(uint8_t)(Servo_Pluse_buffer[i][j]>>8);
				TX_Rasp_Buffer[5+j*4]=(uint8_t)(Servo_Pluse_buffer[i][j]);
			}
			DiscardIn_Out_Buffer();	
	    TM_USB_VCP_Send(TX_Rasp_Buffer,30);
			/* wait respone and check id package */
      Rasp_Feedback_check(TX_Rasp_Buffer[0]);			
		}	
	}
// Finish transfer data and start rasperry caculate and moving 
	TX_Rasp_Buffer[0]=3 ;// ID Function 
/* wait respone and check id package */
	DiscardIn_Out_Buffer();	
	TM_USB_VCP_Send(TX_Rasp_Buffer,30);
  Rasp_Feedback_check(TX_Rasp_Buffer[0]);
	UARTprint(USARTx,"pass 1 ");	
}





