#include "stm32f10x.h"
#include "system_timetick.h"
#include "system_stm32f10x.h"

#include "uart.h"
#include "gpio.h"
#include "pwm.h"

volatile unsigned int share_value = 0;
bool revert = false;

int main(void) {
    SystemInit();
    SystemCoreClockUpdate();

    delay_init(); // Initialize time tick
    gpio_init(); // Initialize GPIO for pin PC13
    TIM4_PWM_Init(); // Initialize PWM on TIM4
    UART1_Init(); // Initialize UART

    set_servo_angle(share_value);

    int delay = 10;

    int angle = 0;
    bool derection = false;
    while (1) {

        // GPIO_SetBits(GPIOC, GPIO_Pin_13);
        // delay_ms(500);
        // GPIO_ResetBits(GPIOC, GPIO_Pin_13);
        // delay_ms(500);
        // printf("current angle is %d degree\r\n", share_value);

        set_servo_angle(0);
        delay_ms(1000);

        set_servo_angle(45);
        delay_ms(1000);

        set_servo_angle(90);
        delay_ms(1000);

        set_servo_angle(135);
        delay_ms(1000);

        set_servo_angle(180);
        delay_ms(1000);
        // if (false == derection) {
        //     angle += 1;
        // } 
        // else {
        //     angle -= 1;
        // }

        // if (180 < angle) {
        //     derection = true;
        // } 
        // else if (0 > angle) {
        //     derection = false;
        // }
    }
}
