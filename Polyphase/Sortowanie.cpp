// Sortowanie.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Polynomial.h"
#include <iostream>
#include "DiskTape.h"
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <queue>

#define N 100
#define m_n 40
#define ONE_PAGE BLOCKING_FACTOR

int numberOfSeries;
char nazwa[20] = "2.bin";
char textNazwa[20] = "";
bool tapePrev = false;
int numberOfSerie[3];
int dummySeries = 0;
int dummyOnTapeNr = 0;
int odczyty;
int zapisy;

void sprawdz(const char* name);
void fibSeries();
void poliMerge();
int checkForFibonacci(int a, int b);

int showMenu();
void generateRandomRecords(int numOfRecords);
void loadRecordsFile(const char* nazwa);
void readRecordsFromUser(int numOfRecords);

int main()
{
	odczyty = 0;
	zapisy = 0;
	while (showMenu() != 0);

	if (tapePrev) {
		sprawdz(nazwa);
	}

	fibSeries();
	if (tapePrev) {
		sprawdz("0.bin");
		sprawdz("1.bin");
		std::cout << "Liczba serii na tasmach: " << numberOfSerie[0] << " | " << numberOfSerie[1] << " | " << numberOfSerie[2] << std::endl;

	}
	std::cout << std::endl << "ETAP SCALANIA" << std::endl;
	poliMerge();
	std::cout << odczyty << std::endl;
	std::cout << zapisy;
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
			return 9;
		case 2:
			while (numOfRecords == 0) {
				std::cout << "Wprwadz liczbe rekordow do wygenerowania: ";
				std::cin >> option;
				numOfRecords = atoi(option);
			}
			generateRandomRecords(numOfRecords);
			//sprawdz("2.bin");
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
			if (tapePrev) std::cout << "Podglad wlaczony" << std::endl;
			else std::cout << "Podglad wylaczony" << std::endl;
			tapePrev = !tapePrev;
			return 1;
			break;
		default:
			return -1;
		}
}

void generateRandomRecords(int numOfRecords) {
	Polynomial* p1;
	DiskTape dts("2.bin", &odczyty, &zapisy);
	p1 = new Polynomial[numOfRecords];
	for (int i = 0; i < numOfRecords; i++) {
		p1[i] = Polynomial(rand() % 30, rand() % 30, rand() % 20, rand() % 10, rand() % 10, rand() % 10);
		dts.save(p1[i]);
	}
	dts.forceSave();
	dts.close();
}

void readRecordsFromUser(int numOfRecords) {
	Polynomial* p1;
	int a[5], x;
	DiskTape dts("2.bin", &odczyty, &zapisy);
	p1 = new Polynomial[numOfRecords];
	for (int i = 0; i < numOfRecords; i++) {
		std::cin >> a[0] >> a[1] >> a[2] >> a[3] >> a[4] >> x;
		p1[i] = Polynomial(a[0],a[1],a[2],a[3],a[4],x);
		dts.save(p1[i]);
	}

	dts.forceSave();
	dts.close();
	sprawdz("2.bin");
}

void sprawdz(const char* name) {
	int od=0,z=0;
	DiskTape o(name, &od, &z);
	int a = 1;
	int licznik = 1;
	std::cout << std::endl << "---------- PODGLAD "<<name<<" ----------";
	std::cout << " \n";
	Polynomial x;
	x = o.read(1, &a);
	while (a != 0) {
		std::cout <<licznik++<< ". "<< x;
		x = o.read(1, &a);
	}
	std::cout << std::endl << "---------- KONIEC " << name << " ----------";
	std::cout << std::endl;
}

void fibSeries() {
	int presentFibNumber = 1;
	int lastFibNumber = 0;
	Polynomial current;
	Polynomial compare;
	Polynomial lastSaved[3];
	int readed = 0;


	int currentTape_0 = 0;
	int currentTape_1 = 1;
	bool switched = false;
	bool cncat = false;
	int frstSerieIndex = 0;
	DiskTape mainTape(nazwa, &odczyty, &zapisy);
	DiskTape tapes[3];
	tapes[0].initDiskTape("0.bin", &odczyty, &zapisy);
	tapes[1].initDiskTape("1.bin", &odczyty, &zapisy);
	//tapes[2].initDiskTape("2.bin", &odczyty, &zapisy);
	std::queue<Polynomial> singlePage;
	current = mainTape.read(1, &readed);
	//page = (mainTape.read(ONE_PAGE, &readed));
	if (readed == 0) return;

	//for (int i = 0; i < readed; i++) {
	//	singlePage.push(page[i]);
	//	if (i > 0 && page[i - 1].value() > page[i].value() && frstSerieIndex == 0) {
	//		frstSerieIndex = i;
	//	}
	//}
	bool firstSwitch = false;
	int counter = presentFibNumber;
	compare = current;// page[0];
	int test = 0;
	lastSaved[currentTape_0] = current;// page[0];
	//lastSaved[currentTape_1] = page[frstSerieIndex];
	while (counter > 0) {
		//if (singlePage.empty()) {
		//	delete[] page;
		//page = mainTape.read(ONE_PAGE, &readed);
		//	for (int i = 0; i < readed; i++) {
		//		singlePage.push(page[i]);
		//	}
			if (readed == 0) {
				numberOfSerie[currentTape_0]++;
				if(firstSwitch) numberOfSerie[currentTape_1]++;
				counter--;
				break;
			}
		//}

		//-----------------------
		if (compare.value() <= current.value()){//singlePage.front().value()) {
			if (switched == true && lastSaved[currentTape_0].value() < current.value()){//singlePage.front().value()) {
				cncat = true;
			}
			/*else if (switched == true && lastSaved[currentTape_0].value() <= singlePage.front().value() && presentFibNumber>1) {
				counter++;
			}*/
			switched = false;
			tapes[currentTape_0].save(current);//singlePage.front());
			//std::cout << " /" << singlePage.front().value() << " /" << singlePage.front() ;;
			compare = current;//singlePage.front();
			lastSaved[currentTape_0] = current;//singlePage.front();
		}
		else if(counter>1){
			//std::cout <<std::endl << " /" << singlePage.front().value() << " /" << singlePage.front() ;
			if (lastSaved[currentTape_0].value() > current.value()){//singlePage.front().value()) {
				numberOfSerie[currentTape_0]++;
				counter--;
				if (switched) cncat = true;
			}
			/*else if (switched == true && lastSaved[currentTape_0].value() <= singlePage.front().value()) {
				counter++;
			}*/
			switched = false;
			lastSaved[currentTape_0] = current;//singlePage.front();
			tapes[currentTape_0].save(current);//singlePage.front());
			compare = current;// singlePage.front();
		}
		else {
			
			switched = true;
			tapes[currentTape_1].save(current);//singlePage.front());
			compare = current;//singlePage.front();
			//std::cout << std::endl <<"\t"<<currentTape_1<< std::endl << "/ " << singlePage.front().value() << " /" << singlePage.front() ;
			counter = presentFibNumber;
			presentFibNumber = presentFibNumber + lastFibNumber;
			lastFibNumber = counter;
			if (!firstSwitch)lastSaved[currentTape_1] = current;
			firstSwitch = true;
			if (lastSaved[currentTape_1].value() > current.value()){//singlePage.front().value()) {
				numberOfSerie[currentTape_1]++;
			}
			else if (switched == true && lastSaved[currentTape_1].value() <= current.value() && presentFibNumber>1) {
				counter++;
			}
			cncat = false;
			//else if(presentFibNumber>1) counter+=1;
			lastSaved[currentTape_1] = current;//singlePage.front();
			std::swap(currentTape_0, currentTape_1);

		}
		current = mainTape.read(1, &readed);
		//singlePage.pop();
		test++;
	}

	tapes[0].forceSave();
	tapes[1].forceSave();
	mainTape.removeContent();
	mainTape.close();
	dummySeries =  presentFibNumber - numberOfSerie[currentTape_0];
	numberOfSerie[currentTape_0] = presentFibNumber;//checkForFibonacci(numberOfSerie[currentTape_0], numberOfSerie[currentTape_1]);
	dummyOnTapeNr = currentTape_0;
	

	//numberOfSerie[currentTape_0] = presentFibNumber; 
	//numberOfSerie[currentTape_1] = lastFibNumber;
	//std::coutut << "koniec";

}




void poliMerge() {
	DiskTape tapes[3];
	tapes[0].initDiskTape("0.bin",&odczyty, &zapisy);
	tapes[1].initDiskTape("1.bin", &odczyty, &zapisy);
	tapes[2].initDiskTape("2.bin", &odczyty, &zapisy);
	int currentTape_0 = 0;
	int currentTape_1 = 1;
	int saveTape = 2;

	int readed[3] = { 0,0,0 };
	bool getNew[3] = { false,false,false };
	std::queue<Polynomial*> discPage[3];
	Polynomial currentRecord[3];
	Polynomial* backupPage;
	Polynomial compare[3];
	Polynomial lastResult;
	//page = (mainTape.read(ONE_PAGE, &readed));

	//for (int i = 0; i < readed[currentTape_1]; i++) {
	//	discPage[currentTape_1].push(&page[currentTape_1][i]);
	//}
	//for (int i = 0; i < readed[currentTape_0]; i++) {
	//	discPage[currentTape_0].push(&page[currentTape_0][i]);
	//}

	//compare[currentTape_1] = currentRecord[currentTape_1];

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
	currentTape_0 = !dummyOnTapeNr; //z tego musze przepisac dummynr serii
	currentTape_1 = !currentTape_0; //currentTape_1 = !currentTape_0;
	currentRecord[currentTape_0] = tapes[currentTape_0].read(1, &readed[currentTape_0]);
	currentRecord[2] = Polynomial();
	compare[currentTape_0] = currentRecord[currentTape_0];
	if (numberOfSerie[0] + numberOfSerie[1] + numberOfSerie[2] < 2) currentTape_0 = 0;

	while (numberOfSerie[0] + numberOfSerie[1] + numberOfSerie[2] > 1) {
		if (dummySeries > 0) {
			//currentTape_0 = !dummyOnTapeNr; //z tego musze przepisac dummynr serii
			//currentTape_1 = !currentTape_0;
			//page[currentTape_0] = tapes[currentTape_0].read(ONE_PAGE, &readed[currentTape_0]);
			if (readed[currentTape_0] == 0) return;
			compare[currentTape_0] = currentRecord[currentTape_0];

			while (dummySeries > 0) {

				//if (discPage[currentTape_0].empty()) {
				//	delete[] page[currentTape_0];
					
					if (readed[currentTape_0] == 0) {
						dummySeries--;
						numberOfSerie[currentTape_0]--;
						numberOfSerie[currentTape_1]--;
						numberOfSerie[saveTape]++;
						break;
					}
				//	for (int i = 0; i < readed[currentTape_0]; i++) {
				//		discPage[currentTape_0].push(&page[currentTape_0][i]);
				//	}
				//}

				//for (int i = 0; i < readed[currentTape_0]; i++) {
					if (compare[currentTape_0].value() <= currentRecord[currentTape_0].value()) {
						tapes[saveTape].save(currentRecord[currentTape_0]);//.
						compare[currentTape_0] = currentRecord[currentTape_0];

						//discPage[currentTape_0].pop();
					}
					else if (dummySeries > 1) {
						dummySeries--;
						numberOfSerie[currentTape_0]--;
						numberOfSerie[currentTape_1]--;
						numberOfSerie[saveTape]++;
						tapes[saveTape].save(currentRecord[currentTape_0]);//.

						compare[currentTape_0] = currentRecord[currentTape_0];
						//discPage[currentTape_0].pop();
					}
					else {
						numberOfSerie[currentTape_0]--;
						numberOfSerie[currentTape_1]--;
						numberOfSerie[saveTape]++;
						dummySeries--;
						break;
					}
					currentRecord[currentTape_0] = tapes[currentTape_0].read(1, &readed[currentTape_0]);
				//}
			}

		}
		//std::coutut << numberOfSerie[0] << " " << numberOfSerie[1] << " " << numberOfSerie[2];
#pragma endregion

		//lastResult = compare[currentTape_1];

		
		bool endOfSerie[3] = { false, false, false };
		bool newSerie[3] = { true, true, true };

		int fibNumber = numberOfSerie[currentTape_0] - numberOfSerie[currentTape_1];
		if (fibNumber < 0) {
			fibNumber = fibNumber*-1;
			std::swap(currentTape_0, currentTape_1);
		}

		currentRecord[currentTape_0] = tapes[currentTape_0].read(1, &readed[currentTape_0]);
		compare[currentTape_0] = currentRecord[currentTape_0];
		lastResult = currentRecord[currentTape_0];

		while (numberOfSerie[currentTape_1] > 0 || numberOfSerie[currentTape_0] > fibNumber) {


			if (endOfSerie[currentTape_0]) {
				//if (discPage[currentTape_1].empty()) endOfSerie[currentTape_0] = true;
				//else 
				if (currentRecord[currentTape_1].value() < compare[currentTape_0].value()) {
						endOfSerie[currentTape_1] = true;
					}else if (lastResult.value() > currentRecord[currentTape_1].value()) {
						//numberOfSerie[saveTape]++;
						endOfSerie[currentTape_1] = true;
						//std::coutut << std::endl << std::endl;;
					}
					else {
						tapes[saveTape].save(currentRecord[currentTape_1]);
						compare[currentTape_1] = currentRecord[currentTape_1];
						lastResult = compare[currentTape_1];
						//std::coutut << lastResult;
						//discPage[currentTape_1].pop();
						getNew[currentTape_1] = true;
						newSerie[currentTape_0] = false;
					}
			}
			else if (endOfSerie[currentTape_1]) {
				//if (discPage[currentTape_0].empty()) endOfSerie[currentTape_0] = true;
				//else 
				if (currentRecord[currentTape_0].value() < compare[currentTape_0].value() && !newSerie[currentTape_0]) {
					//numberOfSerie[currentTape_0]--;
					endOfSerie[currentTape_0] = true;
				} else if (lastResult.value() > currentRecord[currentTape_0].value()) {
				//	numberOfSerie[saveTape]++;
					endOfSerie[currentTape_0] = true;
					//std::coutut << std::endl << std::endl;
				}
				else {
					tapes[saveTape].save(currentRecord[currentTape_0]);
					compare[currentTape_0] = currentRecord[currentTape_0];
					lastResult = compare[currentTape_0];
					//std::coutut << lastResult;
					getNew[currentTape_0] = true;
					//discPage[currentTape_0].pop();
					newSerie[currentTape_0] = false;
				}
			}

			else if (lastResult.value() <= currentRecord[currentTape_0].value() || //czy ostatnia jest mniejsza od dowolnego
				lastResult.value() <= currentRecord[currentTape_1].value()) {
				if (currentRecord[currentTape_0].value() <= currentRecord[currentTape_1].value()) { // czy 1 jest mniejsze od 2
					if (compare[currentTape_0].value() > currentRecord[currentTape_0].value()
						&& !newSerie[currentTape_0]) {	//czy ostatnia 1 jest wieksza od obecnej 1 i czy nie sa to nowe serie
					//	numberOfSerie[currentTape_0]--;
						endOfSerie[currentTape_0] = true;
					}
					else {
						tapes[saveTape].save(currentRecord[currentTape_0]);
						compare[currentTape_0] = currentRecord[currentTape_0];
						lastResult = compare[currentTape_0];
						//discPage[currentTape_0].pop();
						//std::coutut << lastResult;
						getNew[currentTape_0] = true;
						newSerie[currentTape_0] = false;
					}
				}
				else {
					if (compare[currentTape_1].value() > currentRecord[currentTape_1].value()
						&& !newSerie[currentTape_1]) {
					//	numberOfSerie[currentTape_1]--;
						endOfSerie[currentTape_1] = true;
					}
					else {
						tapes[saveTape].save(currentRecord[currentTape_1]);
						compare[currentTape_1] = currentRecord[currentTape_1];
						lastResult = compare[currentTape_1];
						//discPage[currentTape_1].pop();
						getNew[currentTape_1] = true;
						//std::coutut << lastResult;
						newSerie[currentTape_1] = false;
					}

				}
			}
			else {
				//numberOfSerie[saveTape]++;
				if (currentRecord[currentTape_0].value() <= currentRecord[currentTape_1].value()) {
					if (compare[currentTape_0].value() > currentRecord[currentTape_0].value()
						&& !newSerie[currentTape_0]) {
					//	numberOfSerie[currentTape_0]--;
						endOfSerie[currentTape_0] = true;
					}
					else {
						tapes[saveTape].save(currentRecord[currentTape_0]);
						compare[currentTape_0] = currentRecord[currentTape_0];
						lastResult = compare[currentTape_0];
						//discPage[currentTape_0].pop();
						getNew[currentTape_0] = true;
						//std::coutut << lastResult;
						newSerie[currentTape_0] = false;
					}

				}
				else {
					if (compare[currentTape_1].value() > currentRecord[currentTape_1].value()
						&& !newSerie[currentTape_1]) {
					//	numberOfSerie[currentTape_1]--;
						endOfSerie[currentTape_1] = true;
					}
					else {
						tapes[saveTape].save(currentRecord[currentTape_1]);
						compare[currentTape_1] = currentRecord[currentTape_1];
						lastResult = compare[currentTape_1];
						//discPage[currentTape_1].pop();
						getNew[currentTape_1] = true;
						//std::coutut << lastResult;
						newSerie[currentTape_1] = false;
					}
				}
			}

			if (getNew[currentTape_0] && !endOfSerie[currentTape_0]) {
				//delete[] page[currentTape_0];
				currentRecord[currentTape_0] = tapes[currentTape_0].read(1, &readed[currentTape_0]);
				getNew[currentTape_0] = false;
				if (readed[currentTape_0] == 0) {
					//numberOfSerie[currentTape_0]--;
					endOfSerie[currentTape_0] = true;
				}
				//for (int i = 0; i < readed[currentTape_0]; i++) {
				//	discPage[currentTape_0].push(&page[currentTape_0][i]);
				//}
			}


			if (getNew[currentTape_1] && !endOfSerie[currentTape_1]) {
				//delete[] page[currentTape_1];
				currentRecord[currentTape_1] = tapes[currentTape_1].read(1, &readed[currentTape_1]);
				getNew[currentTape_1] = false;
				if (readed[currentTape_1] == 0) {
					//numberOfSerie[currentTape_1]--;
					endOfSerie[currentTape_1] = true;
				}
				//for (int i = 0; i < readed[currentTape_1]; i++) {
				//	discPage[currentTape_1].push(&page[currentTape_1][i]);
				//}
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
		//checkForFibonacci(5, 6);
		int fibDiffrence = checkForFibonacci(numberOfSerie[saveTape], numberOfSerie[currentTape_0]);

		if (fibDiffrence > 0) {
			dummySeries = fibDiffrence;
			numberOfSerie[saveTape] += dummySeries;
		}
		//std::coutut << numberOfSerie[0] << " " << numberOfSerie[1] << " " << numberOfSerie[2];
		//DiskTape tapes[3];

		tapes[currentTape_1].removeContent();
		tapes[saveTape].reopen();

		if (tapePrev) {
			char nazwa[6] = "0.bin";
			nazwa[0] += currentTape_0;
			sprawdz(nazwa);
			nazwa[0] = '0' + saveTape;
			sprawdz(nazwa);
			std::cout << "Liczba serii na tasmach: " << numberOfSerie[0] << " | " << numberOfSerie[1] << " | " << numberOfSerie[2] << std::endl;
		}

		std::swap(saveTape, currentTape_0);
		std::swap(saveTape, currentTape_1);
		endOfSerie[0] = false; endOfSerie[1] = false; endOfSerie[2] = false;
		newSerie[0] = true; newSerie[1] = true;	newSerie[2] = true;
	}
	char nazwa[6] = "0.bin";
	nazwa[0] = '0' + currentTape_0;
	if(tapePrev) sprawdz(nazwa);
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








//
//
//void createSeries() {
//	DiskTape dtr("x.bin", &odczyty, &zapisy);
//	int readed = 0;
//	double k = ((1.0*N) /ONE_PAGE);
//	numberOfSeries = ceil(k);
//	int bufforCount = numberOfSeries > m_n ? m_n : numberOfSeries;
//	DiskTape *series = new DiskTape[numberOfSeries > m_n ? m_n:numberOfSeries];
//
//	for (int i = 0; i < bufforCount; i++) {
//		nazwa[0] = i / 10000 + '0';
//		nazwa[1] = i / 1000 + '0';
//		nazwa[2] = i / 100 + '0';
//		nazwa[3] = i / 10 + '0';
//		nazwa[4] = i % 10 + '0';
//		series[i].initDiskTape(nazwa);
//	}
//
//	for (int i = 0; i < numberOfSeries; i++) {
//		Polynomial* x = dtr.read(ONE_PAGE, &readed);
//		//for (int i = 0; i < readed; i++) {
//		//	std::cout << x[i];
//		//}
//		//std::cout << std::endl << std::endl << std::endl << "-------------------------" << std::endl << std::endl << std::endl;
//		
//		qsort(x, readed, sizeof(Polynomial), compare);
//		
//		//for (int i = 0; i < readed; i++) {
//		//	std::cout << x[i];
//		//}
//		//std::cout << std::endl << std::endl << std::endl << "-------------------------" << std::endl << std::endl << std::endl;
//
//		series[i%bufforCount].save(x,readed);
//		series[i%bufforCount].forceSave();
//		delete[] x;
//	}
//	delete[] series;
//}
//
//int compare(const void * a, const void * b)
//{
//	return ((*(Polynomial*)a).value() - (*(Polynomial*)b).value());
//}
//
//
//
//
//
//
//void merge() {
//	//DiskTape dts("w.bin");
//	 // tyle rekordow wejdzie do buforow
//
//	int steps = 2;
//	bool finished = false;
//
//	while (steps >= 2) {
//		steps = ceil(1.0*numberOfSeries / m_n);
//		int stepsDone = 0;
//
//
//		int seriesLeft = numberOfSeries;
//		int doneSeries = 0;
//		int *readed = new int[numberOfSeries];
//		DiskTape *series = new DiskTape[numberOfSeries];
//		for (int i = 0; i < numberOfSeries; i++) {
//			nazwa[1] = i / 100 + '0';
//			nazwa[2] = i / 10 + '0';
//			nazwa[3] = i % 10 + '0';
//			series[i].initDiskTape(nazwa);
//		}
//
//		DiskTape* DTsave = new DiskTape[steps];
//		nazwa[0]++;
//		for (int i = 0; i < steps; i++) {
//			nazwa[1] = i / 100 + '0';
//			nazwa[2] = i / 10 + '0';
//			nazwa[3] = i % 10 + '0';
//			DTsave[i].initDiskTape(nazwa);
//		}
//
//
//
//		std::vector<SingleSerie> seriesBuffor;
//
//		while (seriesLeft > 0) {
//			//nad tymp petla
//			int seriesToMerge = (seriesLeft > m_n - 1) ? m_n - 1 : seriesLeft; //wszystko albo liczba bufforow pomniejszona o 1
//			seriesLeft = seriesLeft - seriesToMerge;
//			//2 zamiast bufforSize
//			int loadFromOneSerieQty = ceil(1.0*2 / seriesToMerge); // ile ladujemy z jednej serii
//			Polynomial* outBuffor = new Polynomial[loadFromOneSerieQty];
//			//Polynomial** seriesBuffor = new Polynomial*[seriesToMerge];					//(Polynomial**)malloc(sizeof(Polynomial*)*seriesToMerge);
//			int inOutBuffor = 0;
//			Polynomial *x;
//			for (int i = doneSeries; i < doneSeries + seriesToMerge; i++) {
//				x = series[i].read(loadFromOneSerieQty, &readed[i]);
//				SingleSerie ss;
//				ss.vct = std::vector<Polynomial>(x, x + sizeof(x) / sizeof(Polynomial) + readed[i]);
//				ss.number = i;
//
//				seriesBuffor.push_back(ss);
//
//				//seriesBuffor[i] = series[i].read(loadFromOneSerieQty, &readed[i]);		//new Polynomial[loadFromOneSerieQty];// (Polynomial*)malloc(sizeof(Polynomial)*loadFromOneSerieQty);
//				//for (int j = 0; j < loadFromOneSerieQty; j++) {
//				//	std::cout << seriesBuffor.at(i-doneSeries).vct.at(j);//seriesBuffor[i][j];
//				//}
//				//std::cout << std::endl << std::endl << std::endl;
//			}
//
//
//			auto cmp = [](SingleSerie a, SingleSerie b) {
//				if (a.vct.size() == 0) return false;
//				if (b.vct.size() == 0) return true;
//				return (a.vct.at(0).value() > b.vct.at(0).value()); };
//			std::priority_queue<SingleSerie, std::vector<SingleSerie>, decltype(cmp) > pq(cmp);
//
//			for (int i = 0; i < seriesBuffor.size(); i++) {
//				pq.push(seriesBuffor[i]);
//			}
//
//			while (!pq.empty()) {
//				for (inOutBuffor; inOutBuffor < loadFromOneSerieQty; inOutBuffor++) {
//					if (pq.empty()) {
//						break;
//					}
//
//					auto smallest = pq.top();
//
//					outBuffor[inOutBuffor] = smallest.vct.at(0);
//					smallest.vct.erase(smallest.vct.begin());
//
//					if (smallest.vct.size() == 0) {
//						x = series[smallest.number].read(loadFromOneSerieQty, &readed[smallest.number]);
//
//						if (readed[smallest.number] < 1) smallest.empty = true;
//						else {
//							for (int k = 0; k < readed[smallest.number]; k++) {
//								smallest.vct.push_back(x[k]);
//							}
//						}
//					}
//					pq.pop();
//					if (!smallest.empty) {
//						pq.push(smallest);
//					}
//				}
//				DTsave[stepsDone].save(outBuffor, inOutBuffor);
//				DTsave[stepsDone].forceSave();
//				inOutBuffor = 0;
//			}
//			inOutBuffor = 0;
//			doneSeries = doneSeries + seriesToMerge;
//			seriesBuffor.clear();
//			stepsDone++;
//			delete[] outBuffor;
//		}
//		numberOfSeries = stepsDone;
//		delete[] readed;
//		delete[] DTsave;
//		delete[] series;
//	}
//}