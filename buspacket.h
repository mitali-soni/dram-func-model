#ifndef BUSPACKET_H
#define BUSPACKET_H
//BusPacket.h
//
//Header file for bus packet object
//
 #include <stdint.h>
#include <fstream>
using namespace std;
namespace DRAMSim
{
enum BusPacketType
{
	READ,
	READ_P,
	WRITE,
	WRITE_P,
	ACTIVATE,
	PRECHARGE,
	REFRESH,
	DATA
};

class BusPacket
{
	BusPacket();
	ostream &dramsim_log; 
public:
	//Fields
	BusPacketType busPacketType;
	unsigned column;
	unsigned row;
	unsigned bank;
	unsigned rank;
	uint64_t physicalAddress;
	void *data;

	//Functions
	BusPacket(BusPacketType packtype, uint64_t physicalAddr, unsigned col, unsigned rw, unsigned r, unsigned b, void *dat, ostream &dramsim_log_);

	void print();
	void print(uint64_t currentClockCycle, bool dataStart);
	void printData() const;

};
}

#endif

