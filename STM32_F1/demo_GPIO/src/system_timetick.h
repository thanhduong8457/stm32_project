
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F10x_IT_H
#define __STM32F10x_IT_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

/*  These function configure scheduler timer
    Sampling time is set by changing value PERIOD
    in the file "system_timetick.c"
*/

extern uint32_t tick_count;
extern uint32_t tick_flag;

void NMI_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void SVC_Handler(void);
void DebugMon_Handler(void);
void PendSV_Handler(void);
void SysTick_Handler(void);

uint32_t millis(void);
uint32_t micros(void);

void delay_init(void);
void delay_ms(uint32_t nms);
void delay_us(uint32_t us);
void delay_s( uint32_t s );

#ifdef __cplusplus
}
#endif

#endif /* __STM32F10x_IT_H */


