#include "stdafx.h"
#include <stdio.h>
#include "DiskTape.h"


DiskTape::DiskTape(const char* name, int* odczyty, int* zapisy)
	:name(name),initialized(true), readBufforPos(0)
{
	disk.open(name, std::fstream::in | std::fstream::out | std::fstream::binary);
	if (!disk.is_open()) {
		disk.open(name, std::fstream::in | std::fstream::out | std::fstream::trunc | std::fstream::binary);
	}
	this->odczyty = odczyty;
	this->zapisy = zapisy;
}
DiskTape::DiskTape() {
}

void DiskTape::initDiskTape(const char* name, int* odczyty, int* zapisy) {
	if (initialized) return;
	this->name = name;
	initialized = true;
	disk.open(name, std::fstream::in | std::fstream::out | std::fstream::binary);
	if (!disk.is_open()) {
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

void DiskTape::close() {

		if (disk.is_open()) {
			disk.close();
		}
		//std::remove(name);

}
void DiskTape::forceSave() {
	saveBuffored();
}

void DiskTape::saveBuffored() {
	for (int i = 0; i < saveBufforQty; i++) {
		//disk << saveBuffor[i];
		int x = sizeof(Polynomial);
		if (sizeof(saveBuffor[i]) != x) {
			std::cout << "xd";
		}
		disk.write((char*)&(saveBuffor[i].getCoefficients()[0]), sizeof(int));
		disk.write((char*)&(saveBuffor[i].getCoefficients()[1]), sizeof(int));
		disk.write((char*)&(saveBuffor[i].getCoefficients()[2]), sizeof(int));
		disk.write((char*)&(saveBuffor[i].getCoefficients()[3]), sizeof(int));
		disk.write((char*)&(saveBuffor[i].getCoefficients()[4]), sizeof(int));
		disk.write((char*)&(saveBuffor[i].x), sizeof(int));
	//	std::cout << "\t\t\t" << saveBuffor[i];
	}
	(*zapisy) += 1;
	saveBufforQty = 0;
}

void DiskTape::save(Polynomial p)
{	
	saveBuffor[saveBufforQty++] = p;
	if (saveBufforQty >= BLOCKING_FACTOR) saveBuffored();
}


bool DiskTape::readBuffered() {
	(*odczyty) += 1;
	int a[5];
	int x;
	for (readBufforQty = 0; readBufforQty < BLOCKING_FACTOR; readBufforQty++) {
		
		if (disk.eof()) {
			//readBufforQty++;
			readBufforQty--;
			readBufforPos = 0;
			return false;
		}
		else {
			//disk >> readBuffor[readBufforQty];
			disk.read((char*)&a[0], sizeof(int));
			disk.read((char*)&a[1], sizeof(int));
			disk.read((char*)&a[2], sizeof(int));
			disk.read((char*)&a[3], sizeof(int));
			disk.read((char*)&a[4], sizeof(int));
			disk.read((char*)&x, sizeof(int));
			readBuffor[readBufforQty].set(a[0], a[1], a[2], a[3], a[4], x);
		}
	}
	if (disk.eof())readBufforQty--;
	readBufforPos = 0;
	return true;
}

Polynomial DiskTape::read(int numberOfRecords, int* readed)
{
	Polynomial p;
	int pos = 0;
	//wczytaj strone
	//wgraj do pamieci
	//powtorz
	if (readBufforQty > 0) {
		p = readBuffor[readBufforPos++];
		readBufforQty--;
		*readed = 1;
		return p;
	}
	bool fullRead = readBuffered();
	if (readBufforQty < 1) {
		*readed = 0;
		return p;
	}
	p = readBuffor[readBufforPos++];
	readBufforQty--;
	*readed = 1;
	return p;
}

