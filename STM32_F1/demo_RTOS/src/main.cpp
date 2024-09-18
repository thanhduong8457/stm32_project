#include "stm32f10x.h"
#include "stdio.h"
#include "FreeRTOS.h"
#include "task.h"

#include "uart.h"
#include "gpio.h"
#include "pwm.h"

volatile unsigned int share_value = 0;
bool revert = false;

void vTask1(void * pvParams);
void vTask2(void * pvParams);
void vTask3(void * pvParams);
void vTask4(void * pvParams);

/// @brief 
/// @param  
/// @return 
int main(void) {
    SystemInit();
    SystemCoreClockUpdate();

    gpio_init(); // Initialize GPIO for pin PC13
    TIM4_PWM_Init(); // Initialize PWM on TIM4
    UART1_Init(); // Initialize UART

    // Create a task
    // Stack and TCB are placed in CCM of STM32F1
    // The CCM block is connected directly to the core, which leads to zero wait states
    xTaskCreate(vTask1, "T1", 130, NULL, 1, NULL);
    xTaskCreate(vTask2, "T2", 130, NULL, 1, NULL);
    xTaskCreate(vTask3, "T3", 130, NULL, 1, NULL);
    xTaskCreate(vTask4, "T4", 130, NULL, 1, NULL);

    vTaskStartScheduler();

    printf("System Started!\n");
    vTaskStartScheduler();  // should never return
    return -1;
}

/// @brief 
/// @param pvParams 
void vTask1(void * pvParams) {
    while(true) {
        GPIO_SetBits(GPIOC, GPIO_Pin_13);
        vTaskDelay(2000);

    }
}

/// @brief 
/// @param pvParams 
void vTask2(void * pvParams) {
    while(true) {
        GPIO_ResetBits(GPIOC, GPIO_Pin_13);
        vTaskDelay(1000);
    }
}

/// @brief 
/// @param pvParams 
void vTask3(void * pvParams) {
    while(true) {
        set_servo_angle(share_value);
        vTaskDelay(100);
    }
}

/// @brief 
/// @param pvParams 
void vTask4(void * pvParams) {
        while(true) {
        switch (share_value) {
        case 0:
            share_value = 45;
            break;

        case 45:
            share_value = 90;
            break;

        case 90:
            share_value = 135;
            break;

        case 135:
            share_value = 180;
            break;

        default:
            share_value = 0;
            break;
        }
        printf("current angle is %d degree\r\n", share_value);
        vTaskDelay(2000);
    }
}
