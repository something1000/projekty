#include "MyQUEUE.h"



MyQUEUE::MyQUEUE()
	:head(nullptr),last(nullptr)
{
}


MyQUEUE::~MyQUEUE()
{
	clear();
	delete head;
	delete last;
}

void MyQUEUE::clear() {
	while (head != nullptr) {
		delete[] pop();
	}
}

void MyQUEUE::push(char* zn)
{
		if (head == nullptr) {
			head = new operand;
			head->znak = zn;
			head->next = nullptr;
			last = head;
		}
		else {
			operand* nowy = new operand;
			last->next = nowy;
			last = nowy;
			last->znak = zn;
			last->next = nullptr;
		}
	
}

char* MyQUEUE::pop()
{
		if (head != nullptr) {
			char* retznak;
			if (head->next != nullptr) {
				retznak = head->znak;
				operand* tmp = head;
				head = tmp->next;
				delete tmp;
			}
			else {
				retznak = head->znak;
				delete head;
				head = nullptr;
				last = nullptr;
			}
			return retznak;
		}
		else {
			return NULL;
		}
}
	

const operand * const MyQUEUE::getHead()
{
	return head;
}

const operand * const MyQUEUE::getLast()
{
	return last;
}
