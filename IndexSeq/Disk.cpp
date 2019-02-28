#include "Disk.h"



Disk::Disk()
{

	mainArea.open("main.brt", std::ios::binary | std::ios::in | std::ios::out);
	if (!mainArea.is_open()) {
		mainArea.open("main.brt", std::ios::binary | std::ios::in | std::ios::out | std::ios::trunc);
		addGuard();
		overflowArea.open("overflow.brt", std::ios::binary | std::ios::in | std::ios::out | std::ios::trunc);
		index = new Index[BLOCIKNG_INDEX];
	//	for(int i=0;i)
		//mainArea.write(zeros, 10 * BLOCKING_FACTOR * 28);
	}
	else {
		is_ok = loadSettings();
	}
}

bool Disk::loadSettings() {
	std::cout << " laduje ustawienia"<<std::endl;
	overflowArea.open("overflow.brt", std::ios::binary | std::ios::in | std::ios::out);
	if (!overflowArea.is_open()) {
		std::cout << "Nie moge znalezc overflow! Wychodze";
		return false;
	}

	std::fstream conf;
	conf.open("conf.brt", std::ios::binary | std::ios::in | std::ios::out);
	conf.read((char*)&mainKeys, 4);//-1
	conf.read((char*)&mainAreaPages, 4);
	conf.read((char*)&overflowKeys, 4);
	conf.read((char*)&overflowLastAddress, 4);
	conf.read((char*)&indexPages, 4);
	conf.read((char*)&indexKeys, 4);
	conf.close();
	return loadIndexes();
}

bool Disk::loadIndexes() {
	indexFile.open("index.brt", std::ios::binary | std::ios::in | std::ios::out);

	if (!indexFile.is_open()) {
		std::cout << "Nie moge znalezc indexow! Wychodze";
		return false;
	}
	int trash = 0, k=0;
	index = new Index[indexKeys];
	for (int i = 0; i < indexPages; i++) {
		for (int j = 0; j < BLOCIKNG_INDEX; j++) {
			indexFile.read((char*)&index[k].address, 4);
			indexFile.read((char*)&index[k++].key, 4);
			indexFile.read((char*)&trash, 2);
		}
	}
	indexFile.close();
	return true;
}

void Disk::saveIndexes() {
	indexFile.open("index.brt", std::ios::binary | std::ios::in | std::ios::out | std::ios::trunc);

	int trash = 0, k = 0;
	for (int i = 0; i < indexKeys; i++) {
			indexFile.write((char*)&index[i].address, 4);
			indexFile.write((char*)&index[i].key, 4);
			indexFile.write((char*)&trash, 2);
	}
	for (int i = 0; i < BLOCIKNG_INDEX - indexKeys%BLOCIKNG_INDEX; i++) {
		indexFile.write((char*)&zeros, 4);
		indexFile.write((char*)&zeros, 4);
		indexFile.write((char*)&zeros, 2);
	}
	std::flush(indexFile);
	indexFile.close();
}

void Disk::addGuard() {
	int minus = -1;
	mainArea.write((char*)&minus, 4);//-1
	mainArea.write((char*)&zeros, 4);
	mainArea.write((char*)&zeros, 4);
	mainArea.write((char*)&zeros, 4);
	mainArea.write((char*)&zeros, 4);
	mainArea.write((char*)&zeros, 4);
	mainArea.write((char*)&zeros, 4);
	mainArea.write((char*)&zeros, 4);
	mainArea.write((char*)&zeros, 1);
	std::flush(mainArea);
}

int Disk::getKeyPosition(int key)
{
	int left = 0;
	int right = indexKeys - 1;
	int middle;
	int midKey;// = index[middle].key;
	

	if (indexKeys < 2) return index[0].address;

	while (left <= right) {
		middle = (left + right) / 2;
		if (index[middle].key == key) return index[middle].address;
		if (index[middle].key > key) right = middle - 1;
		else left = middle + 1;
	}
	if(index[middle].key > key && middle > 0) 
		return index[middle - 1].address;

	return index[middle].address;
}

bool Disk::save(Polynomial p)
{
	int address = this->getKeyPosition(p.getKey());
	int recordsOnPage = this->readPage(address);
	//mainArea.seekg(writePosition);

	int x = p.getX();
	int key = p.getKey();
	int *a = p.getCoefficient();


	if (recordsOnPage < BLOCKING_FACTOR) {
		int i;
		for (i = 0; i <recordsOnPage; i++) {
			if (pageBuffor[i].getKey() == key) return false;
		}

		for (i = recordsOnPage - 1; i >= 0; i--) {
			if (pageBuffor[i].getKey() > key) pageBuffor[i + 1] = pageBuffor[i];
			else break;//pageBuffor[i+1] = p;
		}
		pageBuffor[i + 1] = p;
		if (i + 1 == 0 || (i+1 == 1 && address ==1)) index[address-1].key = pageBuffor[i+1].getKey();
		savePage(address);
		mainKeys++;
		return true;
	}
	else {
		return addToOverflow(p,address);
	}
	//writePosition = mainArea.tellg();
}

bool Disk::addToOverflow(Polynomial p, int mainAreaAddress) {
	int i;
	for (i =0; i < BLOCKING_FACTOR; i++) {
		if (pageBuffor[i].getKey() >= p.getKey()) break;
	}
	if (pageBuffor[i].getKey() == p.getKey()) return false;
	i--; //chcê mniejszy od mojego klucza

	if (pageBuffor[i].overflowAddr() == 0) {
		pageBuffor[i].setOverflowAddr(++overflowLastAddress);
		savePage(mainAreaAddress);
		int r = readOverflowPage(overflowLastAddress);
		// zmienil sie pageBuffor
		pageBuffor[r] = p;
		saveOverflowPage(overflowLastAddress);
	}
	else {
		// w glownym nic nie zmieniam, aktualizuje ³ancuch 
		Polynomial next = pageBuffor[i];

		int currentAddress = next.overflowAddr();
		readOverflowPage(currentAddress);
		next = pageBuffor[(currentAddress - 1) % BLOCKING_FACTOR];

		if (next.getKey() > p.getKey()) {
			p.setOverflowAddr(currentAddress); //zmiana wskaznika na wiekszy rekord

			int r = readOverflowPage(++overflowLastAddress); // odczyt ostatniej strony
			pageBuffor[r] = p;
			saveOverflowPage(overflowLastAddress);		      // wstawienie rekordu i zapisanie

			readPage(mainAreaAddress);	// odczyt poprzedniego rekordu ze strony glownej
			pageBuffor[i].setOverflowAddr(overflowLastAddress);
			savePage(mainAreaAddress);  //  i zmiana wskaznika na ostatni rekord w pliku
		}
		else {

			int prevAddress = 0;
			while (true) {

				if (next.getKey() == p.getKey()) return false;

				if (next.overflowAddr() == 0) {
					pageBuffor[(currentAddress - 1) % BLOCKING_FACTOR].setOverflowAddr(++overflowLastAddress);
					saveOverflowPage(currentAddress);

					int r = readOverflowPage(overflowLastAddress);
					pageBuffor[r] = p;
					saveOverflowPage(overflowLastAddress);
					break;
				}

				prevAddress = currentAddress;
				currentAddress = next.overflowAddr();
				readOverflowPage(currentAddress);
				next = pageBuffor[(currentAddress - 1) % BLOCKING_FACTOR];

				if (next.getKey() > p.getKey()) {
					p.setOverflowAddr(currentAddress);

					readOverflowPage(prevAddress);
					pageBuffor[(prevAddress-1)%BLOCKING_FACTOR].setOverflowAddr(++overflowLastAddress);
					saveOverflowPage(prevAddress);

					int r = readOverflowPage(overflowLastAddress);
					pageBuffor[r] = p;
					saveOverflowPage(overflowLastAddress);
					break;
				}
			}
		}
	}

	overflowKeys++;
	if ((overflowLastAddress) / BLOCKING_FACTOR > overflowPages - 1) reorganize();
	return true;
	//pageBuffor[i].setOverflowAddr = overflowLastAddress++;
}

Polynomial Disk::read(int key)
{
	int address = this->getKeyPosition(key);
	int recordsOnPage = this->readPage(address);
	//mainArea.seekg(writePosition);

	if (key < 1) return Polynomial();

	for (int i = 0; i < recordsOnPage; i++) {
		if (pageBuffor[i].getKey() == key && pageBuffor[i].deleted != deleted) return pageBuffor[i];
		else if (pageBuffor[i].getKey() > key && i>0) {
			Polynomial next = pageBuffor[i-1];
			while (next.overflowAddr() != 0) {
				readOverflowPage(next.overflowAddr());
				next = pageBuffor[(next.overflowAddr() - 1) % BLOCKING_FACTOR];
				if (next.getKey() == key && !pageBuffor[i].deleted) return next;
			}
		}
	}
	return Polynomial();
}

Polynomial Disk::readNext()
{

	if (lastRecord.page > mainAreaPages) {
		lastRecord.record = Polynomial();
		lastRecord.record.setKey(-1);
		return lastRecord.record;
	}
	if (lastRecord.position == -1 && lastRecord.page == 1) {
		readPage(1);
		if (pageBuffor[0].overflowAddr() != 0) {
			int index = (pageBuffor[0].overflowAddr() - 1) % BLOCKING_FACTOR;
			readOverflowPage(pageBuffor[0].overflowAddr());
			lastRecord.record = pageBuffor[index];
			lastRecord.position = 0;
		}
		else {
			lastRecord.record = pageBuffor[1];
			lastRecord.position = 1;
			lastRecord.page = 1;
			return lastRecord.record;
		}
		if (lastRecord.record.deleted) return readNext();
		return lastRecord.record;
	}

	if (lastRecord.record.overflowAddr() != 0) {
		readOverflowPage(lastRecord.record.overflowAddr());
		lastRecord.record = pageBuffor[(lastRecord.record.overflowAddr() - 1) % BLOCKING_FACTOR];

		if (lastRecord.record.deleted) return readNext();
		return lastRecord.record;
	}

	if (currentPage == lastRecord.page) {
		if (lastRecord.position == BLOCKING_FACTOR - 1) {
			if (currentPage >= mainAreaPages) {
				lastRecord.page = 1;
				lastRecord.position = -1;
				lastRecord.record = Polynomial();
				lastRecord.record.setKey(-1);
				return lastRecord.record;//Polynomial();	/// nie ma dalej rekordow bo nie ma stron
			}
			readPage(currentPage + 1);	/// inaczej czytaj kolejna strone
			lastRecord.record = pageBuffor[0];
			lastRecord.page = currentPage; // readpage zmienia currentpage
			lastRecord.position = 0;
		}
		else {
			lastRecord.record = pageBuffor[lastRecord.position+1];
			lastRecord.page = currentPage;
			lastRecord.position += 1;
		}
	}
	else {
		readPage(lastRecord.page);
		lastRecord.record = pageBuffor[lastRecord.position+1];
		lastRecord.page = currentPage; // readpage zmienia currentpage
		lastRecord.position++;
	}

	if (lastRecord.record.deleted ) return readNext();
	if (lastRecord.record.getKey() == 0) {
		lastRecord.position = -1;
		lastRecord.page ++;
		lastRecord.record.setOverflowAddr(0);
		return readNext();
	}
	return lastRecord.record;
}

int Disk::getNumOfKeys()
{
	return mainKeys;
}

int Disk::getOvNumOfKeys()
{
	return overflowKeys;
}

int Disk::readPage(int address)
{
	int x, key=0, ov;
	int a[5];
	int i = 0, r = 0;
	char del;
	if (currentPage == address) {
		for (int i = 0; i < BLOCKING_FACTOR; i++) 
			if (pageBuffor[i].getKey() != 0) r++;
	}
	else {
		int physicalAddress = (address - 1) * PAGE_BYTES;//zwrot_wyszukiwania_w_indexie*BLOCKING_FACTOR*Rozmiar_rekordu; //nr*strona
		mainArea.clear();
		mainArea.seekg(physicalAddress);
		memcpy(pageBuffor, zeros, sizeof(Polynomial)*BLOCKING_FACTOR);
		for (i = 0; i < BLOCKING_FACTOR; i++) {
			mainArea.read((char*)&key, 4);
			if (mainArea.eof()) break;
			mainArea.read((char*)&a[0], 4);
			mainArea.read((char*)&a[1], 4);
			mainArea.read((char*)&a[2], 4);
			mainArea.read((char*)&a[3], 4);
			mainArea.read((char*)&a[4], 4);
			mainArea.read((char*)&x, 4);
			mainArea.read((char*)&ov, 4);
			mainArea.read((char*)&del, 1);
			pageBuffor[i] = Polynomial(key, a[0], a[1], a[2], a[3], a[4], x);
			pageBuffor[i].setOverflowAddr(ov);
			pageBuffor[i].deleted = del;
			if (key != 0) {
				r++;
			}
		}
	}
	currentPage = address;
	overflowPageInMemory = -1;
	return r;
}

int Disk::readOverflowPage(int address)
{
	char del;
	int x, key=0, ov;
	int a[5];
	int r = 0;
	if (overflowPageInMemory == address) {
		for (int i = 0; i < BLOCKING_FACTOR; i++)
			if (pageBuffor[i].getKey() != 0) r++;
	}
	else {

		int physicalAddress = ((address - 1) / BLOCKING_FACTOR) * PAGE_BYTES;//zwrot_wyszukiwania_w_indexie*BLOCKING_FACTOR*Rozmiar_rekordu; //nr*strona
		overflowArea.clear();
		overflowArea.seekg(physicalAddress);
		memcpy(pageBuffor, zeros, sizeof(Polynomial)*BLOCKING_FACTOR);

		for (int i = 0; i < BLOCKING_FACTOR; i++) {
			overflowArea.read((char*)&key, 4);
			if (overflowArea.eof()) break;
			overflowArea.read((char*)&a[0], 4);
			overflowArea.read((char*)&a[1], 4);
			overflowArea.read((char*)&a[2], 4);
			overflowArea.read((char*)&a[3], 4);
			overflowArea.read((char*)&a[4], 4);
			overflowArea.read((char*)&x, 4);
			overflowArea.read((char*)&ov, 4);
			overflowArea.read((char*)&del, 1);
			pageBuffor[i] = Polynomial(key, a[0], a[1], a[2], a[3], a[4], x);
			pageBuffor[i].setOverflowAddr(ov);
			pageBuffor[i].deleted = del;
			if (key != 0) r++;
		}
	}
	currentPage = -1;
	overflowPageInMemory = address;
	return r;
}

void Disk::savePage(int address)
{
	int physicalAddress = (address - 1) * PAGE_BYTES;//zwrot_wyszukiwania_w_indexie*BLOCKING_FACTOR*Rozmiar_rekordu; //nr*strona
	mainArea.clear();
	mainArea.seekp(physicalAddress);
	int x, key, ov;
	int *a;
	int r = 0;

	for (int i = 0; i < BLOCKING_FACTOR; i++) {
		a = pageBuffor[i].getCoefficient();
		x = pageBuffor[i].getX();
		key = pageBuffor[i].getKey();
		ov = pageBuffor[i].overflowAddr();

		mainArea.write((char*)&key, 4);
		mainArea.write((char*)&a[0], 4);
		mainArea.write((char*)&a[1], 4);
		mainArea.write((char*)&a[2], 4);
		mainArea.write((char*)&a[3], 4);
		mainArea.write((char*)&a[4], 4);
		mainArea.write((char*)&x, 4);
		mainArea.write((char*)&ov, 4);
		mainArea.write((char*)&pageBuffor[i].deleted, 1);
	}
	std::flush(mainArea);
	currentPage = address;
	overflowPageInMemory = -1;
}

void Disk::saveOverflowPage(int address)
{
	int physicalAddress = ((address-1)/BLOCKING_FACTOR) * PAGE_BYTES;//zwrot_wyszukiwania_w_indexie*BLOCKING_FACTOR*Rozmiar_rekordu; //nr*strona

	overflowArea.clear();
	overflowArea.seekp(physicalAddress);
	int x, key, ov;
	int *a;
	int r = 0;

	for (int i = 0; i < BLOCKING_FACTOR; i++) {
		a = pageBuffor[i].getCoefficient();
		x = pageBuffor[i].getX();
		key = pageBuffor[i].getKey();
		ov = pageBuffor[i].overflowAddr();
		overflowArea.write((char*)&key, 4);
		overflowArea.write((char*)&a[0], 4);
		overflowArea.write((char*)&a[1], 4);
		overflowArea.write((char*)&a[2], 4);
		overflowArea.write((char*)&a[3], 4);
		overflowArea.write((char*)&a[4], 4);
		overflowArea.write((char*)&x, 4);
		overflowArea.write((char*)&ov, 4);
		overflowArea.write((char*)&pageBuffor[i].deleted, 1);
	}
	currentPage = -1;
	overflowPageInMemory = address;
	std::flush(overflowArea);
}

bool Disk::DeleteRecord(int key) {
	

	int address = this->getKeyPosition(key);
	int recordsOnPage = this->readPage(address);
	int i = 0;
	for (i = 0; i < recordsOnPage; i++) {
		if (pageBuffor[i].getKey() == key) {
			pageBuffor[i].deleted = deleted;
			savePage(address);
			mainKeys--;
			return true;
		}
		else if (pageBuffor[i].getKey() > key && i > 0) {
			Polynomial next = pageBuffor[i-1];
			while (next.overflowAddr() != 0) {
				int ovAddress = next.overflowAddr();
				readOverflowPage(ovAddress);
				next = pageBuffor[(ovAddress - 1) % BLOCKING_FACTOR];
				if (next.getKey() == key) {
					pageBuffor[(ovAddress - 1) % BLOCKING_FACTOR].deleted = deleted;
					saveOverflowPage(ovAddress);
					overflowKeys--;
					return true;
				}
			}
		}
	}

	Polynomial next = pageBuffor[i - 1];
	while (next.overflowAddr() != 0) {
		int ovAddress = next.overflowAddr();
		readOverflowPage(ovAddress);
		next = pageBuffor[(ovAddress - 1) % BLOCKING_FACTOR];
		if (next.getKey() == key) {
			pageBuffor[(ovAddress - 1) % BLOCKING_FACTOR].deleted = deleted;
			saveOverflowPage(ovAddress);
			overflowKeys--;
			return true;
		}
	}

return false;
}


void Disk::reorganize()
{
	int newMainPages = ceil(1.0*(mainKeys + overflowKeys) / (BLOCKING_FACTOR*0.5));
	std::fstream newMain;
	newMain.open("newMain.brt", std::ios::binary | std::ios::in | std::ios::out | std::ios::trunc);

	std::fstream newIndexFile;
	int newIndexPages = ceil(1.0*newMainPages / BLOCIKNG_INDEX);
	Index* newIndex = new Index[newMainPages];
	//newIndexFile.open("newIndex.brt", std::ios::binary | std::ios::in | std::ios::out | std::ios::trunc);

	int newOverflowPages = ceil(0.20*newMainPages);

	resetLastRecord();
	Polynomial newBuffer[BLOCKING_FACTOR];
	newBuffer[0].setKey(-1);
	Polynomial p = this->readNext();
	newIndex[0].address = 1;
	newIndex[0].key = p.getKey();


	int page = 1;
	int recOnPage = 1; // guard rozpoczyna
	while (p.getKey() != -1) {
		if (recOnPage > 0.5*BLOCKING_FACTOR) {
			savePage(&newMain, page, newBuffer);
			page++;
			newIndex[page - 1].address = page;
			newIndex[page - 1].key = p.getKey();
			memcpy(newBuffer, zeros, BLOCKING_FACTOR * sizeof(Polynomial));
			recOnPage = 0;
		}
		p.setOverflowAddr(0);
		newBuffer[recOnPage++] = p;
		p = this->readNext();
	}
	savePage(&newMain, page, newBuffer);

	mainKeys = mainKeys + overflowKeys;
	mainAreaPages = newMainPages;
	overflowKeys = 0;
	overflowPages = newOverflowPages;
	overflowLastAddress = 0;


	memcpy(pageBuffor, zeros, BLOCKING_FACTOR * sizeof(Polynomial));

	mainArea.close();
	newMain.close();
	std::remove("main.brt");
	std::rename("newMain.brt", "main.brt");

	overflowArea.close();
	overflowArea.open("overflow.brt", std::ios::binary | std::ios::in | std::ios::out | std::ios::trunc);
	delete[] index;
	index = newIndex;
	indexKeys = page;
	indexPages = newIndexPages;
	resetLastRecord();
	saveIndexes();
	mainArea.open("main.brt", std::ios::binary | std::ios::in | std::ios::out);
		std::cout << "Koniec reorganizacji";
}

void Disk::resetLastRecord() {
	lastRecord.position = -1;
	lastRecord.page = 1;
	lastRecord.record = Polynomial();
}

void Disk::savePage(std::fstream *stream, int address, Polynomial* buffor)
{
	int physicalAddress = (address - 1) * PAGE_BYTES;//zwrot_wyszukiwania_w_indexie*BLOCKING_FACTOR*Rozmiar_rekordu; //nr*strona
	stream->clear();
	stream->seekp(physicalAddress);
	int x, key, ov;
	int *a;
	int r = 0;

	for (int i = 0; i < BLOCKING_FACTOR; i++) {
		a = buffor[i].getCoefficient();
		x = buffor[i].getX();
		key = buffor[i].getKey();
		ov = buffor[i].overflowAddr();

		stream->write((char*)&key, 4);
		stream->write((char*)&a[0], 4);
		stream->write((char*)&a[1], 4);
		stream->write((char*)&a[2], 4);
		stream->write((char*)&a[3], 4);
		stream->write((char*)&a[4], 4);
		stream->write((char*)&x, 4);
		stream->write((char*)&ov, 4);
		stream->write((char*)&pageBuffor[i].deleted, 1);
	}
	std::flush(*stream);
}

Disk::~Disk()
{
	std::fstream conf;
	conf.open("conf.brt", std::ios::binary | std::ios::in | std::ios::out | std::ios::trunc);
	conf.write((char*)&mainKeys, 4);//-1
	conf.write((char*)&mainAreaPages, 4);
	conf.write((char*)&overflowKeys, 4);
	conf.write((char*)&overflowLastAddress, 4);
	conf.write((char*)&indexPages, 4);
	conf.write((char*)&indexKeys, 4);
	conf.close();
}
