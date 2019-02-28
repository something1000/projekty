#include "stdafx.h"
#include "Polynomial.h"

Polynomial::Polynomial() {};

Polynomial::Polynomial(const int a0, const int a1, const int a2, const int a3, const int a4, const int x)
	: x(x)
{
	a[0] = a0;
	a[1] = a1;
	a[2] = a2;
	a[3] = a3;
	a[4] = a4;
	val = value();
}

Polynomial::~Polynomial()
{
}

int Polynomial::value()
{
	return a[0] + a[1] * x + a[2] * x*x + a[3] * x*x*x + a[4] * x*x*x*x;
	//return x;
}

const int * Polynomial::getCoefficients()
{
	return a;
}

const int Polynomial::getX()
{
	return x;
}

std::ostream& operator <<(std::ostream& f, Polynomial &p) {
	const int* coeff = p.getCoefficients();
	for (int i = 0; i < 5; i++) {
		f << coeff[i] <<" ";
	}
	f << p.getX();
	f << " = " << p.value();
	f << std::endl;
	return f;
}


std::istream& operator >>(std::istream& f, Polynomial &p) {
	int coeff[5];
	for (int i = 0; i < 5; i++) {
		f >> coeff[i];
	}
	int x = 0;
	f >> x;
	p.set(coeff[0], coeff[1], coeff[2], coeff[3], coeff[4], x);
	//p.set(1, 1, 1,1,1, x);
	return f;
}

void Polynomial::set(int a0, int a1, int a2, int a3, int a4, int x) {
	a[0] = a0;
	a[1] = a1;
	a[2] = a2;
	a[3] = a3;
	a[4] = a4;
	this->x = x;
	val = value();
}