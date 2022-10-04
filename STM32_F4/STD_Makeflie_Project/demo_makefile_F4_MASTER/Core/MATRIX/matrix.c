#include "include.h"

void Mul_Matrix(real_T a[4][4] , real_T b[4][4] ) // A =A*B
{

	int i, j, k = 0;
	real_T temp[4][4];
	for ( i = 0; i < 4 ; i++ )
	{
		for ( j = 0; j < 4 ; j++ )
		{
			temp[i][j] = a[i][j]; // save value to temp variable
			a[i][j] = 0; // reset value of matrix mult
		}
	}

//	UARTCharPut( UART0_BASE, '\n' );
//	UART_Print_Matrix( a );
//	UARTCharPut( UART0_BASE, '\n' );
//	mult[4][4]={{0, 0, 0 ,0},{0, 0, 0 ,0},{0, 0, 0 ,0},{0, 0, 0 ,0}};// phai reset bo nho ko se co gia tri bat dinh
//	float a[4][4]={{1.5, 2.5, 3 ,4},{ 5, 6, 7, 8},{9, 10, 11, 12},{13, 14, 15, 16}};
//	float b[4][4]={{1, 2, 3 ,4},{ 5, 6, 7, 8},{9, 10, 11, 12},{13, 14, 15, 16}};

	for ( i = 0; i < 4 ; ++i )
		for ( j = 0; j < 4 ; ++j )
			for ( k = 0; k < 4 ; ++k )
			{
				a[i][j] += temp[i][k] * b[k][j];
			}
}

