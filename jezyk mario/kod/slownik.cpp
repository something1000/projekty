#include "SLOWNIK.h"



SLOWNIK::SLOWNIK()
{
	int i = 0;
	for (i; i < 26; i++) {
		alfabet[i].znak = 'a' + i;
	}

	for (int j = i; j < i + 26; j++) {
		alfabet[j].znak = 'A' + (j-i);
	}
}




SLOWNIK::~SLOWNIK()
{
	for (int i = 0; i < 52; i++) {
		clearMem(&alfabet[i]);
	}
}

void SLOWNIK::clearMem(slowo* sl) {
	if (sl->nextCount == 0) {

	}
	else {
		for (int i = 0; i < sl->nextCount; i++) {
			clearMem(sl->next[i]);
			delete sl->next[i];
		}
delete[] sl->next;
	}

}

void SLOWNIK::addWord(char * word, value val)
{
	int i = 0;
	char buf;
	int minus='a';
	while (word[i] != '\0') i++;

	int j = 0;
	if (word[0] - 'a' < 0)minus = '0';
	if (word[0] < 'a') minus = 39;
	if (i <= 1) {
		alfabet[word[i-1] - minus].value = val.val;
		alfabet[word[i - 1] - minus].nulV = val.nulx;
		//if (val == nul || isNul) {
		/*if ( isNul) {
			alfabet[word[i - 1] - minus].nulV = true;
		}*/
	}
	else {


		slowo* temp = &alfabet[word[j++] - minus];
		while (j < i) {
			if (temp->nextCount == 0) {
				temp->next[0] = new slowo;
				temp->nextCount++;
				temp = temp->next[0];
				temp->znak = word[j];
			}else{
				bool flag = false;
				for (int i = 0; i < temp->nextCount; i++) {
					if (temp->next[i]->znak == word[j]) {
						temp = temp->next[i];
						flag = true;
						break;
					}
				}
				if (flag == false) {
					temp->addNext();
					temp->next[temp->nextCount - 1] = new slowo;
					temp = temp->next[temp->nextCount - 1];
					temp->znak = word[j];
				}
			}
			j++;
		}
		temp->value = val.val;
		temp->nulV = val.nulx;
		//if (val == nul || isNul) {
		/*if ( isNul) {
			//alfabet[word[i - 1] - minus].nulV = true;
			temp->nulV = true;
		}*/
	}

}

bool equalNull(char* word) {
	if (word[0] == '~') {
				return true;
		
	}
	return false;
}

value SLOWNIK::getWordVal(char * word)
{
	
	if (isNumber(word[0])) return value(toNumber(word),false);
	int i = 0;
	int minus='a';
	while (word[i] != '\0') i++;
	if (i >= 3) {
		if (equalNull(word))return value(true);
	}

	int j = 0;
	if (word[0] < 'a') minus = 39;
	if (word[0] - 'a' < 0 && word[0] - 'A' <0) minus = '0';

	
	if (i <= 1) return (alfabet[word[0] - minus].nulV ? value(true) : value(alfabet[word[0] - minus].value,false));
	slowo* temp = &alfabet[word[j++] - minus];
	while (j < i) {
		if (temp->nextCount == 0) {
			return 0;
		}
		else {
			bool flag = false;
			for (int i = 0; i < temp->nextCount; i++) {
				if (temp->next[i]->znak == word[j]) {
					temp = temp->next[i];
					flag = true;
					break;
				}
			}
			if (flag == false) {
				return value(true);
			}
		}

		j++;
	}
	return (temp->nulV ? value(true) : value(temp->value,false));
}

bool SLOWNIK::getWordStatus(char * word)
{
	//if (toNumber(word) == nul) return true;

	if (isNumber(*word)) return false;
	int i = 0;
	int minus = 'a';
	while (word[i] != '\0') i++;
	if (i >= 3) {
		if (equalNull(word))return true;
	}
	int j = 0;
	if (word[0] < 'a') minus = 39;


	if (i <= 1) return alfabet[word[0] - minus].nulV;
	slowo* temp = &alfabet[word[j++] - minus];
	while (j < i) {
		if (temp->nextCount == 0) {
			return true;
		}
		else {
			bool flag = false;
			for (int i = 0; i < temp->nextCount; i++) {
				if (temp->next[i]->znak == word[j]) {
					temp = temp->next[i];
					flag = true;
					break;
				}
			}
			if (flag == false) {
				return 0;
			}
		}

		j++;
	}
	return temp->nulV;
}

void SLOWNIK::test()
{
	if (alfabet != nullptr) {
		for (int i = 0; i < 52; i++) {
			std::cout<< alfabet[i].znak;
		}
	}
}
