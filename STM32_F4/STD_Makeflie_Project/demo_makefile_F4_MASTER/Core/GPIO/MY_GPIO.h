/*
 * MY_GPIO.h
 *
 *  Created on: May 4, 2016
 *      Author: AnhTuan
 */

#ifndef MY_GPIO_H_
#define MY_GPIO_H_

/******************************************************************
 * PROTOTYPE
 *****************************************************************/
/*
Enable_Servo_1    PA3
EN_MOTOR1         PD10 
SERVO_READY_1     PA2
PULSE_1						PA5
DIRECTION_1				PA4

Enable_Servo_2    PE10
EN_MOTOR2         PD11 
SERVO_READY_2     PE8
PULSE_2						PE14
DIRECTION_2				PE12

Enable_Servo_3    PB13
EN_MOTOR3         PD12 
SERVO_READY_3     PB12
PULSE_3						PD9
DIRECTION_3				PD8

Enable_Servo_4    PE1
EN_MOTOR4         PD13 
SERVO_READY_4     PE2
PULSE_4						PE3
DIRECTION_4				PE5

Enable_Servo_5    PD3
EN_MOTOR5         PD14
SERVO_READY_5     PD7
PULSE_5						PD2
DIRECTION_5				PD4

Enable_Servo_6    PD0
EN_MOTOR6         PD15 
SERVO_READY_6     PA14
PULSE_6						PC8
DIRECTION_6				PA13
*/
/*******************
*Config GPIO give Pulse to Driver 
*
*********************/
//Motor_1 
/*
Enable_Servo_1    PA3
EN_MOTOR1         PD10 
SERVO_READY_1     PA2
PULSE_1						PA5
DIRECTION_1				PA4
*/
#define Servo_Ready_Pin_1                   GPIO_Pin_2    //SERVO_READY_1     PA2            
#define Servo_Ready_Port_1              	  GPIOA                    
#define Servo_Ready_Clk_1               	  RCC_AHB1Periph_GPIOA
#define Servo_In_Enable_Pin_1               GPIO_Pin_10   //EN_MOTOR1         PD10           
#define Servo_In_Enable_Port_1             	GPIOD                    
#define Servo_In_Enable_Clk_1               RCC_AHB1Periph_GPIOD
#define Servo_OUT_Enable_Pin_1              GPIO_Pin_3    //Enable_Servo_1    PA3             
#define Servo_OUT_Enable_Port_1            	GPIOA                    
#define Servo_OUT_Enable_Clk_1              RCC_AHB1Periph_GPIOA

//Motor_2
/*
Enable_Servo_2    PE10
EN_MOTOR2         PD11 
SERVO_READY_2     PE8
PULSE_2						PE14
DIRECTION_2				PE12
*/
#define Servo_Ready_Pin_2                   GPIO_Pin_8      //SERVO_READY_2     PE8            
#define Servo_Ready_Port_2              	  GPIOE                    
#define Servo_Ready_Clk_2               	  RCC_AHB1Periph_GPIOE
#define Servo_In_Enable_Pin_2               GPIO_Pin_11     //EN_MOTOR2         PD11           
#define Servo_In_Enable_Port_2             	GPIOD                    
#define Servo_In_Enable_Clk_2               RCC_AHB1Periph_GPIOD
#define Servo_OUT_Enable_Pin_2              GPIO_Pin_10     //Enable_Servo_2    PE10    
#define Servo_OUT_Enable_Port_2            	GPIOE                    
#define Servo_OUT_Enable_Clk_2              RCC_AHB1Periph_GPIOE

//Motor_3
/*
Enable_Servo_3    PB13
EN_MOTOR3         PD12 
SERVO_READY_3     PB12
PULSE_3						PD9
DIRECTION_3				PD8
*/
#define Servo_Ready_Pin_3                   GPIO_Pin_12      //SERVO_READY_3     PB12          
#define Servo_Ready_Port_3                	GPIOB                    
#define Servo_Ready_Clk_3                 	RCC_AHB1Periph_GPIOB
#define Servo_In_Enable_Pin_3               GPIO_Pin_12      //EN_MOTOR3         PD12          
#define Servo_In_Enable_Port_3             	GPIOD                    
#define Servo_In_Enable_Clk_3               RCC_AHB1Periph_GPIOD
#define Servo_OUT_Enable_Pin_3              GPIO_Pin_13      //Enable_Servo_3    PB13          
#define Servo_OUT_Enable_Port_3            	GPIOB                    
#define Servo_OUT_Enable_Clk_3              RCC_AHB1Periph_GPIOB

//Motor_4
/*
Enable_Servo_4    PE1
EN_MOTOR4         PD13 
SERVO_READY_4     PE2
PULSE_4						PE3
DIRECTION_4				PE5
*/
#define Servo_Ready_Pin_4                   GPIO_Pin_2    // SERVO_READY_4     PE2           
#define Servo_Ready_Port_4              	  GPIOE                    
#define Servo_Ready_Clk_4               	  RCC_AHB1Periph_GPIOE
#define Servo_In_Enable_Pin_4               GPIO_Pin_13    // EN_MOTOR4         PD13            
#define Servo_In_Enable_Port_4             	GPIOD                    
#define Servo_In_Enable_Clk_4               RCC_AHB1Periph_GPIOD
#define Servo_OUT_Enable_Pin_4              GPIO_Pin_1    //  Enable_Servo_4    PE1          
#define Servo_OUT_Enable_Port_4            	GPIOE                    
#define Servo_OUT_Enable_Clk_4              RCC_AHB1Periph_GPIOE


//Motor_5
/*
Enable_Servo_5    PD3
EN_MOTOR5         PD14
SERVO_READY_5     PD7
PULSE_5						PD2
DIRECTION_5				PD4
*/
#define Servo_Ready_Pin_5                   GPIO_Pin_7      //SERVO_READY_5     PD7          
#define Servo_Ready_Port_5              	  GPIOD                    
#define Servo_Ready_Clk_5               	  RCC_AHB1Periph_GPIOD
#define Servo_In_Enable_Pin_5               GPIO_Pin_14       //EN_MOTOR5         PD14         
#define Servo_In_Enable_Port_5             	GPIOD                    
#define Servo_In_Enable_Clk_5               RCC_AHB1Periph_GPIOD
#define Servo_OUT_Enable_Pin_5              GPIO_Pin_3       //Enable_Servo_5    PD3        
#define Servo_OUT_Enable_Port_5            	GPIOD                    
#define Servo_OUT_Enable_Clk_5              RCC_AHB1Periph_GPIOD

//Motor_6
/*
Enable_Servo_6    PD0
EN_MOTOR6         PD15 
SERVO_READY_6     PA14
PULSE_6						PC8
DIRECTION_6				PA13
*/
#define Servo_Ready_Pin_6                   GPIO_Pin_14      //SERVO_READY_6     PA14           
#define Servo_Ready_Port_6              	  GPIOA                    
#define Servo_Ready_Clk_6               	  RCC_AHB1Periph_GPIOA
#define Servo_In_Enable_Pin_6               GPIO_Pin_15      //EN_MOTOR6         PD15          
#define Servo_In_Enable_Port_6             	GPIOD                    
#define Servo_In_Enable_Clk_6               RCC_AHB1Periph_GPIOD
#define Servo_OUT_Enable_Pin_6              GPIO_Pin_0      //Enable_Servo_6    PD0          
#define Servo_OUT_Enable_Port_6            	GPIOD                    
#define Servo_OUT_Enable_Clk_6              RCC_AHB1Periph_GPIOD

//GPIO 

#define Role_Pin             			  GPIO_Pin_1      // PC1             
#define Role_Port          				  GPIOC                    
#define Role_Clk            			  RCC_AHB1Periph_GPIOC
	

#define LED_OUT_Pin                 GPIO_Pin_4     //    PE4           
#define LED_OUT_Port            	  GPIOE                    
#define LED_OUT_Clk              	  RCC_AHB1Periph_GPIOE

#define Clear_Error_Pin             GPIO_Pin_2      // PB2              
#define Clear_Error_Port            GPIOB                    
#define Clear_Error_Clk             RCC_AHB1Periph_GPIOB


#define SW_SL0_Pin            		  GPIO_Pin_13      //        PC13         
#define SW_SL0_Port            			GPIOC                    
#define SW_SL0_Clk               		RCC_AHB1Periph_GPIOC

#define SW_SL1_Pin              		GPIO_Pin_15      //       PC15          
#define SW_SL1_Port         		   	GPIOC                    
#define SW_SL1_Clk           		    RCC_AHB1Periph_GPIOC






/*************************************************************************************************************************/


// Public Function 

extern void Driver_GPIO_Config(void);


#endif /* MY_GPIO_H_ */
