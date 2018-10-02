#pragma once
#include "STACK.h"



STACK::STACK()
	:top(nullptr)
{
}


STACK::~STACK()
{
	clear();
	delete[] top;
}

void STACK::clear() {
	while (top != nullptr) {
		pop();
	}

}

void STACK::push(char* zn)
{
		if (top == nullptr) {
			top = new symbol;
			top->znak = zn;
			top->next = nullptr;
		}
		else {
			symbol* nowy = new symbol;
			nowy->next = top;
			top = nowy;
			top->znak = zn;
		}
	
}

void STACK::push(int liczba)
{
	char* zn =toChar(liczba);
	if (top == nullptr) {
		top = new symbol;
		top->znak = zn;
		top->next = nullptr;
	}
	else {
		symbol* nowy = new symbol;
		nowy->next = top;
		top = nowy;
		top->znak = zn;
	}

}
char* STACK::pop()
{
		if (top != nullptr) {
			char* retznak;
			if (top->next != nullptr) {
				retznak = top->znak;
				symbol* tmp = top;
				top = tmp->next;
				delete tmp;
			}
			else {
				retznak = top->znak;
				symbol* tmp = top;
				top = nullptr;
				delete tmp;
			}
			return retznak;
		}
		else {
			return NULL;
		}
	}
const symbol * const STACK::getTop()
{
	return top;
}
