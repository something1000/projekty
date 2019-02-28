#pragma once
#include <iostream>
#include <fstream>
class Polynomial
{
public:
	Polynomial();
	Polynomial(int a0, int a1, int a2, int a3, int a4, int x);
	~Polynomial();
	int value();
	friend std::ostream& operator <<(std::ostream& f,  Polynomial &p);
	friend std::istream& operator >>(std::istream& f, Polynomial &p);
	const int* getCoefficients();
	const int getX();
	void set(int a0, int a1, int a2, int a3, int a4, int x);
	int x;
private:
	int a[5];

	int val;

};

