

#ifndef TIMER_H_
#define TIMER_H_

/******************************************************************
 * PROTOTYPE
 *****************************************************************/
/*
PULSE_1						PA5
DIRECTION_1				PA4

PULSE_2						PE14
DIRECTION_2				PE12

PULSE_3						PD9
DIRECTION_3				PD8

PULSE_4						PE3
DIRECTION_4				PE5

PULSE_5						PD2
DIRECTION_5				PD4

PULSE_6						PC8
DIRECTION_6				PA13
*/
/*******************
*Config GPIO give Pulse
* to Driver 
*
*********************/


//Motor1
/*
PULSE_1						PA5
DIRECTION_1				PA4
*/
#define Servo_Pulse_Pin_1                   GPIO_Pin_5   //PULSE_1						PA5             
#define Servo_Pulse_Port_1              		GPIOA                    
#define Servo_Pulse_Clk_1               		RCC_AHB1Periph_GPIOA
#define Servo_Dir_Pin_1                     GPIO_Pin_4    //DIRECTION_1				PA4       
#define Servo_Dir_Port_1              	   	GPIOA                    
#define Servo_Dir_Clk_1               		  RCC_AHB1Periph_GPIOA

//Motor2
/*
PULSE_2						PE14
DIRECTION_2				PE12
*/
#define Servo_Pulse_Pin_2                   GPIO_Pin_14  //PULSE_2						PE14           
#define Servo_Pulse_Port_2              		GPIOE                    
#define Servo_Pulse_Clk_2              		  RCC_AHB1Periph_GPIOE
#define Servo_Dir_Pin_2                     GPIO_Pin_12   //DIRECTION_2				PE12             
#define Servo_Dir_Port_2              	   	GPIOE                    
#define Servo_Dir_Clk_2               		  RCC_AHB1Periph_GPIOE

//Motor3
/*
PULSE_3						PD9
DIRECTION_3				PD8
*/
#define Servo_Pulse_Pin_3                   GPIO_Pin_9  //PULSE_3						PD9            
#define Servo_Pulse_Port_3              		GPIOD                    
#define Servo_Pulse_Clk_3               		RCC_AHB1Periph_GPIOD
#define Servo_Dir_Pin_3                     GPIO_Pin_8   //DIRECTION_3				PD8             
#define Servo_Dir_Port_3              	   	GPIOD                    
#define Servo_Dir_Clk_3               		  RCC_AHB1Periph_GPIOD

//Motor4
/*
PULSE_4						PE3
DIRECTION_4				PE5
*/
#define Servo_Pulse_Pin_4                   GPIO_Pin_3   //PULSE_4						PE3            
#define Servo_Pulse_Port_4              		GPIOE                    
#define Servo_Pulse_Clk_4              		  RCC_AHB1Periph_GPIOE
#define Servo_Dir_Pin_4                     GPIO_Pin_5    //DIRECTION_4				PE5             
#define Servo_Dir_Port_4              	   	GPIOE                    
#define Servo_Dir_Clk_4               		  RCC_AHB1Periph_GPIOE


//Motor5
/*
PULSE_5						PD2
DIRECTION_5				PD4
*/
#define Servo_Pulse_Pin_5                   GPIO_Pin_2 //PULSE_5						PD2             
#define Servo_Pulse_Port_5              		GPIOD                    
#define Servo_Pulse_Clk_5               		RCC_AHB1Periph_GPIOD
#define Servo_Dir_Pin_5                     GPIO_Pin_4  //DIRECTION_5				PD4              
#define Servo_Dir_Port_5              	   	GPIOD                    
#define Servo_Dir_Clk_5               		  RCC_AHB1Periph_GPIOD

//Motor6
/*
PULSE_6						PC8
DIRECTION_6				PA13
*/
#define Servo_Pulse_Pin_6                   GPIO_Pin_8  //PULSE_6						PC8             
#define Servo_Pulse_Port_6              		GPIOC                    
#define Servo_Pulse_Clk_6               		RCC_AHB1Periph_GPIOC
#define Servo_Dir_Pin_6                     GPIO_Pin_13   //DIRECTION_6				PA13             
#define Servo_Dir_Port_6              	   	GPIOA                    
#define Servo_Dir_Clk_6               		  RCC_AHB1Periph_GPIOA

/*************************************************************************************************************************/
typedef enum {
	noaction = 0,
	acceleration,
	stable, 
	deceleration
} state_speed;



//Public variable 
extern volatile int32_t ServoPosTemp[6];

extern volatile int16_t pointer_openloop;
extern volatile int32_t Servo_Pluse_buffer[buffer_point][6];
extern volatile bool Servo_Flag[6];
extern volatile bool Flag_Robot_Run;
extern volatile int32_t ServoPulse[6];

// Public Function
extern int32_t Get_Qei_Speed(int joint );
extern void Config_Create_Pluse(void);
extern void Delay(__IO uint32_t nCount);
extern void delay_us(uint16_t period);
extern void delay_ms(uint16_t period);
extern void Display_Ram(void);

#endif /* TIMER_H_ */
