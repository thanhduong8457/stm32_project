#ifndef __pwm_H
#define __pwm_H
#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f4xx.h"
#include "delay.h"

void init_pwm(void);
void PWM_generate(double uk_dis,double uk_theta);
void pwm_left(double value);
void pwm_right(double value);

#ifdef __cplusplus
}
#endif
#endif
