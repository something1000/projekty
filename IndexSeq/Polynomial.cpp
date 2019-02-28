#include "Polynomial.h"



Polynomial::Polynomial()
{
}

Polynomial::Polynomial(int key, int a0, int a1, int a2, int a3, int a4, int x)
	:key(key),x(x)
{
	a[0] = a0;
	a[1] = a1;
	a[2] = a2;
	a[3] = a3;
	a[4] = a4;
}


Polynomial::~Polynomial()
{
}

int* Polynomial::getCoefficient()
{
	return a;
}

int Polynomial::getX()
{
	return x;
}

int Polynomial::getKey()
{
	return key;
}

int Polynomial::Value()
{
	return a[0] + a[1]*x + a[2]*x*x + a[3]*x*x*x + a[4]*x*x*x*x;
}

void Polynomial::setKey(int key) {
	this->key = key;
}

int Polynomial::overflowAddr()
{
	return ofAddress;
}

void Polynomial::setOverflowAddr(int address)
{
	this->ofAddress = address;
}
