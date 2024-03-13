/*
 *  Inverser_Kinematics.h
 *
 *  Created on: May 29, 2016
 *  Author: AnhTuan
 */

extern volatile double Robot_Percent_Speed;
extern volatile bool Flag_Slave_Receive_Finish;
extern volatile bool Flag_Slave_Busy;
extern void  Inverser_Kinematics(real_T q[7]  ,real_T T[4][4]);
extern void  Inverser_Kinematics_2(real_T q[7]  ,real_T T[4][4]);
extern void invert_cutted_9();
extern void Planing_Path();
extern void Planing_Path_Free(int Delta_Q_Move[6],uint32_t Speed);
extern void Planing_Path_Strain_Line_Tracking(double a,double b,double c);
extern  void UART_Start_Send_To_Slave(USART_TypeDef* UART_Base);
extern  void UART_Finish_Send_To_Slave(USART_TypeDef* UART_Base);
extern void Move_Through_Tracking(uint16_t A,uint16_t B);

extern volatile double start_position[3];
extern volatile double end_position[3];
extern volatile double Q_angle_old[6];
extern volatile double Index_Gear[6];
extern volatile double orientation[3][3];
extern volatile double low_limit[6];
extern volatile double High_limit[6];