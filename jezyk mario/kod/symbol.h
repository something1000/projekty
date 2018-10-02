#pragma once
struct symbol {
	char* znak;
	symbol* next;
	symbol() {
		znak = nullptr;
	}
};