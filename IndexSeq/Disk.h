#pragma once
#include <iostream>
#include <fstream>
#include "Polynomial.h"
 
#define BLOCKING_FACTOR 10
#define RECORD_SIZE 33
#define PAGE_BYTES (RECORD_SIZE * BLOCKING_FACTOR) //32*5
#define BLOCIKNG_INDEX PAGE_BYTES/10 // 330/8bajtow (4klucz 4 adreS)
#define OVERFLOW_RATIO 0.2;

typedef struct {
	int key = 0;
	int address = 1;
} Index;

class Disk
{
public:
	Disk();
	bool DeleteRecord(int key);
	~Disk();
	bool save(Polynomial p);
	Polynomial read(int key);
	Polynomial readNext();
	int getNumOfKeys();
	int getOvNumOfKeys();
	bool is_ok = true;
private:
	int currentPage = -1;
	int overflowPageInMemory =-1;
	int overflowPages = 1;
	struct{
		int position = -1;
		int page = 1;
		Polynomial record;
	} lastRecord;
		void resetLastRecord();

		void savePage(std::fstream *stream, int address, Polynomial * buffor);

	std::fstream confFile;
		bool loadSettings();

		bool loadIndexes();

		void saveIndexes();

		void addGuard();

	std::fstream mainArea;
	int mainKeys = 0;
	int mainAreaPages = 1;
		void savePage(int address);

	std::fstream indexFile;

	std::fstream overflowArea;
		void saveOverflowPage(int address);

	Polynomial pageBuffor[BLOCKING_FACTOR];// = Polynomial[BLOCKING_FACTOR];

	Index* index;// = new Index[BLOCIKNG_INDEX];
	int indexPages = 1;
	int indexKeys = 1;
		void saveIndex();
		void readIndex();
		void putKey(int key);
		int getKeyPosition(int key);

	int readPage(int address);

	int readOverflowPage(int address);

	int overflowKeys = 0;
	int overflowLastAddress = 0;
		bool addToOverflow(Polynomial p, int mainAreaAddress);


		void reorganize();

	const char zeros[500] = { 0 };
	const char deleted = 1;
};

