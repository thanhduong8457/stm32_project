#include "stm32f10x.h"
#include "system_timetick.h"

int main(void) {
  delay_init();

  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

  /* Configure PD0 and PD2 in output pushpull mode */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
     
  while (1) {
    GPIO_SetBits(GPIOC, GPIO_Pin_13);
    delay_ms(100);
    GPIO_ResetBits(GPIOC, GPIO_Pin_13);
    delay_ms(100);
    /* Set PD0 and PD2 */
    // GPIOC->BSRR = 0xffffffff;
    /* Reset PD0 and PD2 */
    // GPIOC->BRR  = 0x00000005;
  }
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}

#endif

/**
  * @}
  */

/**
  * @}
  */

