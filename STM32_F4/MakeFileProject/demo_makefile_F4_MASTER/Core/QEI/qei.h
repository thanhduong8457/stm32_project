/*
 * QEI.h
 *
 *  Created on: May 3, 2016
 *      Author: AnhTuan
 */

#ifndef QEI_QEI_H_
#define QEI_QEI_H_
//  Motor 1
#define ENC1A_PIN               GPIO_Pin_0
#define ENC1A_GPIO_PORT         GPIOA
#define ENC1A_GPIO_CLK          RCC_AHB1Periph_GPIOA
#define ENC1A_SOURCE            GPIO_PinSource0
#define ENC1A_AF                GPIO_AF_TIM5

#define ENC1B_PIN               GPIO_Pin_1
#define ENC1B_GPIO_PORT         GPIOA
#define ENC1B_GPIO_CLK          RCC_AHB1Periph_GPIOA
#define ENC1B_SOURCE            GPIO_PinSource1
#define ENC1B_AF                GPIO_AF_TIM5

//  Motor 2
#define ENC2A_PIN               GPIO_Pin_6
#define ENC2A_GPIO_PORT         GPIOA
#define ENC2A_GPIO_CLK          RCC_AHB1Periph_GPIOA
#define ENC2A_SOURCE            GPIO_PinSource6
#define ENC2A_AF                GPIO_AF_TIM3

#define ENC2B_PIN               GPIO_Pin_7
#define ENC2B_GPIO_PORT         GPIOA
#define ENC2B_GPIO_CLK          RCC_AHB1Periph_GPIOA
#define ENC2B_SOURCE            GPIO_PinSource7
#define ENC2B_AF                GPIO_AF_TIM3


//  Motor 3
#define ENC3A_PIN               GPIO_Pin_9
#define ENC3A_GPIO_PORT         GPIOE
#define ENC3A_GPIO_CLK          RCC_AHB1Periph_GPIOE
#define ENC3A_SOURCE            GPIO_PinSource9
#define ENC3A_AF                GPIO_AF_TIM1

#define ENC3B_PIN               GPIO_Pin_11
#define ENC3B_GPIO_PORT         GPIOE
#define ENC3B_GPIO_CLK          RCC_AHB1Periph_GPIOE
#define ENC3B_SOURCE            GPIO_PinSource11
#define ENC3B_AF                GPIO_AF_TIM1
//  Motor 4
#define ENC4A_PIN               GPIO_Pin_6
#define ENC4A_GPIO_PORT         GPIOB
#define ENC4A_GPIO_CLK          RCC_AHB1Periph_GPIOB
#define ENC4A_SOURCE            GPIO_PinSource6
#define ENC4A_AF                GPIO_AF_TIM4

#define ENC4B_PIN               GPIO_Pin_7
#define ENC4B_GPIO_PORT         GPIOB
#define ENC4B_GPIO_CLK          RCC_AHB1Periph_GPIOB
#define ENC4B_SOURCE            GPIO_PinSource7
#define ENC4B_AF                GPIO_AF_TIM4

//  Motor 5
#define ENC5A_PIN               GPIO_Pin_15
#define ENC5A_GPIO_PORT         GPIOA
#define ENC5A_GPIO_CLK          RCC_AHB1Periph_GPIOA
#define ENC5A_SOURCE            GPIO_PinSource15
#define ENC5A_AF                GPIO_AF_TIM2

#define ENC5B_PIN               GPIO_Pin_3
#define ENC5B_GPIO_PORT         GPIOB
#define ENC5B_GPIO_CLK          RCC_AHB1Periph_GPIOB
#define ENC5B_SOURCE            GPIO_PinSource3
#define ENC5B_AF                GPIO_AF_TIM2


//  Motor 6
#define ENC6A_PIN               GPIO_Pin_6
#define ENC6A_GPIO_PORT         GPIOC
#define ENC6A_GPIO_CLK          RCC_AHB1Periph_GPIOC
#define ENC6A_SOURCE            GPIO_PinSource6
#define ENC6A_AF                GPIO_AF_TIM8

#define ENC6B_PIN               GPIO_Pin_7
#define ENC6B_GPIO_PORT         GPIOC
#define ENC6B_GPIO_CLK          RCC_AHB1Periph_GPIOC
#define ENC6B_SOURCE            GPIO_PinSource7
#define ENC6B_AF                GPIO_AF_TIM8


extern void ConfigQEI(void);
extern int32_t Get_QEI(int joint );
extern void Set_QEI( int32_t joint,int32_t value );
extern void Reset_QEI(int joint );

#endif /* QEI_QEI_H_ */
