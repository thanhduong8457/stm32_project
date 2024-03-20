/* Includes ------------------------------------------------------------------*/
#include "system_timetick.h"
#include "stm32f10x.h"

uint32_t	tick_count;
uint32_t	tick_flag;

void NMI_Handler(void) {
  //
}

void HardFault_Handler(void) {
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1) {
    //
  }
}

void MemManage_Handler(void) {
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1) {
    //
  }
}

void BusFault_Handler(void) {
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1) {
    //
  }
}

void UsageFault_Handler(void) {
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1) {
    //
  }
}

void SVC_Handler(void) {
  //
}

void DebugMon_Handler(void) {
  //
}
	  
void PendSV_Handler(void) {
  //
}

volatile uint32_t counter_ms = 0;
volatile uint32_t counter_us = 0;

__IO uint32_t TimingMillis;
static void __empty() {
	// Empty
}

void SysTick_Handler(void) {
	//with interrupt 1ms -> use 2 functions millis() and micros()
  counter_ms++; //1ms

	//if interrupt 1us
	// counter_us++; 
}

uint32_t millis(void) {
  return counter_ms;
}

uint32_t micros(void) {
  return counter_us;
  // return millis()*1000 + (SystemCoreClock/1000000-SysTick->VAL)/72;
}

void delay_init(void){
	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	SysTick_Config(SystemCoreClock / 1000);	// interrupt 1ms
  // SysTick_Config(SystemCoreClock / 1000000); // interrupt 1us
}

void delay_s (uint32_t s) {
  delay_ms(1000*s);
}

void delay_ms (uint32_t ms) {
  counter_ms = 0;
  while (millis() < ms);
}

void delay_us(uint32_t us) {
  counter_us = 0;
  while (micros() < us);
}
