#include "include.h"

int main(void)
{
	
  system_SetState(initial_system);

	while(1)
	{								
			  switch(system_GetState())
				{
					case initial_system :
							System_Initial();
							break;
					case read_enable:		
							Read_Enable();
							break;
						case clear_error :
							Clear_Error();
							break;
					case enable_servo:		
							Enable_Servo();
							break;
					case waiting_servo_ready :
							Waiting_Servo_Ready();
							break;
					case check_servo_ready:		
							Check_Servo_Ready();
							break;
					case error :
							Error();
							break;
					case initial_success:		
							Initial_Success();
							break;
					case home :
							Home();
							break;
					case detect_possition :
							Detect_position();
							break;
					case open_loop:		
							Open_loop();
							break;
					case inverse_kinematics:		
							Inverse_Kinematic();
							break;
					case waitting:
						Waitting();
					break;
					default :
							break;
				}	
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

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
