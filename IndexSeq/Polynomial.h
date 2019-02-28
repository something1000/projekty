#pragma once
class Polynomial
{
public:
	Polynomial();
	Polynomial(int key, int a0, int a1, int a2, int a3, int a4, int x);
	~Polynomial();
	int* getCoefficient();
	int getX();
	int getKey();
	int Value();
	int overflowAddr();
	void setOverflowAddr(int address);
	char deleted = 0;
	void setKey(int key);
private:
	int a[5] = { 0 };
	int x = 0;
	int key = 0;
	int ofAddress = 0;
};

