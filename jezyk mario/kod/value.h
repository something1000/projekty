#pragma once
struct value {
	int val;
	bool nulx;
	value(int a) {
		val = a;
		nulx = false;
	}
	value(bool x) {
		val = -1656190;
		nulx = x;
	}
	value(int v, bool b){
		val = v;
		nulx = b;
	}
};