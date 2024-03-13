///*
// * SPI.c
// *
// *  Created on: Aug 4, 2016
// *      Author: AnhTuan
// */

//void SPIx_IRQHANDLER(void)
//{
//  /* SPI in Receiver mode */
//  if (SPI_I2S_GetITStatus(SPIx, SPI_I2S_IT_RXNE) == SET)
//  {
//    if (ubRxIndex < SPI_BUFFERSIZE)
//    {
//      /* Receive Transaction data */
//      aRxBuffer[ubRxIndex++] = SPI_I2S_ReceiveData(SPIx);
//    }
//    else
//    {
//      /* Disable the Rx buffer not empty interrupt */
//      SPI_I2S_ITConfig(SPIx, SPI_I2S_IT_RXNE, DISABLE);
//    }
//  }
//  /* SPI in Tramitter mode */
//  if (SPI_I2S_GetITStatus(SPIx, SPI_I2S_IT_TXE) == SET)
//  {
//    if (ubTxIndex < SPI_BUFFERSIZE)
//    {
//      /* Send Transaction data */
//      SPI_I2S_SendData(SPIx, aTxBuffer[ubTxIndex++]);
//    }
//    else
//    {
//      /* Disable the Tx buffer empty interrupt */
//      SPI_I2S_ITConfig(SPIx, SPI_I2S_IT_TXE, DISABLE);
//    }
//  }
//}
//void SPI_Master_v2(void)
//{
//	GPIO_InitTypeDef GPIO_InitStructure;
//  NVIC_InitTypeDef NVIC_InitStructure;

//  /* Peripheral Clock Enable -------------------------------------------------*/
//  /* Enable the SPI clock */
//  SPIx_CLK_INIT(SPIx_CLK, ENABLE);
//  
//  /* Enable GPIO clocks */
//  RCC_AHB1PeriphClockCmd(SPIx_SCK_GPIO_CLK | SPIx_MISO_GPIO_CLK | SPIx_MOSI_GPIO_CLK, ENABLE);

//  /* SPI GPIO Configuration --------------------------------------------------*/
//  /* GPIO Deinitialisation */
//  GPIO_DeInit(SPIx_SCK_GPIO_PORT);
//  GPIO_DeInit(SPIx_MISO_GPIO_PORT);
//  GPIO_DeInit(SPIx_MOSI_GPIO_PORT);
//  
//  /* Connect SPI pins to AF5 */  
//  GPIO_PinAFConfig(SPIx_SCK_GPIO_PORT, SPIx_SCK_SOURCE, SPIx_SCK_AF);
//  GPIO_PinAFConfig(SPIx_MISO_GPIO_PORT, SPIx_MISO_SOURCE, SPIx_MISO_AF);    
//  GPIO_PinAFConfig(SPIx_MOSI_GPIO_PORT, SPIx_MOSI_SOURCE, SPIx_MOSI_AF);

//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;

//  /* SPI SCK pin configuration */
//  GPIO_InitStructure.GPIO_Pin = SPIx_SCK_PIN;
//  GPIO_Init(SPIx_SCK_GPIO_PORT, &GPIO_InitStructure);
//  
//  /* SPI  MISO pin configuration */
//  GPIO_InitStructure.GPIO_Pin =  SPIx_MISO_PIN;
//  GPIO_Init(SPIx_MISO_GPIO_PORT, &GPIO_InitStructure);  

//  /* SPI  MOSI pin configuration */
//  GPIO_InitStructure.GPIO_Pin =  SPIx_MOSI_PIN;
//  GPIO_Init(SPIx_MOSI_GPIO_PORT, &GPIO_InitStructure);
// 
//  /* SPI configuration -------------------------------------------------------*/
//  SPI_I2S_DeInit(SPIx);
//  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
//  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
//  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
//  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
//  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
//  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;
//  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
//  SPI_InitStructure.SPI_CRCPolynomial = 7;
//  
//  /* Configure the Priority Group to 1 bit */                
//  	NVIC_SetPriorityGrouping(3);
//  /* Configure the SPI interrupt priority */
//  NVIC_InitStructure.NVIC_IRQChannel = SPIx_IRQn;
//  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//  NVIC_Init(&NVIC_InitStructure);
//	/* Master board configuration */    
//  /* Initializes the SPI communication */
//  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
//  SPI_Init(SPIx, &SPI_InitStructure);
//  

////  /* Enable the SPI peripheral */
////  SPI_Cmd(SPIx, ENABLE);
////  
////  /* Initialize Buffer counters */
////  ubTxIndex = 0;
////  ubRxIndex = 0;

////  /* Enable the Rx buffer not empty interrupt */
////  SPI_I2S_ITConfig(SPIx, SPI_I2S_IT_RXNE, ENABLE);
////  
////  /* Enable the Tx buffer empty interrupt */
////  SPI_I2S_ITConfig(SPIx, SPI_I2S_IT_TXE, ENABLE);
////  /* Waiting the end of Data transfer */
////  while ((ubTxIndex < SPI_BUFFERSIZE) && (ubRxIndex < SPI_BUFFERSIZE))
////  {
////  }
////    	GPIO_SetBits(LED_OUT_Port,LED_OUT_Pin);
////  /* Disable the Rx buffer not empty interrupt */
////  SPI_I2S_ITConfig(SPIx, SPI_I2S_IT_RXNE, DISABLE);
////  
////  /* Disable the Tx empty interrupt */
////  SPI_I2S_ITConfig(SPIx, SPI_I2S_IT_TXE, DISABLE);
////  
////  /* Disable the SPI peripheral */
////  SPI_Cmd(SPIx, DISABLE);

//}
//static void SPI_Config(void)
//{
//  GPIO_InitTypeDef GPIO_InitStructure;
//  DMA_InitTypeDef DMA_InitStructure;

//  /* Peripheral Clock Enable -------------------------------------------------*/
//  /* Enable the SPI clock */
//  SPIx_CLK_INIT(SPIx_CLK, ENABLE);
//  
//  /* Enable GPIO clocks */
//  RCC_AHB1PeriphClockCmd(SPIx_SCK_GPIO_CLK | SPIx_MISO_GPIO_CLK | SPIx_MOSI_GPIO_CLK, ENABLE);
//  
//  /* Enable DMA clock */
//  RCC_AHB1PeriphClockCmd(SPIx_DMA_CLK, ENABLE);

//  /* SPI GPIO Configuration --------------------------------------------------*/
//  /* GPIO Deinitialisation */
//  GPIO_DeInit(SPIx_SCK_GPIO_PORT);
//  GPIO_DeInit(SPIx_MISO_GPIO_PORT);
//  GPIO_DeInit(SPIx_MOSI_GPIO_PORT);
//  
//  /* Connect SPI pins to AF5 */  
//  GPIO_PinAFConfig(SPIx_SCK_GPIO_PORT, SPIx_SCK_SOURCE, SPIx_SCK_AF);
//  GPIO_PinAFConfig(SPIx_MISO_GPIO_PORT, SPIx_MISO_SOURCE, SPIx_MISO_AF);    
//  GPIO_PinAFConfig(SPIx_MOSI_GPIO_PORT, SPIx_MOSI_SOURCE, SPIx_MOSI_AF);

//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;

//  /* SPI SCK pin configuration */
//  GPIO_InitStructure.GPIO_Pin = SPIx_SCK_PIN;
//  GPIO_Init(SPIx_SCK_GPIO_PORT, &GPIO_InitStructure);
//  
//  /* SPI  MISO pin configuration */
//  GPIO_InitStructure.GPIO_Pin =  SPIx_MISO_PIN;
//  GPIO_Init(SPIx_MISO_GPIO_PORT, &GPIO_InitStructure);  

//  /* SPI  MOSI pin configuration */
//  GPIO_InitStructure.GPIO_Pin =  SPIx_MOSI_PIN;
//  GPIO_Init(SPIx_MOSI_GPIO_PORT, &GPIO_InitStructure);
// 
//  /* SPI configuration -------------------------------------------------------*/
//  SPI_I2S_DeInit(SPIx);
//  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
//  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
//  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
//  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
//  SPI_InitStructure.SPI_NSS =  SPI_NSS_Soft;
//  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
//  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
//  SPI_InitStructure.SPI_CRCPolynomial = 7;
//  
//  /* DMA configuration -------------------------------------------------------*/
//  /* Deinitialize DMA Streams */
//  DMA_DeInit(SPIx_TX_DMA_STREAM);
//  DMA_DeInit(SPIx_RX_DMA_STREAM);
//  
//  /* Configure DMA Initialization Structure */
//  DMA_InitStructure.DMA_BufferSize = (uint32_t)SPI_BUFFERSIZE ;
//  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable ;
//  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull ;
//  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single ;
//  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
//  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
//  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
//  DMA_InitStructure.DMA_PeripheralBaseAddr =(uint32_t) (&(SPIx->DR)) ;
//  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
//  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
//  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
//  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
//  /* Configure TX DMA */
//  DMA_InitStructure.DMA_Channel = SPIx_TX_DMA_CHANNEL ;
//  DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral ;
//  DMA_InitStructure.DMA_Memory0BaseAddr =(uint32_t)aTxBuffer ;
//  DMA_Init(SPIx_TX_DMA_STREAM, &DMA_InitStructure);
//  /* Configure RX DMA */
//  DMA_InitStructure.DMA_Channel = SPIx_RX_DMA_CHANNEL ;
//  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory ;
//  DMA_InitStructure.DMA_Memory0BaseAddr =(uint32_t)aRxBuffer ; 
//  DMA_Init(SPIx_RX_DMA_STREAM, &DMA_InitStructure); 
//}
//static TestStatus Buffercmp(uint8_t* pBuffer1, __IO uint8_t* pBuffer2, uint16_t BufferLength)
//{
//  while (BufferLength--)
//  {
//    if (*pBuffer1 != *(pBuffer2))
//    {
//      return FAILED;
//    }
//    pBuffer1++;
//    pBuffer2++;
//  }

//  return PASSED;
//}