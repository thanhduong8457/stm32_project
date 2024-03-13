/*
 * STM32_Slave_Interface.c
 *
 *  Created on: Aug 5, 2016
 *      Author: AnhTuan
 */

#include "include.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
SPI_InitTypeDef  SPI_InitStructure;
/* Public variables ---------------------------------------------------------*/
volatile bool Flag_Send_Slave_Done=false;
 uint8_t aTxBuffer[SPI_BUFFERSIZE]= {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 ,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
__IO uint8_t aRxBuffer [SPI_BUFFERSIZE];


/* Public function prototypes -----------------------------------------------*/
void SPI_Master_Config(void);

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/


void SPIx_TX_DMA_IRQHandler(void)
{
	if(DMA_GetITStatus(SPIx_TX_DMA_STREAM, SPIx_TX_DMA_FLAG_TCIF)==RESET)
  {
	/* Clear the DMA1_Streamx TCIFx pending bit */
  DMA_ClearITPendingBit(SPIx_TX_DMA_STREAM, SPIx_TX_DMA_FLAG_TCIF);
  DMA_ClearFlag(SPIx_RX_DMA_STREAM,SPIx_RX_DMA_FLAG_TCIF); 
		
	/*Send Txbuffer Finish and Stop SPI SCK*/	
	  /* Disable DMA SPI TX Stream */
  DMA_Cmd(SPIx_TX_DMA_STREAM,DISABLE);

  /* Disable DMA SPI RX Stream */
  DMA_Cmd(SPIx_RX_DMA_STREAM,DISABLE);  
  
  /* Disable SPI DMA TX Requsts */
  SPI_I2S_DMACmd(SPIx, SPI_I2S_DMAReq_Tx, DISABLE);

  /* Disable SPI DMA RX Requsts */
  SPI_I2S_DMACmd(SPIx, SPI_I2S_DMAReq_Rx, DISABLE);

  /* Disable the SPI peripheral */
  SPI_Cmd(SPIx, DISABLE);
	Flag_Send_Slave_Done=true;
	UARTprint(USARTx,"SEND ok");

	}
}
void Send_To_STM32_Slave(void)
{
	
//	Transfer data to aTxBuffer
		UARTprint(USARTx,"SEND ");
						Flag_Send_Slave_Done=false;
					 /* Enable DMA SPI TX Stream */
						DMA_Cmd(SPIx_TX_DMA_STREAM,ENABLE);
						/* Enable DMA SPI RX Stream */
						DMA_Cmd(SPIx_RX_DMA_STREAM,ENABLE);
						/* Enable SPI DMA TX Requsts */
						SPI_I2S_DMACmd(SPIx, SPI_I2S_DMAReq_Tx, ENABLE);
						/* Enable SPI DMA RX Requsts */
						SPI_I2S_DMACmd(SPIx, SPI_I2S_DMAReq_Rx, ENABLE);
						/* Enable the SPI peripheral */
						SPI_Cmd(SPIx, ENABLE);	
}
void SPI_Master_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
  DMA_InitTypeDef DMA_InitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;
 /* Enable RX DMA Interrupt to the highest priority */
  NVIC_SetPriorityGrouping(3);
  NVIC_InitStructure.NVIC_IRQChannel = SPIx_TX_DMA_IRQ;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	  /* DMA configuration -------------------------------------------------------*/
  /* Deinitialize DMA Streams */
  DMA_DeInit(SPIx_TX_DMA_STREAM);
  DMA_DeInit(SPIx_RX_DMA_STREAM);
  
  /* Configure DMA Initialization Structure */
  DMA_InitStructure.DMA_BufferSize = (uint32_t)SPI_BUFFERSIZE ;
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable ;
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOStatus_HalfFull ;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single ;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
  DMA_InitStructure.DMA_PeripheralBaseAddr =(uint32_t) (&(SPIx->DR)) ;
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  /* Configure TX DMA */
  DMA_InitStructure.DMA_Channel = SPIx_TX_DMA_CHANNEL ;
  DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral ;
  DMA_InitStructure.DMA_Memory0BaseAddr =(uint32_t)aTxBuffer ;
  DMA_Init(SPIx_TX_DMA_STREAM, &DMA_InitStructure);
  /* Configure RX DMA */
  DMA_InitStructure.DMA_Channel = SPIx_RX_DMA_CHANNEL ;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory ;
  DMA_InitStructure.DMA_Memory0BaseAddr =(uint32_t)aRxBuffer ; 
  DMA_Init(SPIx_RX_DMA_STREAM, &DMA_InitStructure);   
  /* The Data transfer is performed in the SPI using Direct Memory Access */
  /* Transfer complete interrupt mask */
  DMA_ITConfig(SPIx_TX_DMA_STREAM, DMA_IT_TC, ENABLE);
  /* Peripheral Clock Enable -------------------------------------------------*/
  /* Enable the SPI clock */
  SPIx_CLK_INIT(SPIx_CLK, ENABLE);
  /* Enable GPIO clocks */
  RCC_AHB1PeriphClockCmd(SPIx_SCK_GPIO_CLK | SPIx_MISO_GPIO_CLK | SPIx_MOSI_GPIO_CLK, ENABLE);
  
  /* Enable DMA clock */
  RCC_AHB1PeriphClockCmd(SPIx_DMA_CLK, ENABLE);

  /* SPI GPIO Configuration --------------------------------------------------*/
  /* GPIO Deinitialisation */
  GPIO_DeInit(SPIx_SCK_GPIO_PORT);
  GPIO_DeInit(SPIx_MISO_GPIO_PORT);
  GPIO_DeInit(SPIx_MOSI_GPIO_PORT);
  
  /* Connect SPI pins to AF5 */  
  GPIO_PinAFConfig(SPIx_SCK_GPIO_PORT, SPIx_SCK_SOURCE, SPIx_SCK_AF);
  GPIO_PinAFConfig(SPIx_MISO_GPIO_PORT, SPIx_MISO_SOURCE, SPIx_MISO_AF);    
  GPIO_PinAFConfig(SPIx_MOSI_GPIO_PORT, SPIx_MOSI_SOURCE, SPIx_MOSI_AF);

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;

  /* SPI SCK pin configuration */
  GPIO_InitStructure.GPIO_Pin = SPIx_SCK_PIN;
  GPIO_Init(SPIx_SCK_GPIO_PORT, &GPIO_InitStructure);
  
  /* SPI  MISO pin configuration */
  GPIO_InitStructure.GPIO_Pin =  SPIx_MISO_PIN;
  GPIO_Init(SPIx_MISO_GPIO_PORT, &GPIO_InitStructure);  

  /* SPI  MOSI pin configuration */
  GPIO_InitStructure.GPIO_Pin =  SPIx_MOSI_PIN;
  GPIO_Init(SPIx_MOSI_GPIO_PORT, &GPIO_InitStructure);
 
  /* SPI configuration -------------------------------------------------------*/
  SPI_I2S_DeInit(SPIx);
	/* Initializes the SPI communication */
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;//MODE 
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
  SPI_InitStructure.SPI_NSS =  SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
 /* Master board configuration */    

  SPI_Init(SPIx, &SPI_InitStructure); 	
}

