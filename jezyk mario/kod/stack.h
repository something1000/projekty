#pragma once
#include "symbol.h"
#include "tools.h"
#include <iostream>
class STACK
{
public:
	STACK();
	~STACK();
	void push(char* znak);
	void push(int liczba);
	char* pop();
	void clear();
	const symbol* const getTop();
private:
	symbol* top;
};

