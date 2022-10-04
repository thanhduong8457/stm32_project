/*
 *  Inverser_Kinematics.c
 *
 *  Created on: May 29, 2016
 *  Author: AnhTuan
 */
#include "include.h"

/**************************/


double Time_dij[10];
double Acc_move_through[10][6];
double Acc_move_through_ij[10][6];
double Time_i[10][6];
double Time_ij[10][6];
double Velocity_max[10][6];
int Delta_Q_Move_Through[10][6];
double Acc_move_through_global[6]={0.0,0.0,0.0,0.0,0.0,0};
/*************************/


		 /*System parameter */
		 volatile double Robot_Percent_Speed=1.0;// Speed of Robot User will setting Speed Range (0,100%)
     /* Parameter for Planing Path Fee */
		 volatile double Jmax_Robot[6]={100000.,100000.,100000.,100000.,100000.,100000.};// Pulse
		 /**********************************/
        volatile bool Flag_Slave_Receive_Finish=true; 
		    volatile bool Flag_Slave_Busy=false;
				static real_T Q[7];// Q[0] not used
				double NaN;
				char Arr_Time_Step[sizeof(double)]; // array for convert from double to array and tranmist to Slave
				/* Parameter for Find out Planing Path */
				static double amax, Tma, Tmb, Td,T,time,delta_T;
				bool Temp_Velocity_Integra = false;
				double Jmax;// Angle
				/* Parameter for Find out Pass Solution */
				volatile double Index_Gear[6]= {3600.0,3600.0,3600.0,3600.0,3600.0,3600.0};
				volatile double low_limit[6]= {-170.0,-150.0,-190.0,-180.5,-135.0 ,-360.0 };
        volatile double High_limit[6]= { 170.0,  65.0, 170.0, 180.5, 135.0 ,360.0 };
				volatile double Q_angle_old[6]={ 50.0, -44.0, -38.0, -180.0, 83.0, 50.0 };
				volatile double Q_angle[6];
			  static int32_t Delta_Q_angle[6]={0,0,0,0,0,0};
				volatile double Solution [6][8];				
				volatile double detal_L = 0.002;//0.001 m
		
				volatile double start_position[3]= { 0.79, 0, 0.9 };
        volatile double end_position[3]= { 0.79, -0.95, 0.9 };
			  volatile double position[3] ;
				volatile double orientation[3][3];

        /* Invert Function Parameter */
        volatile double num1 = 0.0;
        volatile double num2 = 0.0;
        volatile double num3 = 0.0;
        volatile double num4 = 0.0;
        volatile double num5 = 0.0;
        volatile double num6 = 0.0;
        volatile double num7 = 0.0;
        volatile double num8 = 0.0;
				//length of offset and convert to mm 
        volatile double length_d1 = 450.0;
        volatile double length_d2 = 0.0;
        volatile double length_d3 = 0.0;
        volatile double length_d4 = -640.0;
        volatile double length_d6 = 100;
        //length of a1,a2
        volatile double x1 = 150.0;// x1=length a1
        volatile double x2 = 600.0;//a2
        volatile double x3 = 200.0;//a3
        volatile double lenght_a6 = 0.0;//length a6
        volatile double radian_unit = PI / 180.0;// convert to radian
        volatile double degree_unit = 180.0 / PI;// convert to degree
        volatile double angle_alpha1 = PI / 2;// convert to radian alpha 1...6.cos alpha1 ERROR when replace 
        // calculation value cos() and sin() of alpha 
        volatile double sin_alpha [6]={-1.0,0, 1.0, 1.0,1.0, 0};// sin_alpha value of sin(alpha)
        volatile double numArray2[6] ={ 0,1.0,0,0,0,1.0 };// numArray2 value of cos(alpha)
        //// value position of end effector
        volatile double px, py, pz;
				static double x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20, x21, x22, x23;
				static double num41, num42, num43, num44, num45, num46, num47, num48, num49, num50, num51, num52, num53, num54, num55, num56,
                      num57, num58, num59, num60, num61, num62, num63, num64, num65, num66, num67, num68, num69, num70, num71,num72, num73,
                      num74, num75, num76, num77, num78, num79, num80, num81, num82, num83, num84, num85, num86,num87;
        volatile double x24, x25, x26, x27, x28, x29, x30, x31, x32, x33;
        volatile double d0,d1, d2, d3, d4;
        volatile double  y1, y2, y3, y4;
				volatile double numArray4[4];
				volatile double numArray5[4];
				volatile double numArray6[4];
				volatile double numArray7[4];
				volatile double numArray8[4];
				volatile double matrix_17[2];
				volatile double matrix_18[2];
				volatile double matrix_19[2];
				volatile double matrix_20[2];
				volatile double matrix_21[2];
				volatile double matrix_22[2];
				volatile double position_matix [4][3];
				volatile double orientation_matrix[4][3][2];
				volatile double matrix_11[4][ 3][ 3];
				volatile double matrix_12[4][ 3][ 3];
				volatile double matrix_13[4][ 3][ 3];
				volatile double matrix_14[4][ 3][ 3];
				volatile double matrix_15[4][ 3][ 3];
				volatile double matrix_16[4][ 3][ 3];
				int index0, index1, index2, index3, index4;
				
				
int sign(int x) 
{    
    return (x>>31)|(!!x);
} 

//int sign(int x) 
//{    
//  if(x>0) return 1;
//	if(x<0) return -1;
//	if(x==0) return 0; // error 	if(x==0) return 0;
//} 				
				
				
				
double Cube_Solver(double a,double b,double c,double d)
        {
            double x1=0, x2=0, x3=0;
            double xpass;
            double disc, q, r, dum1, s, t, term1, r13;
            b /= a;
            c /= a;
            d /= a;

            q = (3.0 * c - (b * b)) / 9.0;
            r = -(27.0 * d) + b * (9.0 * c - 2.0 * (b * b));
            r /= 54.0;
            disc = q * q * q + r * r;
            term1 = (b / 3.0);

            if (disc > 0)
            { // one root real, two are complex
                s = r + sqrt(disc);
                s = ((s < 0) ? -pow(-s, (1.0 / 3.0)) : pow(s, (1.0 / 3.0)));
                t = r - sqrt(disc);
                t = ((t < 0) ? -pow(-t, (1.0 / 3.0)) : pow(t, (1.0 / 3.0)));
                x1 = (-term1 + s + t);
                if (x1 <= 0)
                {
					// Error
								UARTprint(USARTx,"ERROR Cube_Solver 1 ");
                while (true) ;
                }
                else
                {
                    xpass = x1;
                }

            }
            else if (disc == 0)
            {
                // The remaining options are all real
                //  x2i = x3i = 0;
                // All roots real, at least two are equal.
                r13 = ((r < 0) ? -pow(-r, (1.0 / 3.0)) : pow(r, (1.0 / 3.0)));
                x1 = (-term1 + 2.0 * r13);
                x2 = x3 = -(r13 + term1);
                if ((x1 > 0) & (x2 > 0))
                {
                    if (x1 >= x2) // choose x smaller
                        xpass = x2;
                    else
                        xpass = x1;
                }
                else if (x1 > 0)
                {
                    xpass = x1;
                }
                else if (x2 > 0)
                {
                    xpass = x2;
                }
                else
                {
                    // error
									 UARTprint(USARTx,"ERROR Cube_Solver 2 ");
                    while (true) ;
                }
                // End if (disc == 0)
            }
            else
            {
                // Only option left is that all roots are real and unequal (to get here, q < 0)
                q = -q;
                dum1 = q * q * q;
                dum1 = acos(r / sqrt(dum1));
                r13 = 2.0 * sqrt(q);
                x1 = (-term1 + r13 * cos(dum1 / 3.0));
                x2 = (-term1 + r13 * cos((dum1 + 2.0 * PI) / 3.0));
                x3 = (-term1 + r13 * cos((dum1 + 4.0 * PI) / 3.0));
                if ((x1 > 0) & (x2 > 0) & (x3 > 0))// 3 Positive
                {
                    if ((x1 <= x2) & (x1 <= x3))
                    {
                        xpass = x1;
                    }
                    else if ((x2 <= x1) & (x2 <= x3))
                    {
                        xpass = x2;
                    }
                    else
                    {
                        xpass = x3;
                    }
                }
                // 2 Positive
                else if ((x1 > 0) & (x2 > 0))
                {
                    if (x1 >= x2) // choose x smaller
                        xpass = x2;
                    else
                        xpass = x1;
                }
                else if ((x1 > 0) & (x3 > 0))
                {
                    if (x1 >= x3) // choose x smaller
                        xpass = x3;
                    else
                        xpass = x1;
                }
                else if ((x2 > 0) & (x3 > 0))
                {
                    if (x2 >= x3) // choose x smaller
                        xpass = x3;
                    else
                        xpass = x2;
                }
                // 1 Positive
                else if (x1 > 0)
                {
                    xpass = x1;
                }
                else if (x2 > 0)
                {
                    xpass = x2;
                }
                else if (x3 > 0)
                {
                    xpass = x3;
                }
                else
                {
                    //error
									  UARTprint(USARTx,"ERROR Cube_Solver 3 ");
                    while (true) ;
                }
            }       
            return xpass;
   }

double Velocity_intergral_2( double t )
        {
            double a, b, c, d,alpha, deltat=0.0;
            alpha = T / 4.0;
            if (t< alpha)
            {
                a = 1.0;
                b = 3.0 * t;
                c = 3.0 * t * t;
                d = -6.0* detal_L / Jmax;
							// UARTprint(USARTx,"Velocity 1 ");
                deltat = Cube_Solver(a, b, c, d);
            }
            else if ((alpha <= t) & (t < 3* alpha))
            {
              //  UARTprint(USARTx,"Velocity 2 3 ");							
                a = 1.0 / 6.0;
                b = t / 2.0 - alpha;
                c = t * t / 2.0 + alpha * alpha - 2.0 * alpha * t;
                d = detal_L / Jmax;
                deltat = Cube_Solver(a, b, c, d);
            }
            else
            {
							// UARTprint(USARTx,"Velocity 4 ");
                a = 1.0 / 6.0;
                b = t / 2.0 - 2.0*alpha;
                c = t * t / 2.0 + 8.0*alpha * alpha - 4.0 * alpha * t;
                d = -detal_L / Jmax;
                deltat = Cube_Solver(a, b, c, d);
            }
            return deltat;
        }
double Velocity_integral( double t)
        {
            double detal_T_temp = 0;
            if (Temp_Velocity_Integra == false)
            {
                if ((0.0 <= t) & (t < Tma))
                {
                    detal_T_temp = -t + sqrt(t * t + 2 * detal_L / amax);
                }
                else if ((Tma <= t) & (t < (Tma + Td+Tmb)))
                {
                    detal_T_temp = detal_L / (amax * Tma);              
                }
                else if (((Tma + Td) <= t) & (t <= (Tma + Td + Tmb)))
                {
                    Temp_Velocity_Integra = true;
                }
            }
            else
            {

            }
            return detal_T_temp;
        }
				
void UART_Send_To_Slave(USART_TypeDef* UART_Base,int32_t Delta_Q[],double time)
{
	int i,j=0;
	int temp;
	aTxBuffer[j++]=(uint8_t)(100); // Detect package byte 
	/* Delta Time */
	memcpy(&Arr_Time_Step,&time,sizeof(time));
	aTxBuffer[j++]=Arr_Time_Step[0];
	aTxBuffer[j++]=Arr_Time_Step[1];
	aTxBuffer[j++]=Arr_Time_Step[2];
	aTxBuffer[j++]=Arr_Time_Step[3];
	aTxBuffer[j++]=Arr_Time_Step[4];
	aTxBuffer[j++]=Arr_Time_Step[5];
	aTxBuffer[j++]=Arr_Time_Step[6];
	aTxBuffer[j++]=Arr_Time_Step[7];
	/* Delta Angle */
	temp=Delta_Q[0];
  aTxBuffer[j++]=(uint8_t)(temp>>24);
	aTxBuffer[j++]=(uint8_t)(temp>>16);
	aTxBuffer[j++]=(uint8_t)(temp>>8);
	aTxBuffer[j++]=(uint8_t)temp;
	temp=Delta_Q[1] ;
	aTxBuffer[j++]=(uint8_t)(temp>>24);
	aTxBuffer[j++]=(uint8_t)(temp>>16);
	aTxBuffer[j++]=(uint8_t)(temp>>8);
	aTxBuffer[j++]=(uint8_t) temp;
	temp=Delta_Q[2] ;
	aTxBuffer[j++]=(uint8_t)(temp>>24);
	aTxBuffer[j++]=(uint8_t)(temp>>16);
	aTxBuffer[j++]=(uint8_t)(temp>>8);
	aTxBuffer[j++]=(uint8_t)temp;
	temp=Delta_Q[3] ;
	aTxBuffer[j++]=(uint8_t)(temp>>24);
	aTxBuffer[j++]=(uint8_t)(temp>>16);
	aTxBuffer[j++]=(uint8_t)(temp>>8);
	aTxBuffer[j++]=(uint8_t)temp;
	temp=Delta_Q[4] ;
	aTxBuffer[j++]=(uint8_t)(temp>>24);
	aTxBuffer[j++]=(uint8_t)(temp>>16);
	aTxBuffer[j++]=(uint8_t)(temp>>8);
	aTxBuffer[j++]=(uint8_t)temp;
	temp=Delta_Q[5] ;
	aTxBuffer[j++]=(uint8_t)(temp>>24);
	aTxBuffer[j++]=(uint8_t)(temp>>16);
	aTxBuffer[j++]=(uint8_t)(temp>>8);
	aTxBuffer[j++]=(uint8_t)temp;									
	for(i=0;i< SPI_BUFFERSIZE;i++)// Use SPI TX buffer is to save memory
	{
		USART_SendData(UART_Base,aTxBuffer[i]);
		while (USART_GetFlagStatus(UART_Base, USART_FLAG_TXE) == RESET);
	}
}

void SPI_Send_To_Slave(int32_t Delta_Q[])
{
	int i,j=0;
	int temp;
	temp=Delta_Q[0];
  aTxBuffer[j++]=(uint8_t)(temp>>24);
	aTxBuffer[j++]=(uint8_t)(temp>>16);
	aTxBuffer[j++]=(uint8_t)(temp>>8);
	aTxBuffer[j++]=(uint8_t)temp;
	temp=Delta_Q[1] ;
	aTxBuffer[j++]=(uint8_t)(temp>>24);
	aTxBuffer[j++]=(uint8_t)(temp>>16);
	aTxBuffer[j++]=(uint8_t)(temp>>8);
	aTxBuffer[j++]=(uint8_t) temp;
	temp=Delta_Q[2] ;
	aTxBuffer[j++]=(uint8_t)(temp>>24);
	aTxBuffer[j++]=(uint8_t)(temp>>16);
	aTxBuffer[j++]=(uint8_t)(temp>>8);
	aTxBuffer[j++]=(uint8_t)temp;
	temp=Delta_Q[3] ;
	aTxBuffer[j++]=(uint8_t)(temp>>24);
	aTxBuffer[j++]=(uint8_t)(temp>>16);
	aTxBuffer[j++]=(uint8_t)(temp>>8);
	aTxBuffer[j++]=(uint8_t)temp;
	temp=Delta_Q[4] ;
	aTxBuffer[j++]=(uint8_t)(temp>>24);
	aTxBuffer[j++]=(uint8_t)(temp>>16);
	aTxBuffer[j++]=(uint8_t)(temp>>8);
	aTxBuffer[j++]=(uint8_t)temp;
	temp=Delta_Q[5] ;
	aTxBuffer[j++]=(uint8_t)(temp>>24);
	aTxBuffer[j++]=(uint8_t)(temp>>16);
	aTxBuffer[j++]=(uint8_t)(temp>>8);
	aTxBuffer[j++]=(uint8_t)temp;									
	/* Delta Time */
	delta_T=delta_T*1000000; // Convert to us 
	temp=(int)(delta_T);
	aTxBuffer[j++]=(uint8_t)(temp>>24);
	aTxBuffer[j++]=(uint8_t)(temp>>16);
	aTxBuffer[j++]=(uint8_t)(temp>>8);
	aTxBuffer[j++]=(uint8_t)temp;
	Send_To_STM32_Slave();

}
 void UART_Start_Send_To_Slave(USART_TypeDef* UART_Base)
 {
	 USART_SendData(UART_Base,(uint16_t)140);
	while (USART_GetFlagStatus(UART_Base, USART_FLAG_TXE) == RESET);
	 
 }
  void UART_Finish_Send_To_Slave(USART_TypeDef* UART_Base)
 {
	 USART_SendData(UART_Base,(uint16_t)150);
	while (USART_GetFlagStatus(UART_Base, USART_FLAG_TXE) == RESET);
	 
 }

void Planing_Path()
{
				int i;
	      double a, b, c;
        double L;
        int total_num;
        int li;	
            a = end_position[0] - start_position[0];//  m unit 
            b = end_position[1] - start_position[1];//  m unit
            c = end_position[2] - start_position[2];//  m unit
            L = sqrt(a * a + b * b + c * c);
            total_num = (int)( L/ detal_L + 0.5);						
						UARTPutnlong(USARTx,total_num);						
						li=0;		
						time = 0;						
						/* velocity paramater */
            Jmax = 0.1;
						T = 4 * pow((L / (2 * Jmax)), 1.0 / 3.0);
  Flag_Slave_Receive_Finish=true;
	UARTprint(USARTx,"Total Time**");	
	UART_Print_Double(USARTx,T);
	USART_SendData(USARTx,(uint16_t)10); 
	while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
	UARTprint(USARTx,"Start Position**");	
	UART_Print_Double(USARTx,start_position[0]);
	USART_SendData(USARTx,(uint16_t)' '); 
	while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
		UART_Print_Double(USARTx,start_position[1]);
	USART_SendData(USARTx,(uint16_t)' '); 
	while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
		UART_Print_Double(USARTx,start_position[2]);
	USART_SendData(USARTx,(uint16_t)10); 
	while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
	
	// Start Save Send Data to slave
	USART_SendData(USARTx1,(uint16_t)140); 
	while (USART_GetFlagStatus(USARTx1, USART_FLAG_TXE) == RESET);								
		       while (li <= total_num)
            {
							if(Flag_Slave_Receive_Finish== true)
							{
                //calulator next step in fix distance ;
                position[0] = start_position[0] + a * (double)((double)li / (double)total_num);
                position[1] = start_position[1] + b * (double)((double)li / (double)total_num);
                position[2] = start_position[2] + c * (double)((double)li / (double)total_num);
								li++;
                // invert and find out the angle for next step 
                invert_cutted_9();
								// Find out Delta Time								
								delta_T =Velocity_intergral_2(time);
								time = time + delta_T;
                for( i=0; i<6;i++)
                {												
								  Delta_Q_angle[i] = (int )((Q_angle[i] - Q_angle_old[i])*Index_Gear[i]) ;
									if(abs(Delta_Q_angle[i])<4) // neu di chuyen qua nho thi  de cho di chuyen ke tiep
									{
										Delta_Q_angle[i]=0;
									}
									else
									{
										// Cong don nhung luong di chuyen nho de tranh lam anh huong den nhung khop khac 
								   Q_angle_old[i] = Q_angle[i];	// Canh bao cong don
									}
                }						
								/* Send To slave */
								UART_Send_To_Slave(USARTx1, Delta_Q_angle,delta_T);	
								Flag_Slave_Receive_Finish=false;							
							}						
            }
													
			// Start Creat Pulse
			UARTprint(USARTx,"START MOVING**");	
			USART_SendData(USARTx1,(uint16_t)150); 
			while (USART_GetFlagStatus(USARTx1, USART_FLAG_TXE) == RESET);	
		 /**********************************/				
	UARTprint(USARTx,"End Position**");						
	UART_Print_Double(USARTx,position[0]);
	USART_SendData(USARTx,(uint16_t)' '); 
	while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
		UART_Print_Double(USARTx,position[1]);
	USART_SendData(USARTx,(uint16_t)' '); 
	while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
		UART_Print_Double(USARTx,position[2]);
	USART_SendData(USARTx,(uint16_t)10); 
	while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
						
}


void Planing_Path_Strain_Line_Tracking(double a,double b,double c)
{
				int i;
        double L;
        int total_num;
        int li;	
	Make_DK_Current_QEI();// Determine Current Position In Euler coordinate 
	
	start_position[0]=T_Matix[0][3];
	start_position[1]=T_Matix[1][3];
	start_position[2]=T_Matix[2][3];	
	
	orientation[0][ 0] = T_Matix[0][0];
	orientation[0][ 1] = T_Matix[0][1];
	orientation[0][ 2] = T_Matix[0][2];	
	orientation[1][ 0] = T_Matix[1][0];
	orientation[1][ 1] = T_Matix[1][1];
	orientation[1][ 2] = T_Matix[1][2];	
	orientation[2][ 0] = T_Matix[2][0];
	orientation[2][ 1] = T_Matix[2][1];
	orientation[2][ 2] = T_Matix[2][2];
	/*********************/
	Flag_Slave_Busy=true;
	
		for(i=0;i<6;i++)
		{
				Q_angle_old[i]=(double)(Get_QEI(i+1)/Index_Gear[i]);
		}
		L = sqrt(a * a + b * b + c * c);
		total_num = (int)( L/ detal_L + 0.5);			
    UARTprint(USARTx,"Total Num**");			
		UARTPutnlong(USARTx,total_num);						
		li=0;		
		time = 0;						
		/* velocity paramater */
		Jmax = 0.1;
		T = 4 * pow((L / (2 * Jmax)), 1.0 / 3.0);
		
  Flag_Slave_Receive_Finish=true;
	Print(USARTx,"Total Time**");	
	UART_Print_Double(USARTx,T);
	USART_SendData(USARTx,(uint16_t)10); 
	while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
		
	UARTprint(USARTx,"a b c**");	
	UART_Print_Double(USARTx,a);
	USART_SendData(USARTx,(uint16_t)' '); 
	while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
	UART_Print_Double(USARTx,b);
	USART_SendData(USARTx,(uint16_t)' '); 
	while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
	UART_Print_Double(USARTx,c);
	USART_SendData(USARTx,(uint16_t)10); 
	while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);	

	
 /* Start Save Send Data to slave*/
	USART_SendData(USARTx1,(uint16_t)140); 
	while (USART_GetFlagStatus(USARTx1, USART_FLAG_TXE) == RESET);	
/*****************************************/	
		       while (li < total_num) // li from 1 to Totalnum
            {
							if(Flag_Slave_Receive_Finish== true)
							{
								li++;	
                //calulator next step in fix distance ;
                position[0] = start_position[0] + a * (double)((double)li / (double)total_num);
                position[1] = start_position[1] + b * (double)((double)li / (double)total_num);
                position[2] = start_position[2] + c * (double)((double)li / (double)total_num);																
                // invert and find out the angle for next step 
                invert_cutted_9();
								// find time arcoding to velocity Scuve
								delta_T =Velocity_intergral_2(time);							
								time = time + delta_T;
                for( i=0; i<6;i++)
                {												
								  Delta_Q_angle[i] = (int )((Q_angle[i] - Q_angle_old[i])*Index_Gear[i]) ;

										UART_Print_Double(USARTx,Q_angle[i]);
	                  USART_SendData(USARTx,(uint16_t)' '); 
	                  while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
																	
									if(abs(Delta_Q_angle[i])<10) // neu di chuyen qua nho thi  chuyen luong di chuyen nho do  cho di chuyen ke tiep
									{
										Delta_Q_angle[i]=0;
									}
									else
									{
										// Cong don nhung luong di chuyen nho de tranh lam anh huong den nhung khop khac 
								   Q_angle_old[i] = Q_angle[i];	// Canh bao cong don
									}
                }
													
	                  USART_SendData(USARTx,(uint16_t)10); 
	                  while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
								/* Send To slave */
								UART_Send_To_Slave(USARTx1, Delta_Q_angle,delta_T);	
								Flag_Slave_Receive_Finish=false;									
							}
            }													
	// Start Creat Pulse													
			UARTprint(USARTx,"Robot Invert Start Moving **");	
			USART_SendData(USARTx1,(uint16_t)150); 
			while (USART_GetFlagStatus(USARTx1, USART_FLAG_TXE) == RESET);	

	while(Flag_Slave_Busy==true);
	/**********************************/							
	Print(USARTx,"End Position **");						
  Make_DK_Current_QEI();// Determine Current Position In Euler coordinate 
	// For simulation 
//		for (i=0;i<6;i++)
//		{
//			Set_QEI(i+1, (int)(Q_angle_old[i]*Index_Gear[i]));
//		}								
}

void Planing_Path_Free(int Delta_Q_Move[6],uint32_t Speed)
{
	double total_pulse[6]={0,0,0,0,0,0};
	double jmax[6]; // jmax temp that calculate other Jmax depend on max time moving
	int i;
	double alpha=0; // =T/4
	double temp_alpha=0;// that find max time moving
	double time_sample =0.05;// second unit
	double Time,t_current=0.0;
	int total_num;
	double Delta_Q_Temp;
	Flag_Slave_Receive_Finish=false;
	UARTprint(USARTx,"PULSE");
	for (i=0;i<6;i++) // Find max time moving 
	{
	  UARTPutnlong(USARTx,Delta_Q_Move[i]);
	  USART_SendData(USARTx,(uint16_t)' '); 
		while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
	}
		USART_SendData(USARTx,(uint16_t)10); 
	  while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);	
	//********Setting Robot Speed	 
	 Robot_Percent_Speed= (double)Speed/100.0;
	if((abs(Delta_Q_Move[0])<30)&(abs(Delta_Q_Move[1])<30)&(abs(Delta_Q_Move[2])<30)
		&(abs(Delta_Q_Move[3])<30)&(abs(Delta_Q_Move[4])<30)&(abs(Delta_Q_Move[5])<30))//Bypass VALUE <30 PULSE.Avoid pulse=1,2 will create ERROR
	{
		UARTprint(USARTx,"PULSE EMTY");	
		return;			
	}
	Flag_Slave_Busy=true;	
	for (i=0;i<6;i++) // Find max time moving 
	{
		temp_alpha=(1.0/Robot_Percent_Speed)* pow(abs((double)Delta_Q_Move[i])/(2.0*Jmax_Robot[i]) ,1.0/3.0); //T =4*(L/(2*Jmax))^(1/3)
		if(temp_alpha>alpha)
		alpha=temp_alpha;
   // Reset Q_old 
    Q_angle_old[i]=0;
	}
	Time =4.0*alpha;
	for(i=0;i<6;i++)// REcaculate jmax for approriate Time
	{
		jmax[i]=(double)Delta_Q_Move[i]/(2*alpha*alpha*alpha);		//S=2*Jmax*alpha^3
	}
	total_num= (int)(4.0*alpha/time_sample);
	UARTprint(USARTx,"Num & Time & Speed %");	
	UARTPutnlong(USARTx,total_num);
	USART_SendData(USARTx,(uint16_t)' '); 
	while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
	UART_Print_Double(USARTx,Time);
	USART_SendData(USARTx,(uint16_t)' '); 
	while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
	UART_Print_Double(USARTx,Robot_Percent_Speed*100);
	USART_SendData(USARTx,(uint16_t)10); 
	while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
 // Start Save Send Data to slave
  UART_Start_Send_To_Slave(USARTx1);
 /******************************/
		  while (total_num>0)
           {					
						if(Flag_Slave_Receive_Finish== true)
							{
								t_current+=time_sample;
								total_num--;							
								for (i=0;i<6;i++)
								{
										if(t_current<=alpha)
											 {
												 Q_angle[i]=jmax[i]*t_current*t_current*t_current/6.0;			 
											 }
											 else if((alpha<t_current)&(t_current<=2.0*alpha))
											 {
												 Q_angle[i]=jmax[i]*(alpha*alpha*alpha/6.0 + alpha*alpha*(t_current-alpha)/2.0 );// Avoid program creat more Temp variable
												 Q_angle[i]=Q_angle[i] + jmax[i]*(alpha*(t_current-alpha)*(t_current-alpha)/2.0);
												 Q_angle[i]=Q_angle[i] - jmax[i]*((t_current-alpha)*(t_current-alpha)*(t_current-alpha)/6.0);
											 }
											 else if((alpha<2.0*t_current)&(t_current<=3.0*alpha))
											 {
												 Q_angle[i] =jmax[i]*(alpha*alpha*alpha + alpha*alpha*(t_current-2.0*alpha)) ;
												 Q_angle[i]=Q_angle[i]-jmax[i]*((t_current-2.0*alpha)*(t_current-2.0*alpha)*(t_current-2.0*alpha)/6.0);	 	 
											 }
											 else if((3.0*alpha<t_current)&(t_current<=Time))
											 {
												 Q_angle[i]=jmax[i]*(11.0*alpha*alpha*alpha/6.0 +alpha*alpha*(t_current-3.0*alpha)/2.0); 
												 Q_angle[i]=Q_angle[i]+ jmax[i]*(-alpha*(t_current-3.0*alpha)*(t_current-3.0*alpha)/2.0);
												 Q_angle[i]=Q_angle[i]+ jmax[i]*((t_current-3.0*alpha)*(t_current-3.0*alpha)*(t_current-3.0*alpha)/6.0);	 
											 }
								}
									
								for(i=0;i<6;i++)
									{										
										Delta_Q_Temp=Q_angle[i]-Q_angle_old[i];																		 	
										if( abs(Delta_Q_Temp) < 5.0) // neu di chuyen qua nho thi  de cho di chuyen ke tiep
										{
											Delta_Q_angle[i]=0;
										}
										else
										{
											//total_pulse[i]+=Delta_Q_Temp;	
											Delta_Q_angle[i]= (int)Delta_Q_Temp;
											// Cong dan nhung luong di chuyen nho de tranh lam anh huong den nhung khop khac 
											Q_angle_old[i]=Q_angle_old[i]+ (double)(Delta_Q_angle[i]);	// Canh bao cong don
										}
									}
								
//				for(i=0;i<6;i++)
//				{						
//					UARTPutnlong(USARTx,Delta_Q_angle[i]);
//					USART_SendData(USARTx,(uint16_t)' '); 
//					while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);		
//				}
//					USART_SendData(USARTx,(uint16_t)10); 
//					while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);										
									for(i=0;i<6;i++)
									{
                    total_pulse[i]+=Delta_Q_angle[i];							
									}																						
								Flag_Slave_Receive_Finish=false;		
								UART_Send_To_Slave(USARTx1, Delta_Q_angle,time_sample);									
             }	
      }
			 UARTprint(USARTx,"Totol Pulse**");	
				for(i=0;i<6;i++)
				{						
					UART_Print_Double(USARTx,total_pulse[i]);
					USART_SendData(USARTx,(uint16_t)' '); 
					while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);		
				}
					USART_SendData(USARTx,(uint16_t)10); 
					while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);					
					// Start Creat Pulse
					UARTprint(USARTx,"START MOVING**");	
			    UART_Finish_Send_To_Slave(USARTx1);
}

void Move_Through_Tracking(uint16_t A,uint16_t B)
 {
	int i,j=0; // i is segment  ,j is joint of robot
	int k;
	double t=0;
	int num_point;// TOTAL Position need to move through 
	int segment[6] ={0,0,0,0,0,0};
	double x0_old[6]={0.0,0.0,0.0,0.0,0.0,0};
	double x0_new[6]={0.0,0.0,0.0,0.0,0.0,0};
	double v0[6]={0.0,0.0,0.0,0.0,0.0,0};
	double t_segment[6] ={0.0,0.0,0.0,0.0,0.0,0};
	double total_time=0;
	double time_stamp=0.01 ; //10ms
	double Delta_Q_Temp;
	int Max_Delta_Q_Segment =0 ;
	Flag_Slave_Receive_Finish=false;
//	Time_dij[0]=0.0;
//	Time_dij[1]=2.0;
//	Time_dij[2]=2.0;
//	Time_dij[3]=1.5;
//	Time_dij[4]=1.0;
//	Time_dij[5]=3.0;
//	Time_dij[6]=2.5;
//	Time_dij[7]=0.5;
	UARTprint(USARTx,"Move Through");	
	Reset_QEI(0);	
	num_point= abs(A-B)+1;
// Calculate Delta Q 	
for(j=0;j<6;j++)
	{
			if(B>A)
			{
				// MOVE(2,6);
				//Move from intial position QEI
				Delta_Q_Move_Through[1][j] = Servo_Pluse_buffer[A][j]-Get_QEI(j);
				for(i=(A+1);i<=B;i++)
				{
					Delta_Q_Move_Through[i-A+1][j] = Servo_Pluse_buffer[i][j]-Servo_Pluse_buffer[i-1][j];			
				}
			}
			else
			{		
				// MOVE(6,2);		
				//Move from intial position QEI
				Delta_Q_Move_Through[1][j] = Servo_Pluse_buffer[B][j]-Get_QEI(j);
				// Delta moving between next to point 
				for(i=(B+1);i<=A;i++)
				{
					Delta_Q_Move_Through[i-B+1][j] = Servo_Pluse_buffer[i][j]-Servo_Pluse_buffer[i-1][j];			
				}
			}
		}
		   UARTprint(USARTx,"Max_Delta_Q_Segment");	
		// Calculate Time_dij 
		for(i=1;i<=num_point;i++)
		{
			for(j=0;j<6;j++)
			{
				
				if(abs(Delta_Q_Move_Through[i][j]) > abs (Max_Delta_Q_Segment))
				{
					Max_Delta_Q_Segment=Delta_Q_Move_Through[i][j];
				}
			}
								UART_Print_Double(USARTx,Max_Delta_Q_Segment );							 
								USART_SendData(USARTx,(uint16_t)' '); 
								while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
			
			Time_dij[i] = (double) (abs( Max_Delta_Q_Segment))/ 50000.0; // Velocity =30000 pulse/s 	
			Max_Delta_Q_Segment=0;//RESET VALUE
		}
						 	USART_SendData(USARTx,(uint16_t)10); 
							while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);			
		
    //Calculate Total Time of moving Through	
		for(i=1;i<=num_point;i++)
		{
			total_time += Time_dij[i];
		}
		     UARTprint(USARTx,"Total Time ");	
				UART_Print_Double(USARTx,total_time );							 
				USART_SendData(USARTx,(uint16_t)10); 
				while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);			
		     UARTprint(USARTx,"Calculate Accelaration Step 1");			
	for(j=0;j<6;j++)
 {
      // Calculate Accelaration 	
			for(i=1;i <= num_point;i++)
			{
				       Acc_move_through_ij[i][j] = 4.0 * Delta_Q_Move_Through[i][j]/(Time_dij[i] *Time_dij[i] );		
				
								UART_Print_Double(USARTx,Acc_move_through_ij[i][j] );							 
								USART_SendData(USARTx,(uint16_t)' '); 
								while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);	
				
				if(abs (Acc_move_through_ij[i][j])> abs (Acc_move_through_global[j]))
				{
					Acc_move_through_global[j] =Acc_move_through_ij[i][j] ;	
				}	
			}		
					USART_SendData(USARTx,(uint16_t)10); 
					while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);		
	
								UART_Print_Double(USARTx,Acc_move_through_global[j] );							 
								USART_SendData(USARTx,(uint16_t)' '); 
								while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);	
			
					UARTprint(USARTx,"Delta_Q_Move_Through");				
						for(i =0;i<=num_point;i++)
						 {
								UART_Print_Double(USARTx,Delta_Q_Move_Through[i][j] );							 
								USART_SendData(USARTx,(uint16_t)' '); 
								while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);			 
						 }
						 	USART_SendData(USARTx,(uint16_t)10); 
							while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);	
		
	          UARTprint(USARTx,"Time_dij");					 
						for(i =0;i<=num_point;i++)
						 {
								UART_Print_Double(USARTx,Time_dij[i] );							 
								USART_SendData(USARTx,(uint16_t)' '); 
								while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);			 
						 }
						 	USART_SendData(USARTx,(uint16_t)10); 
							while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
						 
						 
				// Process Start point 
				if(Delta_Q_Move_Through[1][j] !=0)
				{
				//Acc_move_through[0][j]= sign(Delta_Q_Move_Through[1][j])*abs(Acc_move_through_ij[1][j]); // Smooth but large error position
				Acc_move_through[0][j]= sign(Delta_Q_Move_Through[1][j])*abs(Acc_move_through_global[j]);
				Time_i[0][j]=Time_dij[1]-sqrt(Time_dij[1]*Time_dij[1] - 2*(double)Delta_Q_Move_Through[1][j] / Acc_move_through[0][j]);
				Velocity_max[0][j]=(double)Delta_Q_Move_Through[1][j] /(Time_dij[1] - 0.5*Time_i[0][j]);
				}
				else
				{
				Acc_move_through[0][j]= 0;
				Time_i[0][j]=0;
				Velocity_max[0][j]=0;
				}
			 if(Delta_Q_Move_Through[num_point][j] !=0)
				{
				// Process End point 
				//Acc_move_through[num_point][j]= sign(-Delta_Q_Move_Through[num_point][j])*abs(Acc_move_through_ij[num_point][j]);
				Acc_move_through[num_point][j]= sign(-Delta_Q_Move_Through[num_point][j])*abs(Acc_move_through_global[j]);
				Time_i[num_point][j]=Time_dij[num_point]-sqrt(Time_dij[num_point]*Time_dij[num_point]+ 2*Delta_Q_Move_Through[num_point][j] / Acc_move_through[num_point][j]);
				Velocity_max[num_point-1][j]=Delta_Q_Move_Through[num_point][j] /(Time_dij[num_point] - 0.5*Time_i[num_point][j]);
				}
				else
				{
				Acc_move_through[num_point][j]= 0;
				Time_i[num_point][j]=0;
				Velocity_max[num_point-1][j]=0;// warning 
				}
						// Process inside point 			
						for(i=1;i<num_point;i++)
						{
								if(i== num_point -1)
								{
									Velocity_max[i][j]=Velocity_max[num_point-1][j];
								}
								else
								{
								   Velocity_max[i][j]=Delta_Q_Move_Through[i+1][j]/Time_dij[i+1];										
								}

								//Acc_move_through[i][j]= sign(Velocity_max[i][j]-Velocity_max[i-1][j])*abs(Acc_move_through_ij[i+1][j]); 
								
								Acc_move_through[i][j]= sign(Velocity_max[i][j]-Velocity_max[i-1][j])*abs(Acc_move_through_global[0]); 
								
								if(Acc_move_through[i][j]!=0)	
								{									
								Time_i[i][j]=(Velocity_max[i][j]-Velocity_max[i-1][j])/Acc_move_through[i][j];
								}
								else
								{		
								Time_i[i][j]=0;	
								}									
				
								// Caculate Time_i of previous point 
								if(i==1)
								{
								//Start Point 
								Time_ij[i-1][j] = Time_dij[i] - Time_i[i-1][j]- 0.5*Time_i[i][j] ;
								}
								else
								{
								Time_ij[i-1][j] = Time_dij[i] - 0.5*Time_i[i-1][j]- 0.5*Time_i[i][j] ;			
								}	
						}		
					 //Time_ij of End Point 
						Time_ij[num_point-1][j] = Time_dij[num_point] - Time_i[num_point][j]- 0.5*Time_i[num_point-1][j] ;	



				
					       UARTprint(USARTx,"Acc_move_through");			
									for(i =0;i<=num_point;i++)
									 {
											UART_Print_Double(USARTx,Acc_move_through[i][j] );							 
											USART_SendData(USARTx,(uint16_t)' '); 
											while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);			 
									 }
										USART_SendData(USARTx,(uint16_t)10); 
										while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);		
									 
									UARTprint(USARTx,"Velocity_max");						 
									for(i =0;i<=num_point;i++)
									 {
											UART_Print_Double(USARTx,Velocity_max[i][j] );							 
											USART_SendData(USARTx,(uint16_t)' '); 
											while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);			 
									 }
										USART_SendData(USARTx,(uint16_t)10); 
										while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);						 
										 
				           UARTprint(USARTx,"Time_i");	
				
									for(i =0;i<=num_point;i++)
									 {
											UART_Print_Double(USARTx,Time_i[i][j] );							 
											USART_SendData(USARTx,(uint16_t)' '); 
											while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);			 
									 }
										USART_SendData(USARTx,(uint16_t)10); 
										while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);					
				
				           UARTprint(USARTx,"Time_ij");	
											for(i =0;i<=num_point;i++)
									 {
											UART_Print_Double(USARTx,Time_ij[i][j] );							 
											USART_SendData(USARTx,(uint16_t)' '); 
											while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);			 
									 }
										USART_SendData(USARTx,(uint16_t)10); 
										while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);			


						}
			 /* Process Time ,Velocity Finish */
				UARTprint(USARTx,"Process data and send to Slave");	
				Flag_Slave_Busy=true;	
				Flag_Slave_Receive_Finish=false;
       // Flag_Slave_Receive_Finish=true;							
			 /* Start Save Send Data to slave*/
						while(Flag_Slave_Receive_Finish==false) // wait if slave busy 
						{
							USART_SendData(USARTx1,(uint16_t)140); 
							while (USART_GetFlagStatus(USARTx1, USART_FLAG_TXE) == RESET);
							delay_ms(1000);
						}							
						 
			while(t<total_time)
					{						
						if(Flag_Slave_Receive_Finish== true)
						{						
								for(j=0;j<6;j++)
									 {							
													if(t_segment[j] <= Time_i[segment[j]][j]) // 0 1 2 3 4 
													{
														x0_new[j]=x0_old[j] + v0[j]*time_stamp + (Acc_move_through[segment[j]][j] *time_stamp*time_stamp )/2;
														v0[j]=v0[j] + Acc_move_through[segment[j]][j] *time_stamp;												
													}
													else if(( Time_i[segment[j]][j] < t_segment[j]) & ( t_segment[j] <= ( Time_ij[segment[j]][j] + Time_i[segment[j]][j] ))) //0 1 2 3
													{
														x0_new[j]= x0_old[j]+v0[j]*time_stamp;;	
													}
													else
													{	
														//reset for next segment 
														segment[j]++;
														t_segment[j]=0;
														// Process for first step of next segment 
														x0_new[j]=x0_old[j] + v0[j]*time_stamp + (Acc_move_through[segment[j]][j] *time_stamp*time_stamp )/2;
														v0[j]=v0[j] + Acc_move_through[segment[j]][j] *time_stamp;
													}
																	t_segment[j]+=time_stamp;	
                                  Delta_Q_Temp=x0_new[j]-x0_old[j];								
																	if( abs(Delta_Q_Temp) < 5.0) // neu di chuyen qua nho thi  de cho di chuyen ke tiep
																	{
																		Delta_Q_angle[j]=0;
																	}
																	else
																	{
																		//total_pulse[i]+=Delta_Q_Temp;	
																		Delta_Q_angle[j]= (int)Delta_Q_Temp;
																		// Cong dan nhung luong di chuyen nho de tranh lam anh huong den nhung khop khac 
																		x0_old[j]=x0_old[j]+ (double)(Delta_Q_angle[j]);	// Canh bao cong don
																	}
																								
//															UART_Print_Double(USARTx,Delta_Q_angle[j]);							 
//															USART_SendData(USARTx,(uint16_t)' '); 
//															while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);																									
										}
									t+=time_stamp;	 
										
//  	 						UART_Print_Double(USARTx,t );							 
//								USART_SendData(USARTx,(uint16_t)10); 
//								while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);	
             // Send to Slave 				
								Flag_Slave_Receive_Finish=false;		
								UART_Send_To_Slave(USARTx1, Delta_Q_angle,time_stamp);						
				}
				}
					
					// Start Creat Pulse
					UARTprint(USARTx,"START MOVING**");	
			    UART_Finish_Send_To_Slave(USARTx1);				
}

 void invert_cutted_9()
 {
				 	 int i ,j;
				   double Invert_Delta_Q[8]={ 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,0.0,0.0 };// += therefor not creat in global variable
	         bool Flag_solution[8] = { true, true, true, true, true, true, true, true }; // No creat in type of Global variable 
			     int solution_pass = 10; // If solution =10 it mean error invert 
					 double temp_delta = 2160.0 ;// max of delta ~ 360*6
					 NaN = sqrt(-1);			 
           px = position[0] * 1000.0; //convert to mm 
            py = position[1] * 1000.0;
            pz = position[2] * 1000.0;
            x9 = px - (orientation[0][ 0] * lenght_a6 + orientation[0][ 2] * length_d6);
            x10 = py - (orientation[1][ 0] * lenght_a6 + orientation[1][ 2] * length_d6);
            num41 = pz - (orientation[2][ 0] * lenght_a6 + orientation[2][2] * length_d6);
            num42 = 2.0 * x1 * x9;
            num43 = 2.0 * x1 * x10;
            x11 = 2.0 * x2 * x3;
            x12 = 2.0 * x2 * length_d4;
            x13 = x2*x2 + x3*x3 + length_d2*length_d2 + length_d3*length_d3 + length_d4*length_d4 - x1*x1 - x9*x9 - x10*x10 - (num41 - length_d1)*(num41 - length_d1) + 2.0 * length_d2 * length_d3;
            num44 = -x10;
            num45 = x9;
            x16 = length_d2 + length_d3 - (num41 - length_d1) * cos(angle_alpha1);
            num46 = x11*x11;
            num47 = x12*x12;
            num48 = 2.0 * (x11 * x12);
            num49 = 2.0 * (x13 * x12);
            num50 = 2.0 * (x13 * x12); num87=4.0 * x1*x1 * (x9*x9 + x10*x10);
            num51 = 4.0 * x1*x1 * x16*x16 + x13*x13 - num87;
            num52 = 4.0 * x1*x1 * x16*x16 + (x13 - x11)*(x13 - x11) - num87;
            num53 = 4.0 * (x12 * (x13 - x11));
            num54 = 2.0 * (4.0 * x1*x1 * x16*x16 + ( x13*x13 - x11*x11 + 2.0 * x12*x12) - num87);
            num55 = 4.0 * (x12 * (x13 + x11));
            num56 = 4.0 * x1*x1 * x16*x16 + (x13 + x11)*(x13 + x11) - num87;
            if (num52 != 0.0)
            {
                x17 = num53 / num52;
                x18 = num54 / num52;
                x19 = num55 / num52;
                num57 = num56 / num52;
                num58 = -x17 / 4.0;
                x20 = x18*x18 - 3.0 * x17 * x19 + 12.0 * num57;
                x21 = (2.0 * pow(x18, 3.0) - 9.0 * x17 * x18 * x19 + 27.0 * x19*x19 + 27.0 * x17*x17 * num57 - 72.0 * x18 * num57) / 2.0;
                num59 = (-1.0 * pow(x17, 3.0) + 4.0 * x17 * x18 - 8.0 * x19) / 32.0;
                num60 = (3.0 * x17*x17 - 8.0 * x18) / 48.0;
                x22 = x21*x21 - pow(x20, 3.0);
                num61 = 0.0;
                d1 = sqrt(x22*x22 +num61*num61);
                num62 = atan2(num61, x22);
                num63 = sqrt(d1) * cos(num62 / 2.0);
                num64 = sqrt(d1) * sin(num62 / 2.0);
                x23 = x21 + num63;
                num65 = num64;
                x24 = sqrt(x23*x23 + num65*num65);
                num66 = atan2(num65, x23);
                x25 = pow(x24, 0.33333) * cos(num66 / 3.0);
                x26 = pow(x24, 0.33333) * sin(num66 / 3.0);
                x27 = sqrt(x25*x25 + x26*x26);
                num67 = 1.0 / 12.0 * (x20 * x25 / (x27*x27) + x25);
                num68 = 1.0 / 12.0 * (-1.0 * x20 * x26 / (x27*x27) + x26);
                x28 = num60 + num67;
                num69 = num68;
                d2 = sqrt((x28*x28) + num69*num69);
                num70 = atan2(num69, x28);
                x29 = sqrt(d2) * cos(num70 / 2.0);
                x30 = sqrt(d2) * sin(num70 / 2.0);
                x31 = sqrt(x29*x29 + x30*x30);
                num71 = 2.0 * num60 - num67;
                num72 = -1.0 * num68;
                num73 = num59 * x29 / (x31*x31);
                num74 = -num59 * x30 / (x31*x31);
                num75 = num58 - x29;
                num76 = -1.0 * x30;
                x32 = num71 - num73;
                num77 = num72 - num74;
                num78 = num58 + x29;
                num79 = x30;
                x33 = num71 + num73;
                num80 = num72 + num74;
                d3 = sqrt(x32*x32 + num77*num77);
                num81 = atan2(num77, x32);
                num82 = cos(num81 / 2.0) * sqrt(d3);
                num83 = sin(num81 / 2.0) * sqrt(d3);
                d4 = sqrt(x33*x33 + num80*num80);
                num84 = atan2(num80, x33);
                num85 = cos(num84 / 2.0) * sqrt(d4);
                num86 = sin(num84 / 2.0) * sqrt(d4);
                num1 = num75 - num82;
                num5 = num76 - num83;
                num2 = num75 + num82;
                num6 = num76 + num83;
                num3 = num78 - num85;
                num7 = num79 - num86;
                num4 = num78 + num85;
                num8 = num79 + num86;
            }
            else
            {
                num57 = num53;
                x17 = num54;
                x18 = num55;
                num58 = num56;
                if (num57 != 0.0 && num58 != 0.0)
                {
                    x19 = x17 / num57;
                    num59 = x18 / num57;
                    num60 = num58 / num57;
                    x20 = num59 - x19*x19 / 3.0;
                    x21 = num60 - x19 * num59 / 3.0 + 2.0 * pow(x19, 3.0) / 27.0;
                    if (x21*x21 / 4.0 + pow(x20, 3.0) / 27.0 >= 0.0)
                    {
                        x22 = -x21 / 2.0 + sqrt(x21*x21 / 4.0 + pow(x20, 3.0) / 27.0);
                        x23 = -x21 / 2.0 - sqrt(x21*x21 / 4.0 + pow(x20, 3.0) / 27.0);
                        num61 = x22 >= 0.0 ? pow(x22, 1.0 / 3.0) : -pow(abs(x22), 1.0 / 3.0);
                        num62 = x23 >= 0.0 ? pow(x23, 1.0 / 3.0) : -pow(abs(x23), 1.0 / 3.0);
                        num63 = num61 + num62;
                        num64 = -0.5 * (num61 + num62) + sqrt(-0.75) * (num61 - num62);
                        num65 = -0.5 * (num61 + num62) - sqrt(-0.75) * (num61 - num62);
                        num1 = num63 - x19 / 3.0;
                        num5 = 0.0;
                        num2 = -0.5 * (num61 + num62) - x19 / 3.0;
                        num6 = sqrt(0.75) * (num61 - num62);
                        num3 = -0.5 * (num61 + num62) - x19 / 3.0;
                        num7 = -1.0 * sqrt(0.75) * (num61 - num62);
                        num4 = 0.0;
                        num8 = 1000.0;
                    }
                    else
                    {
                        num61 = 0.0;
                        num62 = 0.0;
                        num63 = 0.0;
                        num1 = num61;
                        num5 = 1000.0;
                        num2 = num62;
                        num6 = 1000.0;
                        num3 = num63;
                        num7 = 1000.0;
                        num4 = 0.0;
                        num8 = 1000.0;
                    }
                }
                else if (num57 != 0.0 && num58 == 0.0)
                {
                    d0 = x17*x17 - 4.0 * num57 * x18;
                    if (d0 >= 0.0)
                    {
                        num59 = (-x17 + sqrt(d0)) / (2.0 * num57);
                        num60 = (-x17 - sqrt(d0)) / (2.0 * num57);
                        num61 = 0.0;
                        num1 = num59;
                        num5 = 0.0;
                        num2 = num60;
                        num6 = 0.0;
                        num3 = num61;
                        num7 = 0.0;
                        num4 = 0.0;
                        num8 = 1000.0;
                    }
                    else
                    {
                        num59 = 0.0;
                        num1 = -x17 / (2.0 * num57);
                        num5 = sqrt(-1.0 * d0) / (2.0 * num57);
                        num2 = -x17 / (2.0 * num57);
                        num6 = -1.0 * (sqrt(-1.0 * d0) / (2.0 * num57));
                        num3 = num59;
                        num7 = 0.0;
                        num4 = 0.0;
                        num8 = 1000.0;
                    }
                }
                else if (num57 == 0.0 && num58 != 0.0)
                {
                    d0 = x18*x18 - 4.0 * x17 * num58;
                    if (d0 >= 0.0)
                    {
                        num1 = (-x18 + sqrt(d0)) / (2.0 * x17);
                        num5 = 0.0;
                        num2 = (-x18 - sqrt(d0)) / (2.0 * x17);
                        num6 = 0.0;
                        num3 = 0.0;
                        num7 = 1000.0;
                        num4 = 0.0;
                        num8 = 1000.0;
                    }
                    else
                    {
                        num1 = -x18 / (2.0 * x17);
                        num5 = sqrt(-1.0 * d0) / (2.0 * x17);
                        num2 = -x18 / (2.0 * x17);
                        num6 = -1.0 * sqrt(-1.0 * d0) / (2.0 * x17);
                        num3 = 0.0;
                        num7 = 1000.0;
                        num4 = 0.0;
                        num8 = 1000.0;
                    }
                }
                else if (num57 == 0.0 && num58 == 0.0)
                {
                    num1 = 0.0;
                    num5 = 0.0;
                    num2 = -x18 / x17;
                    num6 = 0.0;
                    num3 = 0.0;
                    num7 = 1000.0;
                    num4 = 0.0;
                    num8 = 1000.0;
                }
            }

            numArray4[0] = num1;
            numArray4[1] = num2;
            numArray4[2] = num3;
            numArray4[3] = num4;
            numArray5[0] = num5;
            numArray5[1] = num6;
            numArray5[2] = num7;
            numArray5[3] = num8;
            //double[] numArray4 = new double[4]
            //        { num1,num2, num3,num4 };
            //        double[] numArray5 = new double[4]
            //        {  num5,num6,num7,num8 };
            for (index0 = 0; index0 <= 3; ++index0)
            {
                if (abs(numArray5[index0]) <= 0.001)
                {
                    num57 = 2.0 * atan2(numArray4[index0], 1.0);
                    num58 = num57 * degree_unit;
                    numArray8[index0] = num58;
                    num59 = 2.0 * x1 * (x9*x9 + x10*x10);
                    x17 = x2 + x3 * cos(num57) + length_d4 * sin(num57);
                    x18 = -x3 * sin(num57) + length_d4 * cos(num57);
                    num60 = x17*x17 + x18*x18;
                    if (abs(num59) > 0.0001)
                    {
                        x19 = 1.0 / num59 * (-num45 * (x11 * cos(num57) + x12 * sin(num57) + x13) + num43 * x16);
                        num61 = atan2(1.0 / num59 * (num44 * (x11 * cos(num57) + x12 * sin(num57) + x13) - num42 * x16), x19);
                        num62 = num61 * degree_unit;
                        numArray6[index0] = num62;
                        if (abs(num60) > 0.0001)
                        {
                            x20 = 1.0 / num60 * (x17 * (x9 * cos(num61) + x10 * sin(num61) - x1) - x18 * (-x9 * cos(angle_alpha1) * sin(num61) + x10 * cos(angle_alpha1) * cos(num61) - (num41 - length_d1)));
                            num63 = atan2(1.0 / num60 * (x18 * (x9 * cos(num61) + x10 * sin(num61) - x1) + x17 * (-x9 * cos(angle_alpha1) * sin(num61) + x10 * cos(angle_alpha1) * cos(num61) - (num41 - length_d1))), x20) * degree_unit;
                            numArray7[index0] = num63;
                        }
                        else
                            numArray7[index0] = NaN ;  //double.NaN;
                    }
                    else
                    {
                        numArray6[index0] = NaN  ; //double.NaN;
                        numArray7[index0] = NaN  ; //double.NaN;
                    }
                }
                else
                {
                    numArray6[index0] = NaN  ;  //double.NaN;
                    numArray7[index0] = NaN  ; //double.NaN;
                    numArray8[index0] = NaN  ; //double.NaN;
                }
            }
            position_matix[0][ 0] = numArray6[0];
            position_matix[1][ 0] = numArray6[1];
            position_matix[2][ 0] = numArray6[2];
            position_matix[3][ 0] = numArray6[3];

            position_matix[0][ 1] = numArray7[0];
            position_matix[1][ 1] = numArray7[1];
            position_matix[2][ 1] = numArray7[2];
            position_matix[3][ 1] = numArray7[3];

            position_matix[0][ 2] = numArray8[0];
            position_matix[1][ 2] = numArray8[1];
            position_matix[2][ 2] = numArray8[2];
            position_matix[3][ 2] = numArray8[3];
            for (index0 = 0; index0 < 4; ++index0)
            {
                matrix_11[index0][ 0][ 0] = cos(radian_unit * position_matix[index0][ 0]);
                matrix_11[index0][ 0][ 1] = sin(radian_unit * position_matix[index0][ 0]);
                matrix_11[index0][ 0][ 2] = 0.0;
                matrix_11[index0][ 1][ 0] = 0.0;
                matrix_11[index0][ 1][ 1] = 0.0;
                matrix_11[index0][ 1][ 2] = -1.0;
                matrix_11[index0][ 2][ 0] = (-1.0) * sin(radian_unit * position_matix[index0][ 0]);
                matrix_11[index0][ 2][ 1] = 1.0 * cos(radian_unit * position_matix[index0][ 0]);
                matrix_11[index0][ 2][ 2] = 0.0;
            }
            for (index0 = 0; index0 < 4; ++index0)
            {
                matrix_12[index0][ 0][ 0] = cos(radian_unit * position_matix[index0][ 1]);
                matrix_12[index0][ 0][ 1] = sin(radian_unit * position_matix[index0][ 1]);
                matrix_12[index0][ 0][ 2] = 0.0;
                matrix_12[index0][ 1][ 0] = -sin(radian_unit * position_matix[index0][ 1]);
                matrix_12[index0][ 1][ 1] = cos(radian_unit * position_matix[index0][ 1]);
                matrix_12[index0][ 1][ 2] = 0.0;
                matrix_12[index0][ 2][ 0] = 0.0;
                matrix_12[index0][ 2][ 1] = 0.0;
                matrix_12[index0][ 2][ 2] = 1.0;
            }
            for (index0 = 0; index0 < 4; ++index0)
            {
                matrix_13[index0][ 0][ 0] = cos(radian_unit * position_matix[index0][ 2]);
                matrix_13[index0][ 0][ 1] = sin(radian_unit * position_matix[index0][ 2]);
                matrix_13[index0][ 0][ 2] = 0.0;
                matrix_13[index0][ 1][ 0] = 0.0;
                matrix_13[index0][ 1][ 1] = 0.0;
                matrix_13[index0][ 1][ 2] = 1.0;
                matrix_13[index0][ 2][ 0] = 1.0 * sin(radian_unit * position_matix[index0][ 2]);
                matrix_13[index0][ 2][ 1] = -1.0 * cos(radian_unit * position_matix[index0][ 2]);
                matrix_13[index0][ 2][ 2] = 0;
            }
            for (index1 = 0; index1 < 4; ++index1)
            {
                for (index2 = 0; index2 < 3; ++index2)
                {
                    for (index3 = 0; index3 < 3; ++index3)
                    {
                        matrix_14[index1][ index2][ index3] = 0.0;
                        for (index4 = 0; index4 < 3; ++index4)
                            matrix_14[index1][ index2][ index3] += matrix_13[index1][ index2][ index4] * matrix_12[index1][ index4][ index3];
                    }
                }
            }
            for (index1 = 0; index1 < 4; ++index1)
            {
                for (index2 = 0; index2 < 3; ++index2)
                {
                    for (index3 = 0; index3 < 3; ++index3)
                    {
                        matrix_15[index1][ index2][ index3] = 0.0;
                        for (index4 = 0; index4 < 3; ++index4)
                            matrix_15[index1][ index2][ index3] += matrix_14[index1][ index2][ index4] * matrix_11[index1][ index4][ index3];
                    }
                }
            }
            for (index1 = 0; index1 < 4; ++index1)
            {
                for (index2 = 0; index2 < 3; ++index2)
                {
                    for (index3 = 0; index3 < 3; ++index3)
                    {
                        matrix_16[index1][ index2][ index3] = 0.0;
                        for (index4 = 0; index4 < 3; ++index4)
                            matrix_16[index1][ index2][ index3] += matrix_15[index1][ index2][ index4] * orientation[index4][ index3];
                    }
                }
            }
            for (index1 = 0; index1 < 4; ++index1)
            {
                x17 = matrix_16[index1][ 0][ 2];
                x18 = matrix_16[index1][ 1][ 2];
                x19 = matrix_16[index1][ 2][ 2];
                d0 = (x17*x17 + x18*x18) * 1.0;
							/* warning*/
              //   num58 = Math.Round(pow(x19, 2.0) - 1.0, 4);
							
							  num58 = x19*x19 - 1.0;
                x20 = -x18 * 1.0;
                y1 = x17 + sqrt(d0);
                y2 = x17 - sqrt(d0);

                if (num58 <= 0.0)
                {
                    if (x20 == 0.0)
                    {
                        if (y1 != 0.0 && y2 == 0.0)
                        {
                            num65 = 2.0 * atan2(y1, x20);
                            num66 = NaN ;  //double.NaN;
                        }
                        else if (y1 == 0.0 && y2 != 0.0)
                        {
                            num65 = NaN ;  //double.NaN;
                            num66 = 2.0 * atan2(y2, x20);
                        }
                        else if (y1 == 0.0 && y2 == 0.0)
                        {
                            num65 = NaN  ; //double.NaN;
                            num66 = NaN  ; //double.NaN;
                        }
                        else
                        {
                            num65 = 2.0 * atan2(y1, x20);
                            num66 = 2.0 * atan2(y2, x20);
                        }
                    }
                    else
                    {
                        num65 = 2.0 * atan2(y1, x20);
                        num66 = 2.0 * atan2(y2, x20);
                    }
                    num59 = num65 * degree_unit;
                    num60 = num66 * degree_unit;
                    matrix_17[0] = num65;
                    matrix_17[1] = num66;
                    matrix_18[0] = num59;
                    matrix_18[1] = num60;
                    num67 = 1.0 * matrix_16[index1][ 0][ 2];
                    num68 = 1.0 * matrix_16[index1][ 1][ 2];
                    num69 = 1.0 * matrix_16[index1][ 2][ 2];
                    for (index2 = 0; index2 <= 1; ++index2)
                    {
                        y3 = 1.0 / 1.0 * (num67 * cos(matrix_17[index2]) + num68 * sin(matrix_17[index2]));
                        x21 = -1.0 / 1.0 * (num69);
                        matrix_19[index2] = atan2(y3, x21);
                        matrix_20[index2] = matrix_19[index2] * degree_unit;
                        num70 = matrix_16[index1][ 0][ 0] * cos(matrix_17[index2]) + matrix_16[index1][ 1][ 0] * sin(matrix_17[index2]);
                        num71 = 1.0 * matrix_16[index1][ 2][ 0];
                        num72 = 1.0 * (matrix_16[index1][ 0][ 0] * sin(matrix_17[index2]) - matrix_16[index1][ 1][ 0] * cos(matrix_17[index2]));
                        x22 = num70 * x21 + num71 * y3;
                        y4 = num72 * 1.0;
                        matrix_21[index2] = atan2(y4, x22);
                        matrix_22[index2] = matrix_21[index2] * degree_unit;
                    }
                    num61 = matrix_20[0];
                    num62 = matrix_20[1];
                    num63 = matrix_22[0];
                    num64 = matrix_22[1];
                }
                else
                {
                    num59 = NaN  ; //double.NaN;
                    num61 = NaN  ; //double.NaN;
                    num63 = NaN  ; //double.NaN;
                    num60 = NaN  ; //double.NaN;
                    num62 = NaN  ; //double.NaN;
                    num64 = NaN  ; //double.NaN;
                }
                orientation_matrix[index1][ 0][ 0] = num59;
                orientation_matrix[index1][ 0][ 1] = num60;
                orientation_matrix[index1][ 1][ 0] = num61;
                orientation_matrix[index1][ 1][ 1] = num62;
                orientation_matrix[index1][ 2][ 0] = num63;
                orientation_matrix[index1][ 2][ 1] = num64;
            }
			
            // Solution 0 
            Solution [0][0]   =position_matix[0][ 0];
						Solution [1][0]   =position_matix[0][ 1];
            Solution [2][0]   =position_matix[0][ 2];
            Solution [3][0]   =orientation_matrix[0][ 0][ 0];
            Solution [4][0]   =orientation_matrix[0][ 1][ 0];
            Solution [5][0]   =orientation_matrix[0][ 2][ 0];

            // Solution 1 
            Solution [0][1]   =position_matix[0][ 0];
            Solution [1][1]   =position_matix[0][ 1];
            Solution [2][1]   =position_matix[0][ 2];
            Solution [3][1]   =orientation_matrix[0][ 0][ 1];
            Solution [4][1]   =orientation_matrix[0][ 1][ 1];
            Solution [5][1]   =orientation_matrix[0][ 2][ 1];

            // Solution 2
            Solution [0][2]   =position_matix[1][ 0];
            Solution [1][2]   =position_matix[1][ 1];
            Solution [2][2]   =position_matix[1][ 2];
            Solution [3][2]   =orientation_matrix[1][ 0][ 0];
            Solution [4][2]   =orientation_matrix[1][ 1][ 0];
            Solution [5][2]   =orientation_matrix[1][ 2][ 0];

            // Solution 3
            Solution [0][3]   =position_matix[1][ 0];
            Solution [1][3]   =position_matix[1][ 1];
            Solution [2][3]   =position_matix[1][ 2];
            Solution [3][3]   =orientation_matrix[1][ 0][ 1];
            Solution [4][3]   =orientation_matrix[1][ 1][ 1];
            Solution [5][3]   =orientation_matrix[1][ 2][ 1];

            // Solution 4
            Solution [0][4]   =position_matix[2][ 0];
            Solution [1][4]   =position_matix[2][ 1];
            Solution [2][4]   =position_matix[2][ 2];
            Solution [3][4]   =orientation_matrix[2][ 0][ 0];
            Solution [4][4]   =orientation_matrix[2][ 1][ 0];
            Solution [5][4]   =orientation_matrix[2][ 2][ 0];

            // Solution 5
            Solution [0][5]   =position_matix[2][ 0];
            Solution [1][5]   =position_matix[2][ 1];
            Solution [2][5]   =position_matix[2][ 2];
            Solution [3][5]   =orientation_matrix[2][ 0][ 1];
            Solution [4][5]   =orientation_matrix[2][ 1][ 1];
            Solution [5][5]   =orientation_matrix[2][ 2][ 1];

           // Solution 6
            Solution [0][6]   =position_matix[3][ 0];
            Solution [1][6]   =position_matix[3][ 1];
            Solution [2][6]   =position_matix[3][ 2];
            Solution [3][6]   =orientation_matrix[3][ 0][ 0];
            Solution [4][6]   =orientation_matrix[3][ 1][ 0];
            Solution [5][6]   =orientation_matrix[3][ 2][ 0];

           // Solution 7
            Solution [0][7]   =position_matix[3][ 0];
            Solution [1][7]   =position_matix[3][ 1];
            Solution [2][7]   =position_matix[3][ 2];
            Solution [3][7]   =orientation_matrix[3][ 0][ 1];
            Solution [4][7]   =orientation_matrix[3][ 1][ 1];
            Solution [5][7]   =orientation_matrix[3][ 2][ 1];

								
            for ( i = 0; i < 8; i++)//SOLUTION I
            {
                for ( j = 0; j < 6; j++)
                {
                    if (!((Solution[j][i] >= low_limit[j]) &  (Solution[j][i] <= High_limit[j]) & (Flag_solution[i] == true) ))
                    {
                        Flag_solution[i] = false;
                    }
                }
            }
			
            //find delta of each solution			
            for ( i=0;i<8;i++)
            {       
								Invert_Delta_Q[i]=0; 							
                if(Flag_solution[i] ==true)
                {							
                    for ( j=0;j<6;j++)// Caculate all of joint angle of Robot 
                    {
                        Invert_Delta_Q[i] += abs(Solution[j][i]- Q_angle_old[j]);
                    }
                }
            }

            for ( i = 0; i < 8; i++)
            {
                if (Flag_solution[i] == true)
                {
                    if(temp_delta>Invert_Delta_Q[i])// find min delta 
                    {
											temp_delta=Invert_Delta_Q[i];
                      solution_pass = i;
                    }
                }
            }
            // return solution pass 
            if (solution_pass == 10)
            {
								UARTprint(USARTx,"ERROR INVERT **");	
                /* Error Invert */
                for ( i = 0; i < 6; i++)
                {
                    Q_angle[i] = 0;
                }
            }
            else
            {
                for ( i = 0; i < 6; i++)
                {
                    Q_angle[i] =Solution[i][solution_pass];
					
                }
            }
						
						/* Print Terminal */
//						 for(i =0;i<8;i++)
//						 {	
//								UART_Print_Double(USARTx,Invert_Delta_Q[i] );	
//							 
//								USART_SendData(USARTx,(uint16_t)' '); 
//								while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);			 
//						 }
//						 	USART_SendData(USARTx,(uint16_t)10); 
//							while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);		
						 // Print Angle of each Solution 
//						for( j=0;j<8;j++)
//						{
//							 for(i =0;i<6;i++)
//						 {
//								UART_Print_Double(USARTx,Solution[i][j] );							 
//								USART_SendData(USARTx,(uint16_t)' '); 
//								while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);			 
//						 }
//						 	USART_SendData(USARTx,(uint16_t)10); 
//							while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);						 
//					 }
						
//					  Print(USARTx,"Solution Pass : ");
//					  UARTPutnlong(USARTx,solution_pass);
//					 	USART_SendData(USARTx,(uint16_t)10); 
//						while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);		
//						for(i =0;i<6;i++)
//						 {
//								UART_Print_Double(USARTx,Q_angle[i] );						   
//								USART_SendData(USARTx,(uint16_t)' '); 
//								while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);			 
//						 }
						 /**************************/
			}

void  Inverser_Kinematics(real_T q[7]  ,real_T T[4][4])
 {
//	 real_T nx,ny,nz,ox,oy,oz,ax,ay,az,xc,yc,zc;
//	 nx=T[0][0];ny=T[1][0];nz=T[2][0];
//	 ox=T[0][1];oy=T[1][1];oz=T[2][1];
//	 ax=T[0][2];ay=T[1][2];az=T[2][2];
	 uint16_t i;
	  
	 real_T r11,r12,r13,r21,r22,r23,r31,r32,r33,xc,yc,zc;
	 real_T D,d_offset;
	 r11=T[0][0];r21=T[1][0];r31=T[2][0];
	 r12=T[0][1];r22=T[1][1];r32=T[2][1];
	 r13=T[0][2];r23=T[1][2];r33=T[2][2];
	 
	 xc= T[0][3];yc= T[1][3];zc =T[2][3];
	 
	 q[1]=atan2(xc,yc);	 
	 // following Robot_Modeling_and_Control page 104 --> d_offset= a1
	 d_offset=a[1];
	 D =(xc*xc+yc*yc- d_offset*d_offset +(zc-d[1])*(zc-d[1])-a[2]*a[2]-a[3]*a[3])/(2*a[2]*a[3]);
	 q[3]=atan2(D,sqrt(1-D*D));
	 // or q[3]=atan2(D, - sqrt(1-D*D));
	 q[2]=atan2(sqrt(xc*xc+yc*yc-d_offset*d_offset),zc-d[1])-atan2(a[2]+a[3]*cos(q[3]),a[3]*sin(q[3]));
	 q[4]=atan2(cos(q[1])*cos(q[2]+q[3])*r13  +  sin(q[1])*cos(q[2]+q[3])*r23  +  sin(q[2]+q[3])*r33,
							-cos(q[1])*sin(q[2]+q[3])*r13 - sin(q[1])*sin(q[2]+q[3])*r23 + cos(q[2]+q[3])*r33);
	 q[5]=atan2(sin(q[1])*r13 -cos(q[1])*r23,  sqrt(1-(sin(q[1])*r13 - cos(q[1])*r23)*(sin(q[1])*r13 - cos(q[1])*r23)));
	 // or q[5]=atan2(sin(q[1])*r13 -cos(q[1])*r23, - sqrt(1-(sin(q[1])*r13 - cos(q[1])*r23)*(sin(q[1])*r13 - cos(q[1])*r23)));
	 q[6]=atan2(-sin(q[1])*r11 + cos(q[1])*r21 , sin(q[1])*r12 -cos(q[1])*r22);
		 
	 for(i =1;i<=6;i++)
	 {
			UART_Print_Double(USARTx,q[i]*180/PI);
	  	USART_SendData(USARTx,(uint16_t)' '); 
		  while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);			 
	 }
		 
 }
 
 void  Inverser_Kinematics_2(real_T q[7]  ,real_T T[4][4])
 {
	 uint16_t i;
	 real_T e11,e12,e13,e21,e22,e23,e31,e32,e33,ux0,uy0,uz0;
	 real_T nx1,square_s1,square_s2,square_k,s1,s2,k,mp;
	 real_T q1[9],q2[9],q3[9],q4[9],q5[9],q6[9];// 8 solution ,q[0] not used	 
	 real_T cx0,cy0,cz0;
   real_T c1,c2,c3,c4,a1,a2;
	 
	 /********************/	 
	 e11=T[0][0];e12=T[0][1];e13=T[0][2];
	 e21=T[1][0];e22=T[1][1];e23=T[1][2];
	 e31=T[2][0];e32=T[2][1];e33=T[2][2];			 
	 ux0= T[0][3];uy0= T[1][3];uz0 =T[2][3];	 
	 /********************/
	 c1=d[1];
	 c2=a[2];
	 c3=d[4];
	 c4=d[6];
	 a1=a[1];
	 a2=-a[3];	 
//	 c1=0.45;
//	 c2=0.57;
//	 c3=0.64;
//	 c4=0.95;
//	 a1=0.15;
//	 a2=-0.13;
	 /********************/
	 cx0=ux0-c4*e13;
	 cy0=uy0-c4*e23;
	 cz0=uz0-c4*e33;
	 /********************/
	 nx1=sqrt(cx0*cx0 + cy0*cy0) -a1;
	 square_s1=nx1*nx1 + (cz0-c1)*(cz0-c1);
	 s1=sqrt(square_s1);
	 square_s2=(nx1+2*a1)*(nx1+2*a1) + (cz0-c1)*(cz0-c1);
	 s2=sqrt(square_s2);
	 square_k=a2*a2+c3*c3;
	 k=sqrt(square_k);
	 
	 /*******************/
	 q1[1]=q1[2]=q1[5]=q1[6]=atan2(cy0,cx0); //b=0 ---> atan2(b,nx1+a1)=0
	 q1[3]=q1[4]=q1[7]=q1[8]=atan2(cy0,cx0)-PI;
	 /***/
	 q2[1]=q2[5]=  acos((square_s1+c2*c2-square_k)/(2*s1*c2))+atan2(nx1,cz0-c1);
	 q2[2]=q2[6]= -acos((square_s1+c2*c2-square_k)/(2*s1*c2))+atan2(nx1,cz0-c1); 
	 q2[3]=q2[7]=  acos((square_s2+c2*c2-square_k)/(2*s2*c2))-atan2(nx1+2*a1,cz0-c1);
	 q2[4]=q2[8]= -acos((square_s2+c2*c2-square_k)/(2*s2*c2))-atan2(nx1+2*a1,cz0-c1);
	 /***/
	 q3[1]=q3[5]=  acos((square_s1-c2*c2-square_k)/(2*c2*k))-atan2(a2,c3);
	 q3[2]=q3[6]= -acos((square_s1-c2*c2-square_k)/(2*c2*k))-atan2(a2,c3);
	 q3[3]=q3[7]=  acos((square_s2-c2*c2-square_k)/(2*c2*k))-atan2(a2,c3);
	 q3[4]=q3[8]= -acos((square_s2-c2*c2-square_k)/(2*c2*k))-atan2(a2,c3);	
   /***/
	 for(i=1;i<5;i++)
	 {
	 q4[i]=  atan2(e23*cos(q1[i]) - e13*sin(q1[i]),e13*cos(q2[i]+q3[i])*cos(q1[i]) + e23*cos(q2[i]+q3[i])*sin(q1[i]) - e33*sin(q2[i]+q3[i]));
	 q4[i+4]=  q4[i]+PI;
	 /***/
		mp= e13*sin(q2[i]+q3[i])*cos(q1[i]) + e23*sin(q2[i]+q3[i])*sin(q1[i]) + e33*cos(q2[i]+q3[i]);
		q5[i]=atan2(sqrt(1-mp*mp),mp);
		q5[i+4]=-q5[i];
	 /***/ 
		q6[i]=atan2(e12*sin(q2[i]+q3[i])*cos(q1[i]) + e22*sin(q2[i]+q3[i])*sin(q1[i]) + e32*cos(q2[i]+q3[i]),
		   - e11*sin(q2[i]+q3[i])*cos(q1[i]) - e21*sin(q2[i]+q3[i])*sin(q1[i]) - e31*cos(q2[i]+q3[i]));
		q6[i+4]=q6[i]-PI;
		 
	 }
	 
	 UARTprint(USARTx,"Inverser_Kinematics");

for(i=1;i<9;i++)
{
	Print(USARTx,"Solution ");
  UARTPutnlong(USARTx,i);	
	USART_SendData(USARTx,(uint16_t)':'); 
	while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
	UART_Print_Double(USARTx,q1[i]*180/PI);
  USART_SendData(USARTx,(uint16_t)' ');
  while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);		
	UART_Print_Double(USARTx,q2[i]*180/PI);
  USART_SendData(USARTx,(uint16_t)' ');
  while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);		
	UART_Print_Double(USARTx,q3[i]*180/PI);
  USART_SendData(USARTx,(uint16_t)' ');
  while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);		
	UART_Print_Double(USARTx,q4[i]*180/PI);
  USART_SendData(USARTx,(uint16_t)' ');
  while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);		
	UART_Print_Double(USARTx,q5[i]*180/PI);
  USART_SendData(USARTx,(uint16_t)' ');
  while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);		
	UART_Print_Double(USARTx,q6[i]*180/PI);
  USART_SendData(USARTx,(uint16_t)' ');
  while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);		
	
//	USART_SendData(USARTx,(uint16_t)10); 
//  while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);	
}
	USART_SendData(USARTx,(uint16_t)10); 
  while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);	
}