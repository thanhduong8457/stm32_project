/*
 * Direct_Kinematics.h
 *
 *  Created on: May 3, 2016
 *      Author: AnhTuan
 */

#ifndef _DIRECT_KINEMATICS_H_
#define _DIRECT_KINEMATICS_H_

//#include "include.h"

/* UP6 YAKAWA MOTOMAN ROBOT
              theta     d        a       alpha
 L(1) = Link([ 0       0.45      0.15   -pi/2   0]);
 L(2) = Link([ 0       0         0.57    pi     0]);
 L(3) = Link([ 0       0         0.13   -pi/2   0]);
 L(4) = Link([ 0      -0.64      0       pi/2   0]);
 L(5) = Link([ 0       0         0       pi/2   0]);
 L(6) = Link([ 0       0.095     0       0      0]);
*/

/* Public variable */
extern volatile  real_T d[7]    ;
extern volatile  real_T a[7]    ;
extern volatile  real_T Apha[7] ;
extern volatile real_T T_Matix[4][4];

extern void DH_A1(real_T q1 , real_T temp[4][4]);
extern void DH_A(int i , real_T q , real_T temp[4][4]);
extern void Make_DK(real_T q1 , real_T q2 , real_T q3 , real_T q4 , real_T q5 ,real_T q6);
extern void Make_DK_Current_QEI(void);//Calculate Direct Kinematics at Current Position 
#endif /*DIRECT_KINEMATICS_H_*/
