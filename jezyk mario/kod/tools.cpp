#pragma once
#include "tools.h"

int mpow(int a, int b) {
	int result = a;
	if (b == 0) return 1;
	for (int i = 1; i < b; i++) {
		result *= a;
	}
	return result;
}

char* toChar(int liczba) {
	bool min = false;
	if (liczba < 0) {
		liczba = -liczba;
		min = true;
	}
	char* znak = new char[10];
	int i = 1;
	while (liczba / mpow(10, i) != 0) {
		i++;
	}
	int k = i;
	if (min) {
		for (i -= 1; i >= 0; i--) {
			int minus = (liczba % 10);
			znak[i+1] = minus + '0';
			liczba -= minus;
			liczba /= 10;
		}
		znak[0] = '-';
		znak[k+1] = '\0';
	}else {
	for (i -= 1; i >= 0; i--) {
		int minus = (liczba % 10);
		znak[i] = minus + '0';
		liczba -= minus;
		liczba /= 10;
	}
	znak[k] = '\0';
	}
	return znak;
}


bool isNumber(char a) {
	if ((a >= 48 && a <= 57) || a == '-') return true;
	return false;
}

int toNumber(char* word) {
	int i = 0;
	int result = 0;
	bool minus = false;
	if (word[0] == '-') {
		i = 1;
		minus = true;
	}
	while (word[i] != '\0') {
		result *= 10;
		result = result + (word[i] - '0');
		i++;
	}
	return minus?-result:result;
}