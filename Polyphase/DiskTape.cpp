#include "stdafx.h"
#include <stdio.h>
#include "DiskTape.h"


DiskTape::DiskTape(const char* name, int* odczyty, int* zapisy, bool clear)
	:name(name), initialized(true), readBufforPos(0)
{
	if (!clear) {
	disk.open(name, std::fstream::in | std::fstream::out | std::fstream::binary);
	}

	if (!disk.is_open() || clear) {
		disk.open(name, std::fstream::in | std::fstream::out | std::fstream::trunc | std::fstream::binary);
	}
	this->odczyty = odczyty;
	this->zapisy = zapisy;
}
DiskTape::DiskTape() {
}

void DiskTape::initDiskTape(const char* name, int* odczyty, int* zapisy, bool clear) {
	if (initialized) return;
	this->name = name;
	initialized = true;

	if (!clear) {
		disk.open(name, std::fstream::in | std::fstream::out | std::fstream::binary);
	}
	if (!disk.is_open() || clear) {
		disk.open(name, std::fstream::in | std::fstream::out | std::fstream::trunc | std::fstream::binary);
	}

	readBufforPos = 0;
	this->odczyty = odczyty;
	this->zapisy = zapisy;
}

void DiskTape::removeContent()
{
	disk.close();
	std::remove(name);
	disk.open(name, std::fstream::in | std::fstream::out | std::fstream::trunc | std::fstream::binary);
	initialized = true;
}
void DiskTape::reopen()
{
	if (this->saveBufforQty > 0) forceSave();
	disk.close();
	disk.open(name, std::fstream::in | std::fstream::out | std::fstream::binary);
	readBufforPos = 0;
	readBufforQty = 0;
	saveBufforQty = 0;
}

DiskTape::~DiskTape()
{
	if (this->saveBufforQty > 0) forceSave();
;	if (disk.is_open()) {
		//forceSave();
		disk.close();
	}
	//std::remove(name);
}


void DiskTape::showSeries() {
	int startPos = disk.tellg();
	int flags = disk.flags();

	bool endFile = false;
	if (disk.eof()) {
		endFile = true;
	}
	int backup_saved = *zapisy;
	int backup_readed = *odczyty;
	bool readed = true;

	Polynomial *burBuffor = new Polynomial[BLOCKING_FACTOR];
	memcpy(burBuffor, readBuffor, sizeof(Polynomial)*BLOCKING_FACTOR);
	//for (int i = 0; i < BLOCKING_FACTOR; i++) {
	//	burBuffor[i] = readBuffor[i];
	//}

	int burBufforQty = readBufforQty;
	int burBufforPos = readBufforPos;
	if (readBufforPos > 0) {
		readBufforPos--;
		readBufforQty++;
	}
	Polynomial t;
	t = read(&readed);
	std::cout << std::endl << "\n---------- POCZATEK " << name << " ----------\n";
	while (readed) {
		std::cout << t;
		t = read(&readed);
	}
	std::cout << std::endl << "---------- KONIEC " << name << " ----------\n";
	disk.close();
	disk.open(name, std::fstream::in | std::fstream::out | std::fstream::binary);
	if (endFile) {
		//disk.seekg(0);
		disk.seekg(0, disk.end);
		std::cout << disk.eof();
		disk.setstate(std::ios_base::eofbit);
		std::cout << disk.eof();
	}
	else {
		//disk.clear();
		//disk.seekg(0, std::ios::beg);
		disk.seekg(startPos);
		//disk.flags(flags);
	}
	readBufforPos = burBufforPos;
	readBufforQty = burBufforQty;
	memcpy(readBuffor, burBuffor, sizeof(Polynomial)*BLOCKING_FACTOR);
	//for (int i = 0; i < BLOCKING_FACTOR; i++) {
	//	readBuffor[i] = burBuffor[i];
	//}

	delete[] burBuffor;
	*zapisy = backup_saved;
	*odczyty = backup_readed;
}

void DiskTape::close() {
	if (saveBufforQty > 0)saveBuffored();
		if (disk.is_open()) {
			disk.close();
		}
		//std::remove(name);

}



void DiskTape::forceSave() {
	saveBuffored();
}

void DiskTape::saveBuffored() {
	if(saveBufforQty!=0) 	(*zapisy) += 1;
	for (int i = 0; i < saveBufforQty; i++) {
		//disk << saveBuffor[i];
		int x = sizeof(Polynomial);
		if (sizeof(saveBuffor[i]) != x) {
			std::cout << "xd";
		}
		disk.write((char*)&(saveBuffor[i].getCoefficients()[0]), 4);
		disk.write((char*)&(saveBuffor[i].getCoefficients()[1]), 4);
		disk.write((char*)&(saveBuffor[i].getCoefficients()[2]), 4);
		disk.write((char*)&(saveBuffor[i].getCoefficients()[3]), 4);
		disk.write((char*)&(saveBuffor[i].getCoefficients()[4]), 4);
		disk.write((char*)&(saveBuffor[i].x), sizeof(int));
	//	std::cout << "\t\t\t" << saveBuffor[i];
	}

	saveBufforQty = 0;
}

void DiskTape::save(Polynomial p)
{	
	saveBuffor[saveBufforQty++] = p;
	if (saveBufforQty >= BLOCKING_FACTOR) saveBuffored();
}


bool DiskTape::readBuffered() {

	int a[5];
	int x;
	for (readBufforQty = 0; readBufforQty < BLOCKING_FACTOR; readBufforQty++) {
		
		if (disk.eof()) {
			//readBufforQty++;
			if (readBufforQty != 0) {
				(*odczyty) += 1;
			}
			readBufforQty--;
			readBufforPos = 0;
			return false;
		}
		else {
			//disk >> readBuffor[readBufforQty];
			disk.read((char*)&a[0], 4);
			disk.read((char*)&a[1], 4);
			disk.read((char*)&a[2], 4);
			disk.read((char*)&a[3], 4);
			disk.read((char*)&a[4], 4);
			disk.read((char*)&x, 4);
			readBuffor[readBufforQty].set(a[0], a[1], a[2], a[3], a[4], x);
		}
	}
	
	if (disk.eof()) {
		readBufforQty--;
	}
	if(readBufforQty != 0) (*odczyty) += 1;
	readBufforPos = 0;
	return true;
}

Polynomial DiskTape::read(bool* readed)
{
	Polynomial p = readBuffor[readBufforPos];
	int pos = 0;
	//wczytaj strone
	//wgraj do pamieci
	//powtorz
	if (readBufforQty > 0) {
		p = readBuffor[readBufforPos++];
		readBufforQty--;
		*readed = true;
		return p;
	}
	bool fullRead = readBuffered();
	if (readBufforQty < 1) {
		*readed = false;
		return p;
	}
	p = readBuffor[readBufforPos++];
	readBufforQty--;
	*readed = true;
	return p;
}

