#pragma once
#include "slowo.h"
#include "tools.h"
#include "value.h"
#include <iostream>
class SLOWNIK
{
public:
	SLOWNIK();
	~SLOWNIK();
	void clearMem(slowo* sl);
	void addWord(char* word, value val);
	value getWordVal(char* word);
	bool getWordStatus(char* word);
	void test();
	
private:

	slowo alfabet[52];
};

