/**********************************************************************************************
 * Arduino PID Library - Version 1
 * by Brett Beauregard <br3ttb@gmail.com> brettbeauregard.com
 *
 * This Code is licensed under a Creative Commons Attribution-ShareAlike 3.0 Unported License.
 **********************************************************************************************/
#include "PID.h"

PID::PID()
{
	this->Kp = 3.0;
    this->Ki = 2.0;
    this->Kd = 1.0;
	this->Ts = 0.01;
    this->mode = "normal";
}

PID::PID(float Kp, float Ki, float Kd, float Ts)
{
	this->Kp = Kp;
    this->Ki = Ki;
    this->Kd = Kd;
	this->Ts = Ts;
    this->mode = "normal";
}

PID::PID(float Kp, float Ki, float Kd, float Ts, std::string mode)
{
	this->Kp = Kp;
    this->Ki = Ki;
    this->Kd = Kd;
	this->Ts = Ts;
    this->mode = mode;
}

void PID::set_Kp(float Kp)
{
	this->Kp = Kp;
}

void PID::set_Ki(float Ki)
{
	this->Ki = Ki;	
}

void PID::set_Kd(float Kd)
{
	this->Kd = Kd;	
}

void PID::set_mode(std::string mode)
{
	this->mode = mode;
}

double PID::compute(float ref, float y)
{
	ek = ref-y;
	uk = uk1 + Kp*(ek-ek1)+(Ki*Ts*(ek+ek1))/2+(Kd*(ek-2*ek1+ek2))/Ts;
	uk1 = uk;
	ek2 = ek1;
	ek1 = ek;

	return uk;
}

void PID::show_info()
{
	std::cout<<"PID controler: ";
	std::cout<<"  Kp: "<<this->Kp;
	std::cout<<"  Ki: "<<this->Ki;
	std::cout<<"  Kd: "<<this->Kd;
	std::cout<<"  Ts: "<<this->Ts;
	std::cout<<"  mode: "<<this->mode;
}