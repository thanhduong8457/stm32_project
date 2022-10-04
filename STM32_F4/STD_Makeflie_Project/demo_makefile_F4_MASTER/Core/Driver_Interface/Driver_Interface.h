/*
 * Driver_Interface.h
 *
 *  Created on: Jul 31, 2016
 *      Author: AnhTuan
 */

#ifndef DRIVER_INTERFACE_H_
#define DRIVER_INTERFACE_H_

//#define USART_DRIVER_1                           UART4
#define USART_DRIVER_1                           USART3
#define USART_DRIVER_1_CLK                       RCC_APB1Periph_USART3
#define USART_DRIVER_1_CLK_INIT                  RCC_APB1PeriphClockCmd
#define USART_DRIVER_1_IRQn                      USART3_IRQn
#define USART_DRIVER_1_IRQHandler                USART3_IRQHandler

#define USART_DRIVER_1_TX_PIN                    GPIO_Pin_10                
#define USART_DRIVER_1_TX_GPIO_PORT              GPIOC                       
#define USART_DRIVER_1_TX_GPIO_CLK               RCC_AHB1Periph_GPIOC
#define USART_DRIVER_1_TX_SOURCE                 GPIO_PinSource10
#define USART_DRIVER_1_TX_AF                     GPIO_AF_USART3

#define USART_DRIVER_1_RX_PIN                    GPIO_Pin_11                
#define USART_DRIVER_1_RX_GPIO_PORT              GPIOC                   
#define USART_DRIVER_1_RX_GPIO_CLK               RCC_AHB1Periph_GPIOC
#define USART_DRIVER_1_RX_SOURCE                 GPIO_PinSource11
#define USART_DRIVER_1_RX_AF                     GPIO_AF_USART3

#define USART_DRIVER_1_DMA_Stream			  	       DMA1_Stream2
#define USART_DRIVER_1_DMA_Channel 			         DMA_Channel_4
#define USART_DRIVER_1_DMA_IRQ				           DMA1_Stream2_IRQn
#define USART_DRIVER_1_DMA_IRQHandler 		       DMA1_Stream2_IRQHandler
#define USART_DRIVER_1_DMA_FLAG				           DMA_IT_TCIF2


// Public function
 extern void USART_DRIVER_1_Config(void);
 extern void Driver_Send_Setting(USART_TypeDef* UART_Base, float acc,float dec,float speed);




#endif /* DRIVER_INTERFACE_H_ */
