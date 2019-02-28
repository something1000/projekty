
#include <iostream>
#include "Polynomial.h"
#include "Disk.h"
#include <random>
#include <set>
void readCommandsFromFile(Disk* d);
void test(Disk* d);
int main()
{
	Disk d;

	std::cout << "D k - dodaj rekord o kluczu k" << std::endl;
	std::cout << "C k - odczytaj rekord o kluczu k" << std::endl;
	std::cout << "N - pobierz nastepny rekord" << std::endl;
	std::cout << "A k - aktualizuj rekord o kluczu k" << std::endl;
	std::cout << "U k - usun rekord o kluczu k" << std::endl;
	std::cout << "S - wyswietl caly plik" << std::endl;
	std::cout << "L - wlacz logi dla operacji" << std::endl;
	std::cout << "R - reorganizuj plik" << std::endl;
	std::cout << "K - wczytaj komendy z pliku" << std::endl;
	std::cout << "X - wyjscie z programu" << std::endl;
	Polynomial p;
	char cmd = 0;
	int param = 0;

	int suma_r = 0;
	int suma_s = 0;

	while (true) {
		std::cout << ">: ";
		//scanf("%c",&cmd);
		std::cin >> cmd;
		int a = 0;
		switch (cmd) {
			case 'T':
				std::cin >> param;
				for (int i = 0; i < param; i++) {
					a = std::rand() << 16 | std::rand();

					p = Polynomial(a + 1, std::rand() % 100 + 1,
						std::rand() % 100 + 1,
						std::rand() % 100 + 1,
						std::rand() % 100 + 1,
						std::rand() % 100 + 1,
						std::rand() % 100 + 1);
					if (d.save(p)) {
						std::cout << "Dodano rekord "
							<< p.getKey() << " " << p.getX() << " " << p.getCoefficient()[0] << " " << p.getCoefficient()[1] << " "
							<< p.getCoefficient()[2] << p.getCoefficient()[3] << " " << p.getCoefficient()[4] << std::endl;
					}
					else {
						std::cout << "Rekord istnieje" << std::endl;
					}
				}
				break;
			case 'D':
			case 'd':
				std::cin >> param;
				//scanf("%d", &param);
				
				p = Polynomial(param, std::rand() % 100 + 1,
							std::rand() % 100 + 1,
							std::rand() % 100 + 1,
							std::rand() % 100 + 1,
							std::rand() % 100 + 1,
							std::rand() % 100 + 1);
				if (d.save(p)) {
					std::cout << "Dodano rekord "
						<< p.getKey() << " " << p.getX() << " " << p.getCoefficient()[0] << " " << p.getCoefficient()[1] << " "
						<< p.getCoefficient()[2] << p.getCoefficient()[3] << " " << p.getCoefficient()[4] << std::endl;
				}
				else {
					std::cout << "Rekord istnieje" << std::endl;
				}
			
				break;
			case 'C':
			case 'c':
				std::cin >> param;
				//scanf("%d", &param);
				p = d.read(param);
				if(p.getKey() != 0)	
					std::cout << "Odczytano rekord "
							  << p.getKey() << " " << p.getX() << " " << p.getCoefficient()[0] << " " << p.getCoefficient()[1] << " "
							  << p.getCoefficient()[2] << p.getCoefficient()[3] << " " << p.getCoefficient()[4] << std::endl;
				else std::cout << "Nie znaleziono rekordu" << std::endl;
				break;
			case 'N':
			case 'n':
				p = d.readNext();
				if (p.getKey() != -1)
					std::cout << "Odczytano rekord "
					<< p.getKey()<< " " << p.getX() << " " << p.getCoefficient()[0] << " " << p.getCoefficient()[1] << " "
					<< p.getCoefficient()[2] << p.getCoefficient()[3] << " " << p.getCoefficient()[4] << std::endl;
				else {
					std::cout << "Brak nastepnych rekordow" << std::endl;
					d.resetNext();
				}
				break;
			case 'A':
			case 'a':
				std::cin >> param;
				//scanf("%d", &param);
				p = Polynomial(param, std::rand() % 100 + 1,
					std::rand() % 100 + 1,
					std::rand() % 100 + 1,
					std::rand() % 100 + 1,
					std::rand() % 100 + 1,
					std::rand() % 100 + 1);
				if (d.update(p)) {
					std::cout << "Aktualizuje rekord "
						<< param << " " << p.getX() << " " << p.getCoefficient()[0] << " " << p.getCoefficient()[1] << " "
						<< p.getCoefficient()[2] << " " << p.getCoefficient()[3] << " " << p.getCoefficient()[4] << std::endl;
				}
				else {
					std::cout << "Rekord nie istnieje" << std::endl;
				}
				break;
			case 'U':
			case 'u':
				//std::cin >> param;
				scanf("%d", &param);
				if (d.DeleteRecord(param)) {
					std::cout << "Usunięto rekord o kluczu " << param << std::endl;
				}
				else {
					std::cout << "Nie znaleziono rekordu o kluczu " << param << std::endl;
				}
				break;
			case 'L':
			case 'l':
				if (d.setLog()) {
					std::cout << "Wlaczono logi" << std::endl;
				}
				else {
					std::cout << "Wylaczono logi" << std::endl;
				}
				break;
			case 'S':
			case 's':
				d.showFile();
				break;
			case 'F':
			case 'f':
				d.readAllRecords();
				break;
			case 'R':
			case 'r':
				d.reorganize();
				std::cout << "Zakończono reorganizacje"<<std::endl;
				break;
			case 'K':
			case 'k':
				readCommandsFromFile(&d);
				break;
			case 'X':
			case 'x':
				return 0;
		}
		while (std::cin.get() != '\n') {};
	}

	//for (int i = 1; i < 1000; i++) {
	//	int key = std::rand() % 1510 + 199;
	//	std::cout << key << "\n";
	//	Polynomial p(key,
	//				std::rand() % 100 + 1,
	//				std::rand() % 100 + 1,
	//				std::rand() % 100 + 1,
	//				std::rand() % 100 + 1,
	//				std::rand() % 100 + 1,
	//				std::rand() % 100 + 1);
	//	d.save(p);
	//}
	////d.DeleteRecord(23);
	////d.DeleteRecord(5);
	//std::cout << "_______________________" << std::endl;
	//std::cout << "_______________________" << std::endl;
	//std::cout << "_______________________" << std::endl;
	//
	//Polynomial p;
	//p.setKey(1);
	//while(p.getKey() != -1){
	//	p = d.readNext();
	//	std::cout << p.getKey() << std::endl;
	//}
	//std::cout << "Kluczy gł: " << d.getNumOfKeys() << std::endl;
	//std::cout << "Kluczy ov: " << d.getOvNumOfKeys() << std::endl;

	return 0;
}


void readCommandsFromFile(Disk* d){
	Polynomial p;
	char buffer[100];
	std::cout << "Wprowadz nazwe pliku: ";
	std::cin >> buffer;

	std::fstream file;
	file.open(buffer, std::ios::in);
	if (!file.is_open()) {
		std::cout << "Nie znaleziono pliku" << std::endl;
		return;
	}
	int param;
	char cmd;
	while (!file.eof()) {
		file >> cmd;
		switch (cmd) {
		case 'D':
		case 'd':
			file >> param;
			//scanf("%d", &param);
			p = Polynomial(param, std::rand() % 100 + 1,
				std::rand() % 100 + 1,
				std::rand() % 100 + 1,
				std::rand() % 100 + 1,
				std::rand() % 100 + 1,
				std::rand() % 100 + 1);
			if (d->save(p)) {
				std::cout << "Dodano rekord "
					<< param << " " << p.getX() << " " << p.getCoefficient()[0] << " " << p.getCoefficient()[1] << " "
					<< p.getCoefficient()[2] << p.getCoefficient()[3] << " " << p.getCoefficient()[4] << std::endl;
			}
			else {
				std::cout << "Rekord istnieje" << param << " " << p.getX() << " " << p.getCoefficient()[0] << " " << p.getCoefficient()[1] << " "
					<< p.getCoefficient()[2] << p.getCoefficient()[3] << " " << p.getCoefficient()[4] << std::endl;;
			}
			break;
		case 'C':
		case 'c':
			file >> param;
			//scanf("%d", &param);
			p = d->read(param);
			if (p.getKey() != 0)
				std::cout << "Odczytano rekord "
				<< p.getKey() << " " << p.getX() << " " << p.getCoefficient()[0] << " " << p.getCoefficient()[1] << " "
				<< p.getCoefficient()[2] << p.getCoefficient()[3] << " " << p.getCoefficient()[4] << std::endl;
			else std::cout << "Nie znaleziono rekordu" << std::endl;
			break;
		case 'N':
		case 'n':
			p = d->readNext();
			if (p.getKey() != -1)
				std::cout << "Odczytano rekord "
				<< p.getKey() << " " << p.getX() << " " << p.getCoefficient()[0] << " " << p.getCoefficient()[1] << " "
				<< p.getCoefficient()[2] << p.getCoefficient()[3] << " " << p.getCoefficient()[4] << std::endl;
			else {
				std::cout << "Brak nastepnych rekordow" << std::endl;
			}
			break;
		case 'A':
		case 'a':
			file >> param;
			//scanf("%d", &param);
			p = Polynomial(param, std::rand() % 100 + 1,
				std::rand() % 100 + 1,
				std::rand() % 100 + 1,
				std::rand() % 100 + 1,
				std::rand() % 100 + 1,
				std::rand() % 100 + 1);
			if (d->update(p)) {
				std::cout << "Aktualizuje rekord "
					<< param << " " << p.getX() << " " << p.getCoefficient()[0] << " " << p.getCoefficient()[1] << " "
					<< p.getCoefficient()[2] << p.getCoefficient()[3] << " " << p.getCoefficient()[4] << std::endl;
			}
			else {
				std::cout << "Rekord nie istnieje" << std::endl;
			}
			break;
		case 'U':
		case 'u':
			//std::cin >> param;
			file >> param;
			if (d->DeleteRecord(param)) {
				std::cout << "Usunięto rekord o kluczu " << param << std::endl;
			}
			else {
				std::cout << "Nie znaleziono rekordu o kluczu " << param << std::endl;
			}
			break;
		case 'S':
		case 's':
			d->showFile();
			break;
		case 'R':
		case 'r':
			d->reorganize();
			std::cout << "Zakończono reorganizacje" << std::endl;
			break;
		}
	}
}




void test(Disk* d) {
	//char com[4] = { 'D','U','A', 'C' };
	Polynomial p;
	std::fstream x;
	x.open("tst.txt", std::ios::trunc | std::ios::out);	
	for (int i = 0; i < 1000; i++) {
		int a = std::rand() + 1;
		int b = std::rand() + 1;
		a = (a << 16 | b);

		//std::copy(klucze.begin(), klucze.end(), k.begin());
		//int key = k.at(pos);
		int com = std::rand() % 4;
		p = Polynomial((a << 16 | b),
			std::rand() % 100 + 1,
			std::rand() % 100 + 1,
			std::rand() % 100 + 1,
			std::rand() % 100 + 1,
			std::rand() % 100 + 1,
			std::rand() % 100 + 1);

		switch (com) {
		case 0:
			//d->save(p);
			x << "D " << a << "\n";
			break;
		case 1:
			x << "C " << a << "\n";
			break;
		case 2:
			x << "U " << a << "\n";
			break;
		case 3:
			x << "A " << a << "\n";
			break;
		}
	}

	x.close();
}