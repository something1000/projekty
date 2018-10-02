#pragma once

struct slowo {
	char znak;
	int value;
	int nextCount;
	slowo** next;
	bool nulV;
	slowo() {
		next = new slowo*;
		nulV = true;
		value = -1656190;
		nextCount = 0;
	}
	void addNext() {
		slowo** tmp = next;
		next = new slowo*[++nextCount];
		for (int i = 0; i < nextCount - 1; i++) {
			next[i] = tmp[i];
			
		}
		delete tmp;
	}

}; 
