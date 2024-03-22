#include "stm32f10x.h"
#include "system_timetick.h"
#include "system_stm32f10x.h"

// #include "stm32f10x_gpio.h"
// #include "stm32f10x_rcc.h"
// #include "stm32f10x_tim.h"

#include <stdio.h>

// Function prototypes
void UART1_Init(void);
void UART1_SendChar(char ch);

void TIM2_PWM_Init(void);
void update_pwm(int percent); // Function prototype
void gpio_init(void);

extern "C" { // use extern for link C funtions
    void USART1_IRQHandler(void);
    int _write(int file, char *ptr, int len);
}

int main(void) {
    SystemInit();
    SystemCoreClockUpdate();

    delay_init(); // Initialize time tick
    gpio_init(); // Initialize GPIO for pin PC13
    UART1_Init(); // Initialize UART
    TIM2_PWM_Init(); // Initialize PWM on TIM1

    unsigned int temp = 0;

    while (1) {
        temp += 10; 
        if (temp > 100) {
            temp = 0;
        }

        GPIO_SetBits(GPIOC, GPIO_Pin_13);
        delay_ms(500);

        GPIO_ResetBits(GPIOC, GPIO_Pin_13);
        delay_ms(500);

        update_pwm(temp);

        printf("pwm value is udpated to %d%\r\n", temp);
    }
    return 0;
}

void gpio_init(void) {
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

    /* Configure PD0 and PD2 in output pushpull mode */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
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

// Initialize PWM on TIM2
void TIM2_PWM_Init(void) {
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;

    // Enable peripheral clocks
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    // Configure TIM2 pins: CH1 (PA0) as alternate function output push-pull
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // TIM2 configuration
    TIM_TimeBaseStructure.TIM_Period = 1000 - 1; // PWM period (1 kHz)
    TIM_TimeBaseStructure.TIM_Prescaler = 72 - 1; // Prescaler (72 MHz / 72 = 1 MHz)
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    // PWM mode configuration for TIM2 Channel 1
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 0; // 0% duty cycle initially
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC1Init(TIM2, &TIM_OCInitStructure);
    TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);

    // Enable TIM2 counter
    TIM_Cmd(TIM2, ENABLE);
}

// Update PWM pulse width based on percentage
void update_pwm(int percent) {
    if (percent < 0 || percent > 100) {
        // Invalid input, return without updating PWM
        return;
    }

    // Calculate pulse width value
    uint16_t pulse_width = (uint16_t)((percent / 100.0) * TIM2->ARR);

    // Update PWM pulse width
    TIM_SetCompare1(TIM2, pulse_width);
}