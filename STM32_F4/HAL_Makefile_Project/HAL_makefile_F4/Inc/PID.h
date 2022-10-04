/**********************************************************************************************
 * Arduino PID Library - Version 1
 * by Brett Beauregard <br3ttb@gmail.com> brettbeauregard.com
 *
 * This Code is licensed under a Creative Commons Attribution-ShareAlike 3.0 Unported License.
 **********************************************************************************************/
#ifndef __PID_H__
#define __PID_H__
#include <iostream>
#include <string>
class PID
{
private:
    float Kp;
    float Ki;
    float Kd;
    std::string mode;
    double uk = 0, uk1 = 0, ek = 0, ek1 = 0, ek2 = 0;

    double Ts;
    
public:
    PID();
    PID(float, float, float, float);
    PID(float, float, float, float, std::string);
    void set_Kp(float);
    void set_Ki(float);
    void set_Kd(float);
	double compute(float, float);
    void set_mode(std::string);
	void show_info();
};
#endif//__PID_H__