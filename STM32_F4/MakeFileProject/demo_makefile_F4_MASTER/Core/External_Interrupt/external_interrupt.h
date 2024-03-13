/*
 *  external_interrupt.h
 *
 *  Created on: May 29, 2016
 *  Author: AnhTuan
 */
 /*
 	 PE0 In_Possition 4
	 PD1 In_Possition 5
	 PC2 ERROR 
	 PC3 In_Possition 1
	 PB4 SENSOR 3
	 PB5 SENSOR 4
	 PE6 SENSOR 1
	 PE7 In_Possition 2
	 PB8 BUTTON
	 PB9 POWER CHECK 
	 PC10 In_Possition 6
	 PC11 SENSOR 6
	 PC12 SENSOR 5
	 PE13 In_Possition 3
	 PC14 SENSOR 2
	 */
 
 //Define In Posstion Signal
#define Servo_InPossition_Pin_1                   GPIO_Pin_3        //	 PC3 In_Possition 1     
#define Servo_InPossition_Port_1              	  GPIOC
#define Servo_InPossition_PinSource_1							EXTI_PinSource3
#define Servo_InPossition_PortSource_1						EXTI_PortSourceGPIOC

#define Servo_InPossition_Pin_2                   GPIO_Pin_7        //	 PE7 In_Possition 2       
#define Servo_InPossition_Port_2              	  GPIOE 
#define Servo_InPossition_PinSource_2							EXTI_PinSource7
#define Servo_InPossition_PortSource_2						EXTI_PortSourceGPIOE

#define Servo_InPossition_Pin_3                   GPIO_Pin_13        //	 PE13 In_Possition 3       
#define Servo_InPossition_Port_3              	  GPIOE
#define Servo_InPossition_PinSource_3							EXTI_PinSource13
#define Servo_InPossition_PortSource_3						EXTI_PortSourceGPIOE

#define Servo_InPossition_Pin_4                   GPIO_Pin_0        // 	 PE0 In_Possition 4    
#define Servo_InPossition_Port_4              	  GPIOE  
#define Servo_InPossition_PinSource_4							EXTI_PinSource0
#define Servo_InPossition_PortSource_4						EXTI_PortSourceGPIOE

#define Servo_InPossition_Pin_5                   GPIO_Pin_1        //	 PD1 In_Possition 5    
#define Servo_InPossition_Port_5              	  GPIOD 
#define Servo_InPossition_PinSource_5							EXTI_PinSource1
#define Servo_InPossition_PortSource_5						EXTI_PortSourceGPIOD

#define Servo_InPossition_Pin_6                   GPIO_Pin_10        //	 PC10 In_Possition 6     
#define Servo_InPossition_Port_6              	  GPIOC 
#define Servo_InPossition_PinSource_6							EXTI_PinSource10
#define Servo_InPossition_PortSource_6						EXTI_PortSourceGPIOC

//Define Sensor Limit 
#define Servo_SensorLimit_Pin_1                   GPIO_Pin_6        //	 PE6 SENSOR 1     
#define Servo_SensorLimit_Port_1              	  GPIOE  
#define Servo_SensorLimit_PinSource_1							EXTI_PinSource6
#define Servo_SensorLimit_PortSource_1						EXTI_PortSourceGPIOE

#define Servo_SensorLimit_Pin_2                   GPIO_Pin_14       //   PC14 SENSOR 2  
#define Servo_SensorLimit_Port_2              	  GPIOC
#define Servo_SensorLimit_PinSource_2							EXTI_PinSource14
#define Servo_SensorLimit_PortSource_2						EXTI_PortSourceGPIOC

#define Servo_SensorLimit_Pin_3                   GPIO_Pin_4        //	 PB4 SENSOR 3     
#define Servo_SensorLimit_Port_3              	  GPIOB 
#define Servo_SensorLimit_PinSource_3							EXTI_PinSource4
#define Servo_SensorLimit_PortSource_3						EXTI_PortSourceGPIOB

#define Servo_SensorLimit_Pin_4                   GPIO_Pin_5        //	 PB5 SENSOR 4     
#define Servo_SensorLimit_Port_4              	  GPIOB
#define Servo_SensorLimit_PinSource_4							EXTI_PinSource5
#define Servo_SensorLimit_PortSource_4						EXTI_PortSourceGPIOB

#define Servo_SensorLimit_Pin_5                   GPIO_Pin_12       // 	 PC12 SENSOR 5    
#define Servo_SensorLimit_Port_5              	  GPIOC
#define Servo_SensorLimit_PinSource_5							EXTI_PinSource12
#define Servo_SensorLimit_PortSource_5						EXTI_PortSourceGPIOC

#define Servo_SensorLimit_Pin_6                   GPIO_Pin_11       //	 PC11 SENSOR 6      
#define Servo_SensorLimit_Port_6              	  GPIOC
#define Servo_SensorLimit_PinSource_6							EXTI_PinSource11
#define Servo_SensorLimit_PortSource_6						EXTI_PortSourceGPIOC


#define Servo_BUTTON_Pin                     	    GPIO_Pin_8        //	  PB8 BUTTON 
#define Servo_BUTTON_Port              	          GPIOB 
#define Servo_BUTTON_PinSource					      		EXTI_PinSource8
#define Servo_BUTTON_PortSource						        EXTI_PortSourceGPIOB

#define Servo_POWER_CHECK_Pin                     GPIO_Pin_9        //	PB9 POWER CHECK     
#define Servo_POWER_CHECK_Port                	  GPIOB
#define Servo_POWER_CHECK_PinSource					    	EXTI_PinSource9
#define Servo_POWER_CHECK_PortSource						  EXTI_PortSourceGPIOB


#define Servo_ERROR_Pin                           GPIO_Pin_2        //	 PC2 ERROR     
#define Servo_ERROR_Port                	        GPIOC
#define Servo_ERROR_PinSource					    	      EXTI_PinSource2
#define Servo_ERROR_PortSource						        EXTI_PortSourceGPIOC




extern void Driver_EXTI_Config(void);
extern void EXTILine3_Config(void);



