// #include <iostream>
// #include <string>

#include "stm32f4xx.h"
#include "system_timetick.h"
#include "stdio.h"
#include "FreeRTOS.h"
#include "task.h"

// Macro to use CCM (Core Coupled Memory) in STM32F4
#define CCM_RAM __attribute__((section(".ccmram")))
#define FPU_TASK_STACK_SIZE 256

StackType_t fpuTaskStack[FPU_TASK_STACK_SIZE] CCM_RAM;  // Put task stack in CCM
StaticTask_t fpuTaskBuffer CCM_RAM;  // Put TCB in CCM

void test_function_test(void* p);

void init_gpio(void);

int main(void) {
    SystemInit();
    SystemCoreClockUpdate();
    delay_init();
    init_gpio();

    // Create a task
    // Stack and TCB are placed in CCM of STM32F4
    // The CCM block is connected directly to the core, which leads to zero wait states
    xTaskCreateStatic(test_function_test, "test_function_test", FPU_TASK_STACK_SIZE, NULL, 1, fpuTaskStack, &fpuTaskBuffer);

    printf("System Started!\n");
    vTaskStartScheduler();  // should never return

    while(1) {
#ifndef REGISTER
    GPIO_ToggleBits(GPIOD, GPIO_Pin_12|GPIO_Pin_13);
    delay_ms(500);
#else
    GPIOD->ODR |= (1<<12);
    delay_ms(50);
    GPIOD->ODR &= ~(1<<12);
    delay_ms(50);
#endif
    }
}


void vApplicationTickHook(void) {
}

/* vApplicationMallocFailedHook() will only be called if
   configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h.  It is a hook
   function that will get called if a call to pvPortMalloc() fails.
   pvPortMalloc() is called internally by the kernel whenever a task, queue,
   timer or semaphore is created.  It is also called by various parts of the
   demo application.  If heap_1.c or heap_2.c are used, then the size of the
   heap available to pvPortMalloc() is defined by configTOTAL_HEAP_SIZE in
   FreeRTOSConfig.h, and the xPortGetFreeHeapSize() API function can be used
   to query the size of free heap space that remains (although it does not
   provide information on how the remaining heap might be fragmented). */
void vApplicationMallocFailedHook(void) {
  taskDISABLE_INTERRUPTS();
  for(;;);
}

/* vApplicationIdleHook() will only be called if configUSE_IDLE_HOOK is set
   to 1 in FreeRTOSConfig.h.  It will be called on each iteration of the idle
   task.  It is essential that code added to this hook function never attempts
   to block in any way (for example, call xQueueReceive() with a block time
   specified, or call vTaskDelay()).  If the application makes use of the
   vTaskDelete() API function (as this demo application does) then it is also
   important that vApplicationIdleHook() is permitted to return to its calling
   function, because it is the responsibility of the idle task to clean up
   memory allocated by the kernel to any task that has since been deleted. */
void vApplicationIdleHook(void) {
}

void vApplicationStackOverflowHook(xTaskHandle pxTask, signed char *pcTaskName) {
  (void) pcTaskName;
  (void) pxTask;
  /* Run time stack overflow checking is performed if
     configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
     function is called if a stack overflow is detected. */
  taskDISABLE_INTERRUPTS();
  for(;;);
}

StaticTask_t xIdleTaskTCB CCM_RAM;
StackType_t uxIdleTaskStack[configMINIMAL_STACK_SIZE] CCM_RAM;

/* configUSE_STATIC_ALLOCATION is set to 1, so the application must provide an
implementation of vApplicationGetIdleTaskMemory() to provide the memory that is
used by the Idle task. */
void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize) {
  /* Pass out a pointer to the StaticTask_t structure in which the Idle task's
  state will be stored. */
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;

  /* Pass out the array that will be used as the Idle task's stack. */
  *ppxIdleTaskStackBuffer = uxIdleTaskStack;

  /* Pass out the size of the array pointed to by *ppxIdleTaskStackBuffer.
  Note that, as the array is necessarily of type StackType_t,
  configMINIMAL_STACK_SIZE is specified in words, not bytes. */
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}

static StaticTask_t xTimerTaskTCB CCM_RAM;
static StackType_t uxTimerTaskStack[configTIMER_TASK_STACK_DEPTH] CCM_RAM;

/* configUSE_STATIC_ALLOCATION and configUSE_TIMERS are both set to 1, so the
application must provide an implementation of vApplicationGetTimerTaskMemory()
to provide the memory that is used by the Timer service task. */
void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer, StackType_t **ppxTimerTaskStackBuffer, uint32_t *pulTimerTaskStackSize) {
  *ppxTimerTaskTCBBuffer = &xTimerTaskTCB;
  *ppxTimerTaskStackBuffer = uxTimerTaskStack;
  *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
}

/// @brief 
/// @param p 
void test_function_test(void* p) {
    float ff = 1.0f;
    printf("Start function test task.\n");
    for (;;) {
        // float s = sinf(ff);
        // ff += s;
        // TODO some other test
        GPIO_ToggleBits(GPIOD, GPIO_Pin_14|GPIO_Pin_15);

        vTaskDelay(1000);
    }

    vTaskDelete(NULL);
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
