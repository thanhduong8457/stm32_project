#include "stm32f10x.h"
#include "system_timetick.h"
#include "system_stm32f10x.h"

#include <stdio.h>

// Function prototypes
void UART1_Init(void);
void UART1_SendChar(char ch);

extern "C" { // use extern for link C funtions
    void USART1_IRQHandler(void);
    int _write(int file, char *ptr, int len);
}

int main(void) {
    SystemInit();
    SystemCoreClockUpdate();
    delay_init();

    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

    /* Configure PD0 and PD2 in output pushpull mode */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    // Initialize UART
    UART1_Init();
    unsigned int temp = 0;
    while (1) {
        GPIO_SetBits(GPIOC, GPIO_Pin_13);
        delay_ms(100);
        GPIO_ResetBits(GPIOC, GPIO_Pin_13);
        delay_ms(100);
        printf("thanh duong say hello %d times to this world\r\n", ++temp);
    }
    return 0;
}

// Initialize UART1
void UART1_Init(void) {
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure; // Define NVIC_InitStructure here

    // Enable UART1 and GPIO clock
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1, ENABLE);

    // Configure UART1 pins: Tx (PA9) and Rx (PA10)
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // Configure UART1
    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1, &USART_InitStructure);

    // Enable UART1
    USART_Cmd(USART1, ENABLE);

    // Enable USART1 RX interrupt
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

    // Configure and enable USART1 interrupt
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

// UART1 interrupt handler
void USART1_IRQHandler(void) {
    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {
        char received_char = USART_ReceiveData(USART1);
        // Handle received character
        UART1_SendChar(received_char);
    }
}

// Send a character via UART1
void UART1_SendChar(char ch) {
    // Wait until transmit data register is empty
    while (!(USART1->SR & USART_SR_TXE));
    // Send a character
    USART1->DR = (ch & 0xFF);
}

// Redirect fputc() to UART1
int fputc(int ch, FILE *f) {
    UART1_SendChar(ch);
    return ch;
}

// Implementing _write for printf
// This function is used by printf to send character output
int _write(int file, char *ptr, int len) {
    int i;
    for (i = 0; i < len; i++) {
        UART1_SendChar(*ptr++);
    }
    return len;
}
