#include "stdafx.h"
#include "PendCalc.h"
#include <math.h>

const double g = 9.8f ;

PendCalc::PendCalc(void)
{
}

PendCalc::~PendCalc(void)
{

}

void PendCalc::setA0(double A0) 
{
	this->A0 = A0 ;
	update() ;
}

void PendCalc::setV0(double V0) 
{
	this->V0 = V0 ;
	update() ;
}

void PendCalc::setLen(double Len) 
{
	this->Len = Len ;
	w = sqrt(g/Len) ;
	update() ;
}

void PendCalc::update() 
{
	Am = A0+V0*V0/2.0f ;
	Vm = sqrt(2.0f*A0+V0*V0) ;	
}

double PendCalc::getA(double t) 
{			
	return Am*sin(w*t) ;
}

double PendCalc::getV(double t) 
{			
	return Vm*sin(w*t) ;
}

double PendCalc::getE(double t) 
{
	return A0+V0*V0/2.0f-getK(t) ;
}

double PendCalc::getK(double t) 
{
	return getV(t)*getV(t)/2 ;
}
	
bool PendCalc::execTests() 
{	
	PendCalc calc ;

	const double EPS = 0.01 ;
	const double dt = 0.1 ;
	const double tperiod = 100 ;

	const double A0 = 2 ;
	const double V0 = 3 ;
	const double Len = 4 ;
	calc.setA0(A0) ;
	calc.setV0(V0) ;
	calc.setLen(Len) ;

	// Тест расчета w
	if (fabs(calc.w-sqrt(g/Len))>EPS) return false ;

	double t = 0 ;
	double A = (A0+V0*V0/2.0f) ;
	double V = sqrt(2.0f*A0+V0*V0) ;
	while (t<tperiod) 
	{		
		// Тесты, чтобы значения отклонения и скорости не выходили за амплитуды
		double a = calc.getA(t) ;
		if (fabs(a)-A>EPS) return false ;		
		double v = calc.getV(t) ;
		if (fabs(v)-V>EPS) return false ;		

		// Тест закона сохранения энергии
		if ((calc.getE(t)+calc.getK(t)-(A0+V0*V0/2.0f))>EPS) return false ;
		t+=dt ;
	}
	return true ;
}