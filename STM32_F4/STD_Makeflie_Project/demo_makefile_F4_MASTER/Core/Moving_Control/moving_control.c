#include "include.h"			
 

 void Move_Through(uint16_t A,uint16_t B,uint32_t Speed)//	Move_Through(2,7,50);	
 { 
	 uint16_t pointer_move_through=A;
// Move From current Possition to A
	 Move_Current_Point(A,Speed);	
 // Waiting Robot Moving	 
	 waitting_moving();
	 Print(USARTx,"Moving Done Pos :  "); 
	 UARTPutnlong(USARTx,A);
	 USART_SendData(USARTx,(uint16_t)10);
	 while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
	 do
		{
			if(B>A)
				pointer_move_through++;
			else
				pointer_move_through--;			 
	       		Move_Current_Point(pointer_move_through,Speed);
				 // Waiting Robot Moving
				waitting_moving();
				Print(USARTx,"Moving Done Pos :  "); 
				UARTPutnlong(USARTx,pointer_move_through);
				USART_SendData(USARTx,(uint16_t)10);
				while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
		}
		while(pointer_move_through != B);
 }
 
 
 void Move_Current_Point(uint16_t Posx,uint32_t Speed)
 {
	 if(Flag_Slave_Busy==true)
	 {
		 UARTprint(USARTx,"Slave Busy");  
	 }
	 else
	 {
	 uint16_t i;int Delta_Q[6];
	 for(i=0;i<6;i++)// Keil C warning if directly tranfer Servo_Pluse_buffer[Posx][i];
	 {
		 Delta_Q[i]=0;//Reset value 
		 if(true==servo_Enable_Flag[i])
		 {
			 Delta_Q[i]=Servo_Pluse_buffer[Posx][i]-Get_QEI(i+1);
		 }
	 } 
	 Print(USARTx,"Start Move to Possition : "); 
	 UARTPutnlong(USARTx,Posx);
	 USART_SendData(USARTx,(uint16_t)10);
	 while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
	 // Move Robot from current position to Posx
	 Planing_Path_Free(Delta_Q,Speed);
 }
}
 void waitting_moving(void)
 {
	 while(Flag_Slave_Busy==true)
	{
		Display_QEI();						
	} 
 }
	