
#include <iostream>
#include "Polynomial.h"
#include "Disk.h"
#include <random>
int main()
{
	Disk d;
	for (int i = 1; i < 1000; i++) {
		int key = std::rand() % 2010 + 1;
		std::cout << key << "\n";
		Polynomial p(key,
					std::rand() % 100 + 1,
					std::rand() % 100 + 1,
					std::rand() % 100 + 1,
					std::rand() % 100 + 1,
					std::rand() % 100 + 1,
					std::rand() % 100 + 1);
		d.save(p);
	}
	//d.DeleteRecord(23);
	//d.DeleteRecord(5);
	std::cout << "_______________________" << std::endl;
	std::cout << "_______________________" << std::endl;
	std::cout << "_______________________" << std::endl;
	
	Polynomial p;
	p.setKey(1);
	while(p.getKey() != -1){
		p = d.readNext();
		std::cout << p.getKey() << std::endl;
	}
	std::cout << "Kluczy gł: " << d.getNumOfKeys() << std::endl;
	std::cout << "Kluczy ov: " << d.getOvNumOfKeys() << std::endl;
	return 0;
}
