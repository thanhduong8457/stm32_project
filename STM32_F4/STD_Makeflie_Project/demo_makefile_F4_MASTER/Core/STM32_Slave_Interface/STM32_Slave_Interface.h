/*
 * STM32_Slave_Interface.h
 *
 *  Created on: Aug 5, 2016
 *      Author: AnhTuan
 */

#ifndef STM32_SLAVE_INTERFACE_H_
#define STM32_SLAVE_INTERFACE_H_
 typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;
 
  #define SPIx                           SPI2
  #define SPIx_CLK                       RCC_APB1Periph_SPI2
  #define SPIx_CLK_INIT                  RCC_APB1PeriphClockCmd
  #define SPIx_IRQn                      SPI2_IRQn
  #define SPIx_IRQHANDLER                SPI2_IRQHandler

  #define SPIx_SCK_PIN                   GPIO_Pin_13
  #define SPIx_SCK_GPIO_PORT             GPIOB
  #define SPIx_SCK_GPIO_CLK              RCC_AHB1Periph_GPIOB
  #define SPIx_SCK_SOURCE                GPIO_PinSource13
  #define SPIx_SCK_AF                    GPIO_AF_SPI2

  #define SPIx_MISO_PIN                  GPIO_Pin_14
  #define SPIx_MISO_GPIO_PORT            GPIOB
  #define SPIx_MISO_GPIO_CLK             RCC_AHB1Periph_GPIOB
  #define SPIx_MISO_SOURCE               GPIO_PinSource14
  #define SPIx_MISO_AF                   GPIO_AF_SPI2

  #define SPIx_MOSI_PIN                  GPIO_Pin_15
  #define SPIx_MOSI_GPIO_PORT            GPIOB
  #define SPIx_MOSI_GPIO_CLK             RCC_AHB1Periph_GPIOB
  #define SPIx_MOSI_SOURCE               GPIO_PinSource15
  #define SPIx_MOSI_AF                   GPIO_AF_SPI2

  #define SPIx_DMA                       DMA1
  #define SPIx_DMA_CLK                   RCC_AHB1Periph_DMA1
	
  #define SPIx_TX_DMA_CHANNEL            DMA_Channel_0
  #define SPIx_TX_DMA_STREAM             DMA1_Stream4
  #define SPIx_TX_DMA_FLAG_TCIF          DMA_FLAG_TCIF4
	#define SPIx_TX_DMA_IRQ								 DMA1_Stream4_IRQn
	#define	SPIx_TX_DMA_IRQHandler 				 DMA1_Stream4_IRQHandler
	
  #define SPIx_RX_DMA_CHANNEL            DMA_Channel_0
  #define SPIx_RX_DMA_STREAM             DMA1_Stream3
  #define SPIx_RX_DMA_FLAG_TCIF          DMA_FLAG_TCIF3
	#define SPIx_RX_DMA_IRQ								 DMA1_Stream3_IRQn
	#define	SPIx_RX_DMA_IRQHandler 				 DMA1_Stream3_IRQHandler

  #define SPI_BUFFERSIZE                   33
extern  uint8_t aTxBuffer[SPI_BUFFERSIZE];
extern __IO uint8_t aRxBuffer [SPI_BUFFERSIZE];
extern volatile bool Flag_Send_Slave_Done;
/* Public function prototypes -----------------------------------------------*/
extern void SPI_Master_Config(void);
extern void Send_To_STM32_Slave(void);

#endif /* STM32_SLAVE_INTERFACE_H_ */
