// Sortowanie.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Polynomial.h"
#include <iostream>
#include "DiskTape.h"
#include <stdlib.h>
#include <string.h>
#include <ctime>

int numberOfSeries;
char nazwa[20] = "2.bin";
char textNazwa[20] = "";
bool tapePrev = false;
int numberOfSerie[3];
int dummySeries = 0;
int dummyOnTapeNr = 0;
int odczyty;
int zapisy;
int liczbaFaz = 0;

void sprawdz(const char* name);
void fibSeries();
void poliMerge();
int checkForFibonacci(int a, int b);

int showMenu();

void showLiczbaSerii();
void generateRandomRecords(int numOfRecords);
void loadRecordsFile(const char* nazwa);
void readRecordsFromUser(int numOfRecords);

int main()
{
	srand(time(NULL));
	odczyty = 0;
	zapisy = 0;
	while (showMenu() != 0);

//	if (tapePrev) {
		sprawdz(nazwa);


	fibSeries();
	if (tapePrev) {
		sprawdz("0.bin");
		sprawdz("1.bin");
	}
	showLiczbaSerii();
	if (numberOfSerie[0] + numberOfSerie[1] + numberOfSerie[2] > 1) {
		std::cout << std::endl << "ETAP SCALANIA" << std::endl;
		poliMerge();
	}

	std::cout << "\nLiczba odczytow: " << odczyty;
	std::cout << "\nLiczba zapisow: " << zapisy;
	std::cout << "\nLiczba etapow: " << liczbaFaz;
    return 0;
}


int showMenu() {
	char option[20];
	int c;
	bool done = false;
	int numOfRecords = 0;
		std::cout << "Sortowanie Polifazowe" << std::endl;
		std::cout << "1. Wprowadz rekordy do posortowania." << std::endl;
		std::cout << "2. Wygeneruj losowe rekordy do posortowania" << std::endl;
		std::cout << "3. Wczytaj plik rekordow do posortowania: "<<textNazwa<< std::endl;
		std::cout << "9. Podgląd taśm po każdej fazie" << std::endl;
		std::cout << "Dowolna litera konczy dzialanie programu" << std::endl;

		std::cin >> option;
	switch (atoi(option)) {
		case 1:	
			while (numOfRecords == 0) {
				std::cout << "Wprwadz liczbe rekordow do wprowadzenia: ";
				std::cin >> option;
				numOfRecords = atoi(option);
			}
			readRecordsFromUser(numOfRecords);
			return 0;
		case 2:
			while (numOfRecords == 0) {
				std::cout << "Wprwadz liczbe rekordow do wygenerowania: ";
				std::cin >> option;
				numOfRecords = atoi(option);
			}
			generateRandomRecords(numOfRecords);
			return 0;
		case 3:
			std::cout << "Wprowadz nazwe pliku do wczytania: ";
			std::cin >> nazwa;
			strcpy(textNazwa, nazwa);
			if (FILE* f = fopen(nazwa, "r")) {
				fclose(f);
				done = true;
				return 0;
			}
			else {
				std::cout << "Taki plik nie istnieje !";
				return 1;
			}
			break;
		case 9:
			if (tapePrev) std::cout << "Podglad wylaczony" << std::endl;
			else std::cout << "Podglad wlaczony" << std::endl;
			tapePrev = !tapePrev;
			return 1;
			break;
		default:
			return -1;
		}
}

void showLiczbaSerii() {
	if (dummySeries == 0) {
		std::cout << "Liczba serii na tasmach: " << numberOfSerie[0] << " | " << numberOfSerie[1] << " | " << numberOfSerie[2] << std::endl;
	} else {
		switch (dummyOnTapeNr) {
		case 0:
			std::cout << "Liczba serii na tasmach: " << numberOfSerie[0] - dummySeries << " (" << dummySeries << ")" << " | " << numberOfSerie[1] << " | " << numberOfSerie[2] << std::endl;
			break;
		case 1:
			std::cout << "Liczba serii na tasmach: " << numberOfSerie[0] << " | " << numberOfSerie[1] - dummySeries << " (" << dummySeries << ")" << " | " << numberOfSerie[2] << std::endl;
			break;
		case 2:
			std::cout << "Liczba serii na tasmach: " << numberOfSerie[0] << " | " << numberOfSerie[1] << " | " << numberOfSerie[2] - dummySeries << " (" << dummySeries << ")" << std::endl;
			break;
		}
	}
	
}


void generateRandomRecords(int numOfRecords) {

	Polynomial p1;
	int zapis = 0;
	int odcz = 0;
	DiskTape dts("2.bin", &odcz, &zapis, true);
		//p1 = new Polynomial[numOfRecords%10000000];
		for (int i = 0; i < numOfRecords; i++) {
			p1 = Polynomial(rand() % 70, rand() % 70, rand() % 70, rand() % 70, rand() % 70, rand() % 70);
			dts.save(p1);
		}
	dts.close();
}

void readRecordsFromUser(int numOfRecords) {
	Polynomial p1;
	int a[5], x;
	DiskTape dts("2.bin", &odczyty, &zapisy,true);
	//p1 = new Polynomial[numOfRecords];
	for (int i = 0; i < numOfRecords; i++) {
		std::cin >> a[0] >> a[1] >> a[2] >> a[3] >> a[4] >> x;
		p1 = Polynomial(a[0],a[1],a[2],a[3],a[4],x);
		dts.save(p1);
	}
	dts.close();
	sprawdz("2.bin");
}

void sprawdz(const char* name) {
	int od=0,z=0;
	DiskTape o(name, &od, &z);
	bool a;
	int licznik = 1;
	std::cout << std::endl << "---------- PODGLAD "<<name<<" ----------";
	std::cout << " \n";
	Polynomial x;
	x = o.read(&a);
	while (a) {
		std::cout <<licznik++<< ". "<< x;
		x = o.read(&a);
	}
	std::cout << std::endl << "---------- KONIEC " << name << " ----------";
	std::cout << std::endl;
}

void fibSeries() {
	//liczbaFaz++;
	int presentFibNumber = 1;
	int lastFibNumber = 0;
	Polynomial current;
	Polynomial compare;
	Polynomial lastSaved[3];
	bool readed = false;


	int currentTape_0 = 0;
	int currentTape_1 = 1;
	bool switched = false;
	bool cncat = false;
	int frstSerieIndex = 0;
	DiskTape mainTape(nazwa, &odczyty, &zapisy);
	DiskTape tapes[3];
	tapes[0].initDiskTape("0.bin", &odczyty, &zapisy, true);
	tapes[1].initDiskTape("1.bin", &odczyty, &zapisy, true);
	//std::queue<Polynomial> singlePage;
	current = mainTape.read(&readed);
	if (readed == false) return;

	bool firstSwitch = false;
	int counter = presentFibNumber;
	compare = current;
	lastSaved[currentTape_0] = current;
	while (counter > 0) {

			if (readed == false) {
				numberOfSerie[currentTape_0]++;
				if(firstSwitch) numberOfSerie[currentTape_1]++;
				counter--;
				break;
			}

		if (compare.value() <= current.value()){
			if (switched == true && lastSaved[currentTape_0].value() < current.value()){
				cncat = true;
			}

			switched = false;
			tapes[currentTape_0].save(current);
			compare = current;
			lastSaved[currentTape_0] = current;
		}
		else if(counter>1){
			if (lastSaved[currentTape_0].value() > current.value()){
				numberOfSerie[currentTape_0]++;
				counter--;
				if (switched) cncat = true;
			}

			switched = false;
			lastSaved[currentTape_0] = current;
			tapes[currentTape_0].save(current);
			compare = current;
		}
		else {
			
			switched = true;
			tapes[currentTape_1].save(current);
			compare = current;
			counter = presentFibNumber;
			presentFibNumber = presentFibNumber + lastFibNumber;
			lastFibNumber = counter;
			if (!firstSwitch)lastSaved[currentTape_1] = current;
			firstSwitch = true;
			if (lastSaved[currentTape_1].value() > current.value()){
				numberOfSerie[currentTape_1]++;
			}
			else if (switched == true && lastSaved[currentTape_1].value() <= current.value() && presentFibNumber>1) {
				counter++;
			}
			cncat = false;
			lastSaved[currentTape_1] = current;
			std::swap(currentTape_0, currentTape_1);

		}
		current = mainTape.read(&readed);
	}

	mainTape.close();
	dummySeries =  presentFibNumber - numberOfSerie[currentTape_0];
	numberOfSerie[currentTape_0] = presentFibNumber;
	dummyOnTapeNr = currentTape_0;
}


void poliMerge() {
	DiskTape tapes[3];
	tapes[0].initDiskTape("0.bin", &odczyty, &zapisy);
	tapes[1].initDiskTape("1.bin", &odczyty, &zapisy);
	tapes[2].initDiskTape("2.bin", &odczyty, &zapisy, true);
	int currentTape_0 = 0;
	int currentTape_1 = 1;
	int saveTape = 2;

	bool readed[3] = { false,false,false };
	bool getNew[3] = { false,false,false };
	//std::queue<Polynomial*> discPage[3];
	Polynomial currentRecord[3];
	Polynomial* backupPage;
	Polynomial compare[3];
	Polynomial lastResult;

#pragma region Dummy Series
	if (numberOfSerie[0] > numberOfSerie[1]) {
		currentTape_0 = 0;
		currentTape_1 = 1;
	}
	else {
		currentTape_1 = 1;
		currentTape_0 = 0;
	}
	saveTape = 2;
	currentTape_0 = !dummyOnTapeNr;
	currentTape_1 = !currentTape_0;
	if (dummySeries > 0) {
		currentRecord[currentTape_0] = tapes[currentTape_0].read(&readed[currentTape_0]);
		currentRecord[2] = Polynomial();
		if (readed[currentTape_0] == false) return;
		compare[currentTape_0] = currentRecord[currentTape_0];
		compare[currentTape_1] = currentRecord[currentTape_1];
		while (dummySeries > 0) {

			if (readed[currentTape_0] == false) {
				dummySeries--;
				numberOfSerie[currentTape_0]--;
				numberOfSerie[currentTape_1]--;
				numberOfSerie[saveTape]++;
				break;
			}

			if (compare[currentTape_0].value() <= currentRecord[currentTape_0].value()) {
				tapes[saveTape].save(currentRecord[currentTape_0]);//.
				compare[currentTape_0] = currentRecord[currentTape_0];
			}
			else if (dummySeries > 1) {
				dummySeries--;
				numberOfSerie[currentTape_0]--;
				numberOfSerie[currentTape_1]--;
				numberOfSerie[saveTape]++;
				tapes[saveTape].save(currentRecord[currentTape_0]);//.

				compare[currentTape_0] = currentRecord[currentTape_0];
			}
			else {
				numberOfSerie[currentTape_0]--;
				numberOfSerie[currentTape_1]--;
				numberOfSerie[saveTape]++;
				dummySeries--;
				break;
			}
			currentRecord[currentTape_0] = tapes[currentTape_0].read(&readed[currentTape_0]);
		}
	}
	else {
		int fibNumber = numberOfSerie[currentTape_0] - numberOfSerie[currentTape_1];
		if (fibNumber < 0) {
			fibNumber = fibNumber * -1;
			currentRecord[currentTape_0] = tapes[currentTape_0].read(&readed[currentTape_0]);
		}
		else {
			currentRecord[currentTape_1] = tapes[currentTape_1].read(&readed[currentTape_1]);
		}
	}

	compare[currentTape_0] = currentRecord[currentTape_0];
	if (numberOfSerie[0] + numberOfSerie[1] + numberOfSerie[2] < 2) currentTape_0 = 0;
#pragma endregion
	while (numberOfSerie[0] + numberOfSerie[1] + numberOfSerie[2] > 1) {
		liczbaFaz++;

		bool endOfSerie[3] = { false, false, false };
		bool newSerie[3] = { true, true, true };

		int fibNumber = numberOfSerie[currentTape_0] - numberOfSerie[currentTape_1];
		if (fibNumber < 0) {
			fibNumber = fibNumber*-1;
			std::swap(currentTape_0, currentTape_1);
		}

		currentRecord[currentTape_0] = tapes[currentTape_0].read(&readed[currentTape_0]);
		compare[currentTape_0] = currentRecord[currentTape_0];
		lastResult = currentRecord[currentTape_0];

		while (numberOfSerie[currentTape_1] > 0 || numberOfSerie[currentTape_0] > fibNumber) {

			if (endOfSerie[currentTape_0]) {

				if (currentRecord[currentTape_1].value() < compare[currentTape_0].value()) {
						endOfSerie[currentTape_1] = true;
					}else if (lastResult.value() > currentRecord[currentTape_1].value()) {
						endOfSerie[currentTape_1] = true;
					}
					else {
						tapes[saveTape].save(currentRecord[currentTape_1]);
						compare[currentTape_1] = currentRecord[currentTape_1];
						lastResult = compare[currentTape_1];
						getNew[currentTape_1] = true;
						newSerie[currentTape_0] = false;
					}
			}
			else if (endOfSerie[currentTape_1]) {
				if (currentRecord[currentTape_0].value() < compare[currentTape_0].value() && !newSerie[currentTape_0]) {
					endOfSerie[currentTape_0] = true;
				} else if (lastResult.value() > currentRecord[currentTape_0].value()) {
					endOfSerie[currentTape_0] = true;
				}
				else {
					tapes[saveTape].save(currentRecord[currentTape_0]);
					compare[currentTape_0] = currentRecord[currentTape_0];
					lastResult = compare[currentTape_0];
					getNew[currentTape_0] = true;
					newSerie[currentTape_0] = false;
				}
			}
			else if (lastResult.value() <= currentRecord[currentTape_0].value() || //czy ostatnia jest mniejsza od dowolnego
				lastResult.value() <= currentRecord[currentTape_1].value()) {
				if (currentRecord[currentTape_0].value() <= currentRecord[currentTape_1].value()) { // czy 1 jest mniejsze od 2
					if (compare[currentTape_0].value() > currentRecord[currentTape_0].value()
						&& !newSerie[currentTape_0])
					{	//czy ostatnia 1 jest wieksza od obecnej 1 i czy nie sa to nowe serie
						endOfSerie[currentTape_0] = true;
					}
					else {
						tapes[saveTape].save(currentRecord[currentTape_0]);
						compare[currentTape_0] = currentRecord[currentTape_0];
						lastResult = compare[currentTape_0];
						getNew[currentTape_0] = true;
						newSerie[currentTape_0] = false;
					}
				}
				else {
					if (compare[currentTape_1].value() > currentRecord[currentTape_1].value()
						&& !newSerie[currentTape_1]) {
						endOfSerie[currentTape_1] = true;
					}
					else {
						tapes[saveTape].save(currentRecord[currentTape_1]);
						compare[currentTape_1] = currentRecord[currentTape_1];
						lastResult = compare[currentTape_1];
						getNew[currentTape_1] = true;
						newSerie[currentTape_1] = false;
					}

				}
			}
			else {
				if (currentRecord[currentTape_0].value() <= currentRecord[currentTape_1].value()) {
					if (compare[currentTape_0].value() > currentRecord[currentTape_0].value()
						&& !newSerie[currentTape_0]) {
						endOfSerie[currentTape_0] = true;
					}
					else {
						tapes[saveTape].save(currentRecord[currentTape_0]);
						compare[currentTape_0] = currentRecord[currentTape_0];
						lastResult = compare[currentTape_0];
						getNew[currentTape_0] = true;
						newSerie[currentTape_0] = false;
					}
				}
				else {
					if (compare[currentTape_1].value() > currentRecord[currentTape_1].value()
						&& !newSerie[currentTape_1]) {
						endOfSerie[currentTape_1] = true;
					}
					else {
						tapes[saveTape].save(currentRecord[currentTape_1]);
						compare[currentTape_1] = currentRecord[currentTape_1];
						lastResult = compare[currentTape_1];
						getNew[currentTape_1] = true;
						newSerie[currentTape_1] = false;
					}
				}
			}

			if (getNew[currentTape_0] && !endOfSerie[currentTape_0]) {
				currentRecord[currentTape_0] = tapes[currentTape_0].read(&readed[currentTape_0]);
				getNew[currentTape_0] = false;
				if (readed[currentTape_0] == false) {
					endOfSerie[currentTape_0] = true;
				}
			}

			if (getNew[currentTape_1] && !endOfSerie[currentTape_1]) {
				currentRecord[currentTape_1] = tapes[currentTape_1].read(&readed[currentTape_1]);
				getNew[currentTape_1] = false;
				if (readed[currentTape_1] == false) {
					endOfSerie[currentTape_1] = true;
				}
			}

			if (endOfSerie[currentTape_0] && endOfSerie[currentTape_1]) {
				endOfSerie[currentTape_0] = false;
				endOfSerie[currentTape_1] = false;
				newSerie[currentTape_0] = true;
				newSerie[currentTape_1] = true;
				numberOfSerie[saveTape] ++;
				numberOfSerie[currentTape_0]--;
				numberOfSerie[currentTape_1]--;
			}
		}
		int fibDiffrence = checkForFibonacci(numberOfSerie[saveTape], numberOfSerie[currentTape_0]);

		if (fibDiffrence > 0) {
			dummySeries = fibDiffrence;
			numberOfSerie[saveTape] += dummySeries;
		}

		tapes[currentTape_1].removeContent();
		tapes[saveTape].reopen();

		if (tapePrev) {
			//char nazwa[6] = "0.bin";
			//nazwa[0] += currentTape_0;
			//sprawdz(nazwa);
			//nazwa[0] = '0' + saveTape;
			//sprawdz(nazwa);
			tapes[saveTape].showSeries();
			tapes[currentTape_0].showSeries();
			std::cout << "Liczba serii na tasmach: " << numberOfSerie[0] << " | " << numberOfSerie[1] << " | " << numberOfSerie[2] << std::endl;
		}

		std::swap(saveTape, currentTape_0);
		std::swap(saveTape, currentTape_1);
		endOfSerie[0] = false; endOfSerie[1] = false; endOfSerie[2] = false;
		newSerie[0] = true; newSerie[1] = true;	newSerie[2] = true;
	}
	char nazwa[6] = "0.bin";
	nazwa[0] = '0' + currentTape_0;
	//if(tapePrev)
	sprawdz(nazwa);
	std::cout << "Plik wynikowy: " << nazwa;

}

int checkForFibonacci(int a, int b) {
	if ((a == 1) && (b = 1)) return 0;
	int z = 1, y = 1, x = 0;
	while (z < a && z < b) {
		x = z + y;
		y = z;
		z = x;
	}
		if (a < b && b!= z+y) {
			return z + y - b;
		}
		else return z + y - a;
	
	return 0;
}