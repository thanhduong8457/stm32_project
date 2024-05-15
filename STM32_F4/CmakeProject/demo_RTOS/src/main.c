// #include <iostream>
// #include <string>

#include "stm32f4xx.h"
#include "stdio.h"
#include "FreeRTOS.h"
#include "task.h"

void vTask1(void * pvParams);
void vTask2(void * pvParams);
void vTask3(void * pvParams);
void vTask4(void * pvParams);

void init_gpio(void);

int main(void) {
    SystemInit();
    SystemCoreClockUpdate();
    init_gpio();

    // Create a task
    // Stack and TCB are placed in CCM of STM32F4
    // The CCM block is connected directly to the core, which leads to zero wait states
    xTaskCreate (vTask1, "T1", 150, NULL, 1, NULL);
    xTaskCreate (vTask2, "T2", 150, NULL, 1, NULL);
    xTaskCreate (vTask3, "T2", 150, NULL, 1, NULL);
    xTaskCreate (vTask4, "T2", 150, NULL, 1, NULL);

    vTaskStartScheduler();

    printf("System Started!\n");
    vTaskStartScheduler();  // should never return
    return -1;
}

void init_gpio(void) {
    #ifndef REGISTER
    GPIO_InitTypeDef GPIO_InitStructure; 
    //Enable clock GPIOD
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;

    GPIO_Init(GPIOD, &GPIO_InitStructure);  
    #else
    /*GPIOD clock*/
    RCC->AHB1ENR = (1<<3);
    /*GPIOD config*/
    GPIOD->MODER &= ~(3<<24);
    GPIOD->MODER |= (1<<24);
    GPIOD->OTYPER &= ~(1<<12);
    #endif   
}

void vTask1(void * pvParams) {
    while(1) {
#ifndef REGISTER
        GPIO_ToggleBits(GPIOD, GPIO_Pin_12);
        vTaskDelay(100);
#else
        GPIOD->ODR |= (1<<12);
        delay_ms(50);
        GPIOD->ODR &= ~(1<<12);
        delay_ms(50);
#endif
    }
}

void vTask2(void * pvParams) {
    while(1) {
#ifndef REGISTER
        GPIO_ToggleBits(GPIOD, GPIO_Pin_13);
        vTaskDelay(200);
#else
        GPIOD->ODR |= (1<<12);
        delay_ms(50);
        GPIOD->ODR &= ~(1<<12);
        delay_ms(50);
#endif
    }
}

void vTask3(void * pvParams) {
    while(1) {
#ifndef REGISTER
        GPIO_ToggleBits(GPIOD, GPIO_Pin_14);
        vTaskDelay(300);
#else
        GPIOD->ODR |= (1<<12);
        delay_ms(50);
        GPIOD->ODR &= ~(1<<12);
        delay_ms(50);
#endif
    }
}

void vTask4(void * pvParams) {
    while(1) {
#ifndef REGISTER
        GPIO_ToggleBits(GPIOD, GPIO_Pin_15);
        vTaskDelay(400);
#else
        GPIOD->ODR |= (1<<12);
        delay_ms(50);
        GPIOD->ODR &= ~(1<<12);
        delay_ms(50);
#endif
    }
}