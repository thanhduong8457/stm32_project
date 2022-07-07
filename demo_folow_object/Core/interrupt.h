#ifndef __interrupt_H
#define __interrupt_H
#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f4xx.h"

#define SET_POINT_DISTANCE 20
#define SET_POINT_THETA 0

extern double distance;
extern double theta;

extern double uk_dis;
extern double uk_theta;

void init_interrupt(void);
void TIM2_IRQHandler(void);
void pid_control(void);

#ifdef __cplusplus
}
#endif
#endif
