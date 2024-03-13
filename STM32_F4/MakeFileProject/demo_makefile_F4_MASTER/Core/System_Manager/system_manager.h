/*
 *  system_manager.h
 *
 *  Created on: May 23, 2016
 *  Author: AnhTuan
 */
 
#ifndef _SYSTEM_MANAGER_H_
#define  _SYSTEM_MANAGER_H_



typedef enum
{
	initial_system = 0,
	read_enable = 1,
	enable_servo = 2,
	waiting_servo_ready = 3,
	check_servo_ready = 4,
	clear_error = 5,
	error = 6,
	initial_success = 7,
	detect_possition = 8,
	open_loop = 9,
	inverse_kinematics = 10,
	home=11,
	waitting
} system_state;


extern system_state system_GetState(void);
extern void system_SetState(system_state SysState);
extern void System_Initial(void);
extern void Read_Enable(void);
extern void Clear_Error(void);
extern void Enable_Servo(void);
extern void Waiting_Servo_Ready(void);
extern void Check_Servo_Ready(void);
extern void Initial_Success(void);
extern void Waitting(void);
extern void Error();
extern void Detect_position(void);
extern void Home(void);
extern void Open_loop(void);
extern void Inverse_Kinematic(void);
extern void Display_QEI(void);

extern void System_Error( char* error);
//Public Variable 
extern volatile uint32_t user_pointer;
extern volatile  bool QEI_Display;
extern volatile bool servo_Enable_Flag[6];


#endif /*_SYSTEM_MANAGER_H_*/
