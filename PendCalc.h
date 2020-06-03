#pragma once

// ����� ��� ������� ����������� ��������
class PendCalc
{
private:
	// �������� ��������
	double A0 ;
	double V0 ;
	double Len ;
	// ����������� ���������
	double Am ;
	double Vm ;
	// ����������� ��������
	double w ;
	// ���������� �����������
	void update() ;
public:
	PendCalc(void);
	// ��������� ��������
	void setA0(double A0) ;
	void setV0(double V0) ;
	void setLen(double Len) ;
	// ��������� ����������, �������� � �������
	double getA(double t) ;
	double getV(double t) ;
	double getE(double t) ;
	double getK(double t) ;
	~PendCalc(void);
	// � ��� �����, ���������� � ������ ���������
	static bool execTests() ;
};

