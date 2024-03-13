
/*
 * UART.h
 *
 *  Created on: May 3, 2016
 *      Author: AnhTuan
 */

#ifndef UART_UART_H_
#define UART_UART_H_


/*	USART MASTER Communication with SLAVER */
#define USARTx1                           USART3
#define USARTx1_CLK                       RCC_APB1Periph_USART3
#define USARTx1_CLK_INIT                  RCC_APB1PeriphClockCmd
#define USARTx1_IRQn                      USART3_IRQn
#define USARTx1_IRQHandler                USART3_IRQHandler

#define USARTx1_TX_PIN                    GPIO_Pin_10                
#define USARTx1_TX_GPIO_PORT              GPIOC                       
#define USARTx1_TX_GPIO_CLK               RCC_AHB1Periph_GPIOC
#define USARTx1_TX_SOURCE                 GPIO_PinSource10
#define USARTx1_TX_AF                     GPIO_AF_USART3

#define USARTx1_RX_PIN                    GPIO_Pin_11                
#define USARTx1_RX_GPIO_PORT              GPIOC                    
#define USARTx1_RX_GPIO_CLK               RCC_AHB1Periph_GPIOC
#define USARTx1_RX_SOURCE                 GPIO_PinSource11
#define USARTx1_RX_AF                     GPIO_AF_USART3

/* Definition for DMAx resources ********************************************/
#define USARTx1_DR_ADDRESS                ((uint32_t)USART3 + 0x04) 

#define USARTx1_DMA                       DMA1
#define USARTx1_DMAx_CLK                  RCC_AHB1Periph_DMA1
	 
#define USARTx1_TX_DMA_CHANNEL            DMA_Channel_4
#define USARTx1_TX_DMA_STREAM             DMA1_Stream3
#define USARTx1_TX_DMA_FLAG_FEIF          DMA_FLAG_FEIF3
#define USARTx1_TX_DMA_FLAG_DMEIF         DMA_FLAG_DMEIF3
#define USARTx1_TX_DMA_FLAG_TEIF          DMA_FLAG_TEIF3
#define USARTx1_TX_DMA_FLAG_HTIF          DMA_FLAG_HTIF3
#define USARTx1_TX_DMA_FLAG_TCIF          DMA_FLAG_TCIF3
						
#define USARTx1_RX_DMA_CHANNEL            DMA_Channel_4
#define USARTx1_RX_DMA_STREAM             DMA1_Stream1
#define USARTx1_RX_DMA_FLAG_FEIF          DMA_FLAG_FEIF1
#define USARTx1_RX_DMA_FLAG_DMEIF         DMA_FLAG_DMEIF1
#define USARTx1_RX_DMA_FLAG_TEIF          DMA_FLAG_TEIF1
#define USARTx1_RX_DMA_FLAG_HTIF          DMA_FLAG_HTIF1
#define USARTx1_RX_DMA_FLAG_TCIF          DMA_FLAG_TCIF1

#define USARTx1_DMA_TX_IRQn               DMA1_Stream3_IRQn
#define USARTx1_DMA_RX_IRQn               DMA1_Stream1_IRQn
#define USARTx1_DMA_TX_IRQHandler         DMA1_Stream3_IRQHandler
#define USARTx1_DMA_RX_IRQHandler         DMA1_Stream1_IRQHandler


/* USART 2 MASTER Communition with PC Or SmartPhone */

#define USARTx                           USART2
#define USARTx_CLK                       RCC_APB1Periph_USART2
#define USARTx_CLK_INIT                  RCC_APB1PeriphClockCmd
#define USARTx_IRQn                      USART2_IRQn
#define USARTx_IRQHandler                USART2_IRQHandler

//#define USARTx_TX_PIN                    GPIO_Pin_5                
//#define USARTx_TX_GPIO_PORT              GPIOD                       
//#define USARTx_TX_GPIO_CLK               RCC_AHB1Periph_GPIOD
//#define USARTx_TX_SOURCE                 GPIO_PinSource5
//#define USARTx_TX_AF                     GPIO_AF_USART2

#define USARTx_TX_PIN                    GPIO_Pin_2                
#define USARTx_TX_GPIO_PORT              GPIOA                       
#define USARTx_TX_GPIO_CLK               RCC_AHB1Periph_GPIOA
#define USARTx_TX_SOURCE                 GPIO_PinSource2
#define USARTx_TX_AF                     GPIO_AF_USART2


#define USARTx_RX_PIN                    GPIO_Pin_6               
#define USARTx_RX_GPIO_PORT              GPIOD                  
#define USARTx_RX_GPIO_CLK               RCC_AHB1Periph_GPIOD
#define USARTx_RX_SOURCE                 GPIO_PinSource6
#define USARTx_RX_AF                     GPIO_AF_USART2

#define USARTx_DMA_Stream			  	 DMA1_Stream5
#define USARTx_DMA_Channel 				 DMA_Channel_4
#define USARTx_DMA_IRQ					 DMA1_Stream5_IRQn
#define	USARTx_DMA_IRQHandler 			 DMA1_Stream5_IRQHandler
#define USARTx_DMA_FLAG					 DMA_IT_TCIF5


//Public Variable 
extern volatile bool Flag_Request_App;

extern void USART_DMA (void); 
extern void ConfigUART4(void);
extern void ConfigUSART2(void);
extern void USART_Config(void);
extern void UARTPut(USART_TypeDef* UART_Base, int32_t num2send);
extern void UARTPutnlong(USART_TypeDef*  UART_Base, long Num);
extern void UARTPutFloat(USART_TypeDef* UART_Base, float fnum2send);
extern void UART_Print_Matrix(USART_TypeDef* UART_Base,real_T a[4][4]);
extern void UARTFloatx(USART_TypeDef* UART_Base ,float fnum2send);
extern void UARTprint(USART_TypeDef* UART_Base, char *send);
extern void Print(USART_TypeDef* UART_Base, char *send);
extern void UART_Send_Possition(USART_TypeDef* UART_Base, int32_t j1,int32_t j2,int32_t j3,int32_t j4, int32_t j5, int32_t j6);
extern void UART_Print_Double(USART_TypeDef* UART_Base ,double fnum2send);

#endif /* UART_UART_H_ */
