#pragma once
struct operand {
	char* znak;
	operand* next;
	operand() {
		znak = nullptr;
	}
};