/*
 * Direct_Kinematics.c
 *
 *  Created on: May 3, 2016
 *      Author: AnhTuan
 */


#include "include.h"

/*Private Public ************************************/

//ver1.1
volatile  real_T d[7] = { 0, 0.45, 0, 0, -0.64, 0, 0.1 }; // d[0] not used onlly for synchoronous
volatile  real_T a[7] ={ 0, 0.15, 0.6, 0.2, 0, 0, 0 };  // a[0] not used
volatile  real_T Apha[7] = { 0, -PI/2, 0, PI/2, PI/2, PI/2, 0 }; // alpha[0] not used

////ver1.2
//volatile  real_T d[7] = { 0, 0.45, 0, 0, -0.64, 0, -0.095 }; // d[0] not used onlly for synchoronous
//volatile  real_T a[7] ={ 0, 0.15, 0.57, 0.13, 0, 0, 0 };  // a[0] not used
//volatile  real_T Apha[7] = { 0, PI/2, 0, -PI/2, PI/2, PI/2, -PI }; // alpha[0] not used

volatile real_T T_Matix[4][4];

/*Private variable ************************************/
static  real_T A1[4][4], A2[4][4], A3[4][4], A4[4][4], A5[4][4], A6[4][4];

static real_T q[7];

// T Matrix
void  DH_A1(real_T q,real_T temp[4][4])
{
	temp[0][0]=cos(q);   temp[0][1]= 0;  temp[0][2]= -sin(q) ;  temp[0][3]=a[1]*cos(q);
	temp[1][0]=sin(q);   temp[1][1]= 0;  temp[1][2]=  cos(q) ;  temp[1][3]=a[1]*sin(q);
	temp[2][0]=   0   ;  temp[2][1]=-1;  temp[2][2]=    0 ;     temp[2][3]=   d[1]     ;
	temp[3][0]=   0   ;  temp[3][1]= 0;  temp[3][2]=    0 ;     temp[3][3]=   1      ;
}

// A2 Matrix
void  DH_A2(real_T q,real_T temp[4][4])
{
	temp[0][0]=cos(q);   temp[0][1]= sin(q);  temp[0][2]=    0 ;     temp[0][3]=a[2]*cos(q);
	temp[1][0]=sin(q);   temp[1][1]=-cos(q);  temp[1][2]=    0 ;     temp[1][3]=a[2]*sin(q);
	temp[2][0]=   0   ;  temp[2][1]=-1;       temp[2][2]=    0 ;     temp[2][3]=  0     ;
	temp[3][0]=   0   ;  temp[3][1]= 0;       temp[3][2]=    0 ;     temp[3][3]=  1     ;
}
void DH_A(int i,real_T q,real_T temp[4][4])// i  is joint
{
	q=  q*PI/180.0 ;// Convert to radian
	temp[0][0]=cos(q);     temp[0][1]= -sin(q)*cos(Apha[i]);     temp[0][2]=  sin(q)*sin(Apha[i]) ;  temp[0][3]=a[i]*cos(q);
	temp[1][0]=sin(q);     temp[1][1]=  cos(q)*cos(Apha[i]);     temp[1][2]= -cos(q)*sin(Apha[i]) ;  temp[1][3]=a[i]*sin(q);
	temp[2][0]=   0.0  ;   temp[2][1]=  sin(Apha[i]);            temp[2][2]=  cos(Apha[i])  ;        temp[2][3]=   d[i]    ;
	temp[3][0]=   0.0  ;   temp[3][1]=   0.0   ;                 temp[3][2]=    0.0         ;        temp[3][3]=   1.0     ;
}


void Make_DK_Current_QEI(void)//Calculate Direct Kinematics at Current Position 
{
	real_T Q_Angle_DK[6];
	int i;
	for(i=0;i<6;i++)
	{
		Q_Angle_DK[i]=((real_T)(Get_QEI(i+1)))/((real_T)(Index_Gear[i]));
	}
	 Make_DK(Q_Angle_DK[0],Q_Angle_DK[1],Q_Angle_DK[2],Q_Angle_DK[3],Q_Angle_DK[4],Q_Angle_DK[5]);
}


void Make_DK(real_T q1,real_T q2, real_T q3, real_T q4, real_T q5 ,real_T q6)// calculator direct kinematics and display to Terminal UART_BASE_0
{
// Calculator each matrix Ai
	DH_A( 1, q1, A1 );
	//UART_Print_Matrix( USARTx,A1);
	
	DH_A( 2, q2, A2 );
	//UART_Print_Matrix( USARTx,A2);

	DH_A( 3, q3, A3 );
	//UART_Print_Matrix( USARTx,A3);

	DH_A( 4, q4, A4 );
	//UART_Print_Matrix( USARTx,A4);

	DH_A( 5, q5, A5 );
	//UART_Print_Matrix( USARTx,A5);

	DH_A( 6, q6, A6 );
	//UART_Print_Matrix( USARTx,A6);

//	UARTprint(USARTx," MULT MATRIX ");	
// mult matrix
	Mul_Matrix( A1, A2 );
//	UART_Print_Matrix( USARTx,A1);
	// A1=A1*A2

//	UARTprintf( "A1*A2*A3" );
	Mul_Matrix( A1, A3 );
//	UART_Print_Matrix( USARTx,A1);
	// A1=A1*A2*A3

//	UARTprintf( "A1*A2*A3*A4" );
	Mul_Matrix( A1, A4 );
//	UART_Print_Matrix( USARTx,A1);
//	UARTprintf( "A1*A2*A3*A4*A5" );
	Mul_Matrix( A1, A5 );
//	UART_Print_Matrix( USARTx,A1);

//	UARTprintf( "A1*A2*A3*A4*A5*A6" )
  UARTprint(USARTx," T MATRIX ");	
	Mul_Matrix( A1, A6 );
 UART_Print_Matrix( USARTx,A1);
	
	
	T_Matix[0][ 0] = A1[0][0];
	T_Matix[0][ 1] = A1[0][1];
	T_Matix[0][ 2] = A1[0][2];
	T_Matix[0][ 3] = A1[0][3];
	
	T_Matix[1][ 0] = A1[1][0];
	T_Matix[1][ 1] = A1[1][1];
	T_Matix[1][ 2] = A1[1][2];	
	T_Matix[1][ 3] = A1[1][3];
	
	T_Matix[2][ 0] = A1[2][0];  
	T_Matix[2][ 1] = A1[2][1];
	T_Matix[2][ 2] = A1[2][2];
	T_Matix[2][ 3] = A1[2][3];
	
	T_Matix[3][ 0] = A1[3][0];  
	T_Matix[3][ 1] = A1[3][1];
	T_Matix[3][ 2] = A1[3][2];
	T_Matix[3][ 3] = A1[3][3];
	
//	start_position[0]=T_Matix[0][3];
//	start_position[1]=T_Matix[1][3];
//	start_position[2]=T_Matix[2][3];	
//	
//	orientation[0][ 0] = T_Matix[0][0];
//	orientation[0][ 1] = T_Matix[0][1];
//	orientation[0][ 2] = T_Matix[0][2];
//	
//	orientation[1][ 0] = T_Matix[1][0];
//	orientation[1][ 1] = T_Matix[1][1];
//	orientation[1][ 2] = T_Matix[1][2];	
//	
//	orientation[2][ 0] = T_Matix[2][0];
//	orientation[2][ 1] = T_Matix[2][1];
//	orientation[2][ 2] = T_Matix[2][2];	
}
