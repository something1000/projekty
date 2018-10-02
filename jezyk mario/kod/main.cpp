// projekt2.cpp : Defines the entry point for the console application.
//
#pragma once
#include <iostream>
#include "STACK.h"
#include "tools.h"
#include "MyQUEUE.h"
#include "SLOWNIK.h"
#include "value.h"
using namespace std;
SLOWNIK zmienne;
/////
STACK stos;
MyQUEUE kolejka;
//////
int licznik;
int stan_licznika;
int ile_lini;
bool returned_val;
int przeskok;
bool isONPOperand(char* znak);
void conditionInstruction(char* buffer, char** saved);
void whileLoop(char*buffer, char** saved);
/////////////////////***************** DO ONP **************//////////


bool isOperand(char* znak, char prev) {
	if ((*znak >= 'a' && *znak <= 'z') || (*znak >= 'A' && *znak <= 'Z') || (*znak >= '0' && *znak <= '9')) return true;
	switch (*znak) {
		/*case '!':
		case '=':
		case '<':
		case '>':
		case '*':
		case '|':
		case '/':
		case '&':
		case '%':
		case '(':
		case'_':
		case ')':
		return false;
		break;*/
	case '-':
	case '+':
		if (!isOperand(&prev, ' ')) return true;
		else return false;
		break;
	}

	//if ((znak >= '0' && znak <= '9') || (znak>='A' && znak<='Z') || (znak >= 'a' && znak <= 'z')) return true;
	return false;
}

bool isOperand(char* znak) {
	if ((*znak >= 'a' && *znak <= 'z') || (*znak >= 'A' && *znak <= 'Z') || (*znak >= '0' && *znak <= '9')) return true;
	switch (*znak) {
		/*case '!':
		case ' ':
		case '=':
		case '<':
		case '>':
		case '*':
		case '|':
		case '/':
		case '&':
		case '%':
		case '(':
		case'_'://-unarny
		case ')':
		case '~'://+unarny
		return false;
		break;*/
	case '-':
	case '+':
		if (*(znak - 1) == ')') return false;
		if (isOperand(znak + 1) && !isOperand(znak - 1))return true;
		return false;
		break;
	}

	//if ((znak >= '0' && znak <= '9') || (znak>='A' && znak<='Z') || (znak >= 'a' && znak <= 'z')) return true;
	return false;
}

int getPriority(char* znak) {
	int ile_z = 0;

	while (znak[ile_z] != '\0') ile_z++;

	switch (ile_z) {
	case 2:
		switch (*znak) {
		case '!':
		case '=':
			return 3;
			break;
		case '<':
		case '>':
			return 4;
			break;
		}
		break;
	case 1:
		switch (*znak) {
		case '=':
			return 0;
			break;
		case '|':
			return 1;
			break;
		case '&':
			return 2;
			break;
		case '<':
			return 4;
			break;
		case '>':
			return 4;
			break;
		case '+':
			return 5;
			break;
		case '-':
			return 5;
			break;
		case '*':
			return 6;
			break;
		case '/':
			return 6;
			break;
		case '%':
			return 6;
			break;
		case '!':
		case '_':
			return 7;
			break;
		}
	}
	return 0;
}

char* getToken(char* line, int &ile, char prev) {
	ile = 1;
	char check = prev;
	switch (*line) {
		/*---------dwuargumentowe---------*/
	case '!':
		if (*(line + 1) == '=') {
			ile = 2;
			return "!=";
		}
		else return "!";
		break;
	case '=':
		if (*(line + 1) == '=') {
			ile = 2;
			return "==";
		}
		else return "=";
		break;
	case '<':
		if (*(line + 1) == '=') {
			ile = 2;
			return "<=";
		}
		else return "<";
		break;
	case '>':
		if (*(line + 1) == '=') {
			ile = 2;
			return ">=";
		}
		else return ">";
		break;
		/*---------koniec dwuargumentowe---------*/
	case '|':
		return "|";
		break;
	case '&':
		return "&";
		break;

	case '/':
		return "/";
		break;
	case '*':
		return "*";
		break;
	case '%':
		return "%";
		break;
	case '(':
		return "(";
		break;
	case ')':
		return ")";
		break;
	case '+':
		if (isOperand(&check) || check == '(') {
			return "+";
		}
	case '-':
		if (isOperand(&check) || check == '(') {
			return "-";
		}
		else if (*(line + 1) > '9') {
			return "_";
		}
	default:
		bool minus = false;
		if (!isOperand(&prev) && *line == '-') {
			minus = true;
		}
		int i = minus ? 1 : 0;
		char* ret = new char[10];
		/*while (*(line + i) != '!' || *(line + i) != '=' || *(line + i) != '+' ||
		*(line + i) != '-' || *(line + i) != '<' || *(line + i) != '>' ||
		*(line + i) != '*' || *(line + i) != '!' || *(line + i) != '&' ||
		*(line + i) != '%' || *(line + i) != '|' || *(line + i) != '/')*/
		char prev = ' ';
		while (isOperand(line + i, prev) && line[i] != '\0') {
			ret[i] = line[i++];
			prev = ret[i - 1];
		}
		ret[i] = '\0';
		if (minus) ret[0] = '-';
		ile = i;
		return ret;
		break;
	}



}

char* toONP(char* line, char* onp) {

	char* token;// = new char[10];
	char  prev = ' ';
	char *top;// = new char[10];
	char* item;// = new char[10];
	int priorytet;
	int ile_znakow = 0;
	int i = 0;
	while (line[i] != '\0') {
		//token = line[i++];
		if (line[i] == ' ' || line[i] == '\t') {
			i++;
			continue;
		}
		token = getToken(line + i, ile_znakow, prev);
		if (isONPOperand(token)) kolejka.push(token);
		else if (*token == '(') stos.push(token);
		else if (*token == ')') {
			while (stos.getTop() != nullptr) {
				token = stos.pop();
				if (*token == '(') break;
				else kolejka.push(token);
			}
		}
		else if ((*token == '=' || *token == '!' || *token == '_') && ile_znakow == 1) {
			priorytet = getPriority(token);
			while (stos.getTop() != nullptr) {
				top = stos.pop();
				if (getPriority(top) <= priorytet) {
					stos.push(top);
					break;
				}
				kolejka.push(top);
			}
			stos.push(token);
		}
		else {
			priorytet = getPriority(token);
			while (stos.getTop() != nullptr) {
				top = stos.pop();
				if (*top == '(' || getPriority(top) < priorytet) {
					stos.push(top);
					break;
				}
				kolejka.push(top);
			}
			stos.push(token);
		}
		prev = token[ile_znakow - 1];
		i += ile_znakow;
	}


	while (stos.getTop() != nullptr) {
		item = stos.pop();
		kolejka.push(item);
	}

	char* buf;// = new char[1000];
			  //char* onp = new char[10000];
	int k = 0, j = 0;
	while (kolejka.getLast() != nullptr) {
		buf = kolejka.pop();
		while (buf[k] != '\0') {
			onp[j++] = buf[k++];
		}
		onp[j] = ',';
		j++;
		k = 0;

	}
	onp[j] = '\0';
	kolejka.clear();
	stos.clear();
	return onp;
}

/********************///////////////// PO ONP //////////////**********



					  /***************************OBLICZANIE ONP*/


bool isONPOperand(char* znak) {
	if (*znak == '-') {
		if ((*(znak + 1) >= 'a' && *(znak + 1) <= 'z') || (*(znak + 1) >= 'A' && *(znak + 1) <= 'Z') || (*(znak + 1) >= '0' && *(znak + 1) <= '9')) return true;

	}

	if ((*znak >= 'a' && *znak <= 'z') || (*znak >= 'A' && *znak <= 'Z') || (*znak >= '0' && *znak <= '9')) return true;
	return false;
}


char* getONPToken(char* onp, int &ile) {
	char* token = new char[10];
	int i = 0;
	while (onp[i] != ',' && onp[i] != '\0') {
		token[i] = onp[i];
		i++;
	}
	token[i] = '\0';
	ile = i;
	return token;
}

value doCalculation(char* token, char* op1, char* op2) {
	//int a = zmienne.getWordVal(op1);
	//int b = zmienne.getWordVal(op2);

	value ax = zmienne.getWordVal(op1);
	value bx = zmienne.getWordVal(op2);

	value result(0, false);
	int i = 0;
	while (token[i] != '\0')i++;
	stan_licznika++;

	if (ax.nulx || bx.nulx) {
		if (token[0] != '>' && token[0] != '<' && token[0] != '=' && token[0] != '|' && token[0] != '&') {
			result.val = -1656190;
			result.nulx = true;
			return result;
		}
	}

	/*if ((a == nul || b == nul)) {
	if (token[0] != '>' && token[0] != '<' && token[0] != '=' && token[0] != '|' && token[0] != '&') {
	return nul;
	}
	}*/
	switch (i) {
	case 2:
		switch (*token) {
		case '!':
			if (ax.val != bx.val) {
				result.val = 0; result.nulx = false;
			}
			else {
				result.val = -1656190; result.nulx = true;
			}
			break;
		case '=':
			if (ax.val == bx.val) {
				result.val = 0; result.nulx = false;
			}
			else {
				result.val = -1656190; result.nulx = true;
			}
			break;
		case '<':

			if (ax.val <= bx.val) {
				result.val = 0; result.nulx = false;
			}
			else {
				result.val = -1656190; result.nulx = true;
			}
			break;
		case '>':
			if (ax.val >= bx.val || ax.nulx == bx.nulx) {
				result.val = 0; result.nulx = false;
			}
			else {
				result.val = -1656190; result.nulx = true;
			}
			break;
		}
		break;
	case 1:
		switch (*token) {
		case '=':
			zmienne.addWord(op1, bx);
			result.val = 0; result.nulx = false;
			break;
		case '|':
			if (ax.nulx == true && bx.nulx == true) {
				result.val = -1656190; result.nulx = true;
			}
			else {
				result.val = 0; result.nulx = false;
			}
			break;
		case '&':
			if (ax.nulx == true || bx.nulx == true) {
				result.val = -1656190; result.nulx = true;
			}
			else {
				result.val = 0; result.nulx = false;
			}
			break;
		case '<':
			if (ax.val < bx.val) {
				result.val = 0; result.nulx = false;
			}
			else {
				result.val = -1656190; result.nulx = true;
			}
			break;
		case '>':
			if (ax.val > bx.val) {
				result.val = 0; result.nulx = false;
			}
			else {
				result.val = -1656190; result.nulx = true;
			}
			break;
		case '+':
			result.val = ax.val + bx.val; result.nulx = false;

			break;
		case '-':
			result.val = ax.val - bx.val; result.nulx = false;
			break;
		case '*':
			result.val = ax.val * bx.val; result.nulx = false;
			break;
		case '/':
			if (bx.val == 0) {
				result.val = -165619; result.nulx = true;
			}
			else {
				result.val = ax.val / bx.val; result.nulx = false;
			}
			break;
		case '%':
			result.val = ax.val % bx.val; result.nulx = false;
			break;
		case '!':
			result.val = 0; result.nulx = false;
			break;
		}
	}
	return result;
}

char* doEqCalculation(char* token, char* op1, char* op2) {
	stan_licznika++;
	zmienne.addWord(op1, zmienne.getWordVal(op2));
	return op1;
}
value doUnCalculation(char* token, char* op1) {
	stan_licznika++;
	value result(0, true);
	result.val = -zmienne.getWordVal(op1).val;
	result.nulx = false;
	if (zmienne.getWordStatus(op1) == true) {
		result.val = -1656190; result.nulx = true;
	}
	return result;
}

value doNotCalculation(char* token, char*op) {
	stan_licznika++;
	value result(0, true);
	if (zmienne.getWordStatus(op) || zmienne.getWordVal(op).nulx) {
		result.val = 0;
		result.nulx = false;
	}
	return result;
}

void calcONP(char* onp, int tryb = 0) {
	STACK stos;
	int olk = 1;
	int i = 0;
	i = 0;
	int k = 0;
	char* token;// = new char[100];
	value result(0, true);
	char* equal;// = new char[100];
				//cout << onp[i - 1];
	while (onp[i] != '\0') {
		if (stan_licznika >= licznik) break;
		token = getONPToken(onp + i, i);
		onp += (i);
		olk += i;
		if (onp[0] == ',') { onp += 1; olk += 1; }
		if (isONPOperand(token)) {
			stos.push(token);
		}
		else {
			//cout << "LICZE" << endl;
			if (token[0] == '=' && i == 1) {
				equal = doEqCalculation(token, stos.pop(), stos.pop());
				stos.push(equal);
			}
			else if (token[0] == '!' && i == 1) {
				result = doNotCalculation(token, stos.pop());
				if (result.nulx) stos.push("~");
				else stos.push(result.val);
				//stos.push(result.nulx ? "~" : result.val);
			}
			else if (token[0] == '_' && i == 1) {
				result = doUnCalculation(token, stos.pop());
				if (result.nulx) stos.push("~");
				else stos.push(result.val);
				//stos.push(result.nulx ? "~" : result.val);
			}
			else {
				result = doCalculation(token, stos.pop(), stos.pop());
				if (result.nulx) stos.push("~");
				else stos.push(result.val);
				//stos.push(result.nulx ? "~" : result.val);
			}
		}
		i = 0;
	}
	if (tryb == 1) {
		if (stan_licznika >= licznik) {
			returned_val = false;
			return;
		}
		char* x = stos.pop();
		if (zmienne.getWordStatus(x) == true /*|| zmienne.getWordVal(x) == nul*/) {
			returned_val = false;
		}
		else {
			returned_val = true;
		}
	}
	while (stos.getTop() != nullptr) {
		stos.pop();
	}
	onp -= olk;
	//delete[] token;
	//delete[] equal;
}


int skipBlock(char* buffer, char** saved) {
	int ile_naw = 1;
	int i = 0, k = 1;
	int line = 0;
	//char* buffer = new char[1000];
	while (ile_naw) {

		buffer = saved[k++];
		line++;
		while (buffer[i] != '\0') {
			if (buffer[i] == '}')ile_naw--;
			else if (buffer[i] == '{')ile_naw++;
			i++;
		}
		i = 0;
	}
	return line + 1;
}




void conditionInstruction(char* buffer, char** saved) {
	przeskok = 0;
	int i = 1, k = 0;
	int lk = 0;
	int licznik_naw = 1;
	while (buffer[i] != '(') i++;
	char warunek[1000];// = new char[1000];
	i++;
	while (buffer[i] != ')' || licznik_naw>0) {
		warunek[k++] = buffer[i];
		if (buffer[i] == '(') licznik_naw++;
		i++;
		if (buffer[i] == ')') licznik_naw--;

	}
	warunek[k] = '\0';
	toONP(warunek, warunek);
	calcONP(warunek, 1);
	stan_licznika++;
	//delete[] warunek;
	if (returned_val == true) {


		int i = 0;
		k = 1;
		przeskok = 0;
		bool quit = false;
		char* onp = new char[1000];
		while (!quit) {
			buffer = saved[k + przeskok];
			przeskok = 0;
			while (buffer[i] != '}' && buffer[i] != '\0') {
				i++;
			}
			if (buffer == "end" || buffer[i] == '}') {
				quit = true;
				i++;
				continue;
				//buffer[i] = '\0';
			}

			while (buffer[0] == '\t' || buffer[0] == ' ') {
				buffer += 1;
				lk += 1;
			}

			switch (buffer[0]) {
			case '@':
				//skipBlock(buffer);
				whileLoop(buffer, saved + k + 1);
				break;
			case '?':
				conditionInstruction(buffer, saved + 1 + k);
				break;
			default:
				toONP(buffer, onp);
				calcONP(onp);
				break;
			}
			i = 0;
			k++;
		}
		buffer -= lk;
		delete[] onp;
		przeskok = k;
		//readLine(buffer);
	}
	else {
		przeskok = skipBlock(buffer, saved) - 1;
	}

}



void whileLoop(char* buffer, char** saved) {
	int i = 1, k = 0;
	int lk = 0;
	int licznik_naw = 1;
	while (buffer[i] != '(') i++;
	char warunek[1000];// = new char[1000];
	i++;
	while (buffer[i] != ')' || licznik_naw>0) {
		warunek[k++] = buffer[i];
		if (buffer[i] == '(') licznik_naw++;
		i++;
		if (buffer[i] == ')') licznik_naw--;

	}
	warunek[k] = '\0';
	toONP(warunek, warunek);
	calcONP(warunek, 1);
	stan_licznika++;

	k = 0;
	char *onp = new char[1000];
	if (returned_val == true) {
		//saved = readAndSaveLine(buffer);
	}
	else {
		przeskok = skipBlock(buffer, saved);
		return;
	}
	przeskok = 1;
	while (returned_val) {
		k += przeskok;
		buffer = saved[k++];
		przeskok = 0;
		i = 0;
		while (buffer[i] != '}' && buffer[i] != '\0') {
			i++;
		}
		if (buffer == "end" || buffer[i] == '}') {
			calcONP(warunek, 1);
			if (stan_licznika >= licznik) {
				returned_val = false;

				continue;
			}
			stan_licznika++;

			if (returned_val == true) {
				k = 1;
			}
			continue;
			//buffer[i] = '\0';
		}


		/*if (buffer == "end") {
		calcONP(warunek, 1);
		k = 0;
		continue;
		}*/
		while (buffer[0] == '\t' || buffer[0] == ' ') {
			buffer = buffer + 1;
			lk += 1;
		}
		switch (buffer[0]) {

		case '?':
			conditionInstruction(buffer, saved + k - 1);
			//skipBlock(buffer);
			break;
		case '@':
			whileLoop(buffer, saved + k - 1);
			przeskok--;
			//skipBlock(buffer);
			break;
		default:
			toONP(buffer, onp);
			calcONP(onp);
			break;
		}
		returned_val = true;

	}
	przeskok = k;
	buffer -= lk;
	//delete[] warunek;
	delete[] onp;

}


char** readLines(char*buffer, char**saved) {


	//char* buffer = new char[1000];
	int i = 0, k = 0, j = 0;
	int quit = 1;
	bool space = false;
	char prev = ' ';
	int licznika = 0;
	//saved[k] = new char[75000];
	//while (!cin.eof()) {
	//	cin.getline(buffer, 1000);
while (scanf("%s", buffer) != EOF){
		while (buffer[i] != '\0') {
			if (prev == '{') {
				saved[k][j] = '\0';
				//cout << saved[k]<<endl;
				k++;
				saved[k] = new char[75000];
				while (buffer[i] == ' ' || buffer[i] == '\t') {
					i++;
				}
				space = false;
				j = 0;
				saved[k][j] = buffer[i];
				prev = buffer[i];
				j++;
				i++;
				continue;
			}
			if (buffer[i] == '}') {
				saved[k][j] = '\0';
				//cout << saved[k] << endl;
				k++;
				saved[k] = new char[75000];
				j = 0;
				saved[k][j] = buffer[i];
				saved[k][++j] = '\0';
				prev = buffer[i];
				//cout << saved[k] << endl;
				k++;
				saved[k] = new char[75000];
				j = 0;
				i++;
				continue;
			}
			if (buffer[i] == '@' || buffer[i] == '?') {
				saved[k][j] = '\0';
				k++;
				saved[k] = new char[75000];
				j = 0;
				saved[k][j] = buffer[i];
				prev = buffer[i];
				space = false;
				j++;
				i++;
				////cout << saved[k - 1] << endl;
				continue;
			}
			if (buffer[i] == ' ' || buffer[i] == '\t') {
				space = true;
			}
			else if (space) {
				if (buffer[i] == '(' && isOperand(&prev)) {
					saved[k][j] = '\0';
					//cout << saved[k] << endl;
					k++;
					saved[k] = new char[75000];
					space = true;
					j = 0;
				}
				else if (prev == ')' && isOperand(&buffer[i])) {
					saved[k][j] = '\0';
					//cout << saved[k] << endl;
					k++;
					saved[k] = new char[75000];
					space = true;
					j = 0;
				}
				else if (isOperand(&prev) && isOperand(&buffer[i])) {
					saved[k][j] = '\0';
					//cout<<saved[k] << endl;
					space = true;
					k++; j = 0;
					saved[k] = new char[75000];
				}

				saved[k][j] = buffer[i];
				prev = buffer[i];
				space = false;
				j++;
			}
			else {
				saved[k][j] = buffer[i];
				prev = buffer[i];
				space = false;
				j++;
			}
			i++;
		}
		i = 0;
		space = true;

	}
	saved[k][j] = '\0';
	saved[k + 1] = new char[3];
	saved[k + 1][0] = '#';
	saved[k + 1][1] = '\0';
	ile_lini = k + 1;
	return saved;
}

int main()
{
	//zmienne.test();
	MyQUEUE wyjscie;
	STACK sto;


	/*wyjscie.push("a");
	wyjscie.push("b");
	wyjscie.push("c");*/

	char* buffer = new char[1000];

	char* onp = new char[75000];
	//delete[] onp;
	//char **saved = (char**)malloc(sizeof(char*) * 50);
	char **saved = new char*[20];
	//for (int j = 0; j < 20; j++) {
	saved[0] = new char[75000];
	//}


	char zmienne_wyjscie[10000];
	cin >> licznik;
	cin.ignore();
	cin.getline(zmienne_wyjscie, 1000);
	int i = 0;
	int k = 0;
	bool spacja = false;
	char* buf = new char[10];

	while (zmienne_wyjscie[i] != '\0') {


		if (zmienne_wyjscie[i] == ' ') {
			//buf[k] = '\0';
			spacja = true;

		}
		else if (spacja) {
			buf[k] = '\0';
			wyjscie.push(buf);
			k = 0;
			buf = new char[10];
			buf[k++] = zmienne_wyjscie[i];
			spacja = false;
		}
		else {
			spacja = false;
			buf[k++] = zmienne_wyjscie[i];
		}
		i++;
	}
	buf[k] = '\0';
	wyjscie.push(buf);

	//saved = 
	readLines(buffer, saved);
	/*while (cin.eof()) {
	cin.getline(buffer, 1000);
	}*/
	k = 0;
	delete[] buffer;
	buffer = nullptr;
	while (licznik - stan_licznika > 0) {
		k = k + przeskok;
		buffer = saved[k];
		if (buffer[0] == '#') {
			break;
		}
		else if (buffer[0] == '}') {
			przeskok = 0;
			k++;
		}
		else {
			//cout << a=b;

			//cout << "----" << buffer << endl;
			//cin.getline(buffer, 1000);
			//buffer = saved[k];
			/*while (buffer[0] == '\t' || buffer[0] == ' ') {
			buffer = buffer + 1;
			}*/

			switch (buffer[0]) {

			case '?':
				//saved = readAndSaveLine();
				conditionInstruction(buffer, saved + k);
				//skipBlock(buffer);
				break;
			case '@':
				//saved = readAndSaveLine();
				whileLoop(buffer, saved + k);
				//skipBlock(buffer);
				break;
			default:
				toONP(buffer, onp);
				calcONP(onp);
				przeskok = 1;
				break;
			}

			//cout <<	
			//<<endl;

		}
	}


	//cout << stan_licznika << endl;
	printf("%d\n", stan_licznika);
	while (wyjscie.getLast() != nullptr) {
		buf = wyjscie.pop();
		//cout << buf;
		printf("%s", buf);
		if (!zmienne.getWordStatus(buf) && !zmienne.getWordVal(buf).nulx) {
			//cout << " " << zmienne.getWordVal(buf) << endl;
			printf(" %d\n", zmienne.getWordVal(buf));
		}
		else {
			printf(" Nul\n");
			//cout << " Nul" << endl;
		}
	}
	int z = 0;
	for (z; z<ile_lini - 1; z++) {
		delete[] saved[z];
	}
	delete buf;
	delete saved;
	//delete onp;
	//delete saved; na 3 tescie nie zwalnia poprawnie - nie wiem czemu
	return 0;
}