#ifndef INCLUDE_H_
#define INCLUDE_H_

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "stm32f4xx.h"

#include "define.h"
#include "UART/MYUART.h"
#include "Direct_Kinematics/Direct_Kinematics.h"
#include "Inverse_Kinematics/Inverse_Kinematic.h"
#include "TIMER/Timer.h"
#include "GPIO/MY_GPIO.h"
#include "MATRIX/matrix.h"
#include "QEI/qei.h"
#include "FLASH/MY_Flash.h"
#include "System_Manager/system_manager.h"
#include "External_Interrupt/external_interrupt.h"
#include "Moving_Control/moving_control.h"
#include "SPI/SPI.h"
#include "STM32_Slave_Interface/STM32_Slave_Interface.h"
#include "Rasperry_Interface/Rasperry_Interface.h"
#include "tm_stm32f4_usb_vcp.h"

#endif /* INCLUDE_H_ */
