/*
 *  Flash.c
 *
 *  Created on: May 23, 2016
 *  Author: AnhTuan
 */
 
 #include "include.h"	
uint8_t Read_Byte(uint32_t address)
{
	return (*(__IO uint8_t*) (address));
}
uint16_t Read_HalfWord(uint32_t address)
{
	return (*(__IO uint16_t*) (address));
}

uint32_t Read_Word(uint32_t address)
{
	return (*(__IO uint32_t*) (address));
}

void Write_User_Program(uint8_t code[] ,uint16_t lenght) 
	{
	uint16_t i;
  int startAddress;
	FLASH_Unlock();
	/* Clear All pending flags */
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR| FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR);
	FLASH_EraseSector(FLASH_Sector_User_Program, VoltageRange_3);
/*******************************************************************/		
			
	  startAddress = startAddress_User_Program;
		for(i=0;i<lenght;i++)
		{
				FLASH_ProgramByte(startAddress, code[i]);//Function
	      startAddress += 1;		
		}
		
		// Reset User Pointer 	
		user_pointer=startAddress_User_Program;
			
/*******************************************************************/	
	FLASH_Lock();
	UARTprint(USARTx, "Write User Progarm into Flash Done ****");

}



void Save_Pos(void)// data old will be deleted
{
	int i,j,startAddress; 
	FLASH_Unlock();
	/* Clear All pending flags */
	FLASH_ClearFlag( FLASH_FLAG_EOP |  FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR);
	if(GPIO_ReadInputDataBit(SW_SL0_Port,SW_SL0_Pin))// SWITCH ON IN THE CASE OF LAB
	{
			FLASH_EraseSector(FLASH_Sector_HOME, VoltageRange_3);
			//VoltageRange_1        ((uint8_t)0x00)  /*!< Device operating range: 1.8V to 2.1V */
			//VoltageRange_2        ((uint8_t)0x01)  /*!<Device operating range: 2.1V to 2.7V */
			//VoltageRange_3        ((uint8_t)0x02)  /*!<Device operating range: 2.7V to 3.6V */
			//VoltageRange_4        ((uint8_t)0x03)  /*!<Device operating range: 2.7V to 3.6V + External Vpp */	
			startAddress=	startAddress_HOME;
		 GPIO_ResetBits(LED_OUT_Port,LED_OUT_Pin);// LED indicator ,If work in LAB then ON else then OFF
	}
	else
	{
			FLASH_EraseSector(FLASH_Sector_LAB, VoltageRange_3);
		  startAddress=	startAddress_LAB;
			GPIO_SetBits(LED_OUT_Port,LED_OUT_Pin);
	}
	//Save possition of encoder 
	FLASH_ProgramWord((startAddress)   ,Get_QEI(1));
	FLASH_ProgramWord((startAddress+4) ,Get_QEI(2));
	FLASH_ProgramWord((startAddress+8) ,Get_QEI(3));
	FLASH_ProgramWord((startAddress+12),Get_QEI(4));
	FLASH_ProgramWord((startAddress+16),Get_QEI(5));
	FLASH_ProgramWord((startAddress+20),Get_QEI(6));
  startAddress	+=24;
	// save possition  into Flash
  for(j=0;j<buffer_point;j++)
	{
		for(i=0;i<6;i++)
		{	
			FLASH_ProgramWord(startAddress,Servo_Pluse_buffer[j][i]);		
			startAddress	+=4;
		}
	}
	FLASH_Lock();
	UARTprint(USARTx,"Save All data into Flash Done ****");
}

void Display_possition(void)
{
	int i,j;
	int startAddress;
	if(GPIO_ReadInputDataBit(SW_SL0_Port,SW_SL0_Pin))
	{
	 Print(USARTx,"/****************************************/");	
	 UARTprint(USARTx," DATA AT HOME ");	
			startAddress=	startAddress_HOME;
		  GPIO_ResetBits(LED_OUT_Port,LED_OUT_Pin);// LED indicator ,If work in LAB then ON else then OFF
	}
	else
	{
	 Print(USARTx,"/****************************************/");	
	 UARTprint(USARTx," DATA AT LAB ");			
		  startAddress=	startAddress_LAB;		
			GPIO_SetBits(LED_OUT_Port,LED_OUT_Pin);
	}
	 startAddress	+=24;
	 Print(USARTx,"/****************************************/");	
	 Print(USARTx," Possition in Flash ");	
	 for(j=0;j<buffer_point;j++)
	{
		Print(USARTx,"* Possiton ");
    UARTPutnlong(USARTx,j);	
		USART_SendData(USARTx,(uint16_t)':'); 
		while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
		USART_SendData(USARTx,(uint16_t)' '); 
		while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
		for(i=0;i<6;i++)
		{			
			UARTPutnlong(USARTx,*(__IO uint32_t*)startAddress);
			startAddress+=4;
      USART_SendData(USARTx,(uint16_t)' '); 			
		  while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
		}
	}	
		USART_SendData(USARTx,(uint16_t)10);
		while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);	
}
void Get_possition(void)
{
	int startAddress;
	if(GPIO_ReadInputDataBit(SW_SL0_Port,SW_SL0_Pin))
	{
			startAddress=	startAddress_HOME;
		  GPIO_ResetBits(LED_OUT_Port,LED_OUT_Pin);// LED indicator ,If work in LAB then ON else then OFF
	}
	else
	{
		  startAddress=	startAddress_LAB;		
			GPIO_SetBits(LED_OUT_Port,LED_OUT_Pin);
	}
	Set_QEI(1,(*(__IO uint32_t*) (startAddress)));
	Set_QEI(2,(*(__IO uint32_t*) (startAddress+4)));
	Set_QEI(3,(*(__IO uint32_t*) (startAddress+8)));
	Set_QEI(4,(*(__IO uint32_t*) (startAddress+12)));
	Set_QEI(5,(*(__IO uint32_t*) (startAddress+16)));
	Set_QEI(6,(*(__IO uint32_t*) (startAddress+20)));
}
void Load_possition(void)
{
	int i,j, startAddress;
	Get_possition();
	if(GPIO_ReadInputDataBit(SW_SL0_Port,SW_SL0_Pin))
	{
			startAddress=	startAddress_HOME;
		  GPIO_ResetBits(LED_OUT_Port,LED_OUT_Pin);// LED indicator ,If work in LAB then ON else then OFF
	}
	else
	{
		  startAddress=	startAddress_LAB;		
			GPIO_SetBits(LED_OUT_Port,LED_OUT_Pin);
	}
	startAddress	+=24;
// Load value of Possition
	 for(j=0;j<buffer_point;j++)
	{
		for(i=0;i<6;i++)
		{			
		  Servo_Pluse_buffer[j][i]=*(__IO uint32_t*)(startAddress);					
			startAddress+=4;
		}
	}
		Print(USARTx,"Load data from Flash into Ram Done ****");	
		USART_SendData(USARTx,(uint16_t)10); 
}
