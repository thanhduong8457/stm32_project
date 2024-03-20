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
	//neu ngat 1ms su dung 2 ham millis() v?micros()
  counter_ms++;//1ms
	//neu ngat 1us
	//counter_us++; counter_ms %= counter_us;//1us
}

uint32_t millis(void) {
  return counter_ms;
}

uint32_t micros(void) {
  int Micros = millis()*1000 + (SystemCoreClock/1000000-SysTick->VAL)/72;
  return Micros;
}

void delay_init(void){
	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	//SysTick_Config(SystemCoreClock / 1000);	// ngat 1ms
  SysTick_Config(SystemCoreClock / 1000000); // Configure SysTick timer
}
void delay_ms (uint32_t ms) {
  delay_us(1000*ms);
}

void delay_us(uint32_t us) {
  counter_ms = 0;
  while (millis() < us);
}
