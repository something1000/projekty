#pragma once
#include <iostream>
#include "operand.h"
class MyQUEUE
{
public:
	MyQUEUE();
	~MyQUEUE();
	void push(char* zn);
	char* pop();
	void clear();
	const operand* const getHead();
	const operand* const getLast();
private:
	operand* head;
	operand* last;
};

