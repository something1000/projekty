#pragma once
#include "Polynomial.h"
#include <fstream>
#include <iostream>


#define BLOCKING_FACTOR 64
#define PAGE_SIZE 1536//in bytes

class DiskTape
{
public:
	DiskTape(const char* nazwa, int* odczyty, int* zapisy);
	DiskTape();
	~DiskTape();
	void save(Polynomial p);
	Polynomial read(int numberOfRecords, int* readed);
	void forceSave();
	void initDiskTape(const char* nazwa, int* odczyty, int* zapisy);
	void removeContent();
	void reopen();
	void close();
private:
	std::fstream disk;
	int bfr;
	Polynomial* saveBuffor = new Polynomial[BLOCKING_FACTOR];
	void saveBuffored();
	int saveBufforQty = 0;

	bool readBuffered();
	int readBufforQty = 0;
	int readBufforPos = 0;
	Polynomial* readBuffor = new Polynomial[BLOCKING_FACTOR];
	const char* name;
	bool initialized = false;
	int* odczyty;
	int* zapisy;
};

