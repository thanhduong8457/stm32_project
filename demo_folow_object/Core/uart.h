#ifndef __uart_H
#define __uart_H
#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f4xx.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "stdint.h"

#include "cJSON.h"

#define USE_UART4
#define RX_BUFFER_SIZE 128

typedef struct {
	int8_t *Buffer;
	uint16_t Size;
	uint16_t Num;
	uint16_t In;
	uint16_t Out;
} USART_t;

extern double distance;
extern double theta;

#define USART2_BUFFER_SIZE 100

//USART2 default settings
#ifndef USART2_HARDWARE_FLOW_CONTROL
#define USART2_HARDWARE_FLOW_CONTROL		USART_HardwareFlowControl_None
#endif
#ifndef USART2_MODE
#define USART2_MODE						USART_Mode_Tx | USART_Mode_Rx
#endif
#ifndef USART2_PARITY
#define USART2_PARITY					USART_Parity_No
#endif
#ifndef USART2_STOP_BITS
#define USART2_STOP_BITS					USART_StopBits_1
#endif
#ifndef USART2_WORD_LENGTH
#define USART2_WORD_LENGTH				USART_WordLength_8b
#endif

void init_uart(void);
void USART_Putc(USART_TypeDef* USARTx, volatile char c);
void USART_Puts(USART_TypeDef* USARTx, char* str);
uint8_t USART_Getc(USART_TypeDef* USARTx);
uint16_t USART_Gets(USART_TypeDef* USARTx, char* buffer, uint16_t bufsize);
uint8_t USART_FindCharacter(USART_TypeDef* USARTx, volatile char c);
uint8_t USART_BufferEmpty(USART_TypeDef* USARTx);
uint8_t USART_BufferFull(USART_TypeDef* USARTx);
void USART_ClearBuffer(USART_TypeDef* USARTx);

void USART_INT_InsertToBuffer(USART_t* u, uint8_t c);
USART_t* USART_INT_GetUsart(USART_TypeDef* USARTx);
void clear_rx_buffer(void);
void json_data_process(char *data);
void do_something(void);

#ifdef __cplusplus
}
#endif
#endif
