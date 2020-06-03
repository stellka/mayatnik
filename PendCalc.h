#pragma once

// Класс для расчета показателей маятника
class PendCalc
{
private:
	// Входящие значения
	double A0 ;
	double V0 ;
	double Len ;
	// Расчитанные амплитуды
	double Am ;
	double Vm ;
	// Коэффициент маятника
	double w ;
	// Обновление показателей
	void update() ;
public:
	PendCalc(void);
	// Установка значений
	void setA0(double A0) ;
	void setV0(double V0) ;
	void setLen(double Len) ;
	// Получение отклонения, скорости и энергий
	double getA(double t) ;
	double getV(double t) ;
	double getE(double t) ;
	double getK(double t) ;
	~PendCalc(void);
	// А это тесты, вызываемые в начале программы
	static bool execTests() ;
};

