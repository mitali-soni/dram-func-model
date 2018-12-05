#ifndef BANK_H
#define BANK_H

//Bank.h
//
//Header file for bank class
//
#include <iostream>
#include <vector>
#include "buspacket.h"

using namespace std;
namespace DRAMSim
{
class Bank
{
	typedef struct _DataStruct
	{
		unsigned row;
		void *data;
		struct _DataStruct *next;
	} DataStruct;

public:
	//functions
	Bank(ostream &dramsim_log_);
	void read(BusPacket *busPacket);
	void write(const BusPacket *busPacket);
	void load(const BusPacket *busPacket, vector<int> * value);

	vector<int> pimxor(BusPacket *left, BusPacket *right);
	vector<int> pimCorrector(BusPacket * busPacket);
	vector<int> pimconcate(const BusPacket *left, const BusPacket *right);
	vector<int> pimcomp(const BusPacket *weight, const BusPacket * stream);
    


private:
	// private member
	std::vector<DataStruct *> rowEntries;
	ostream &dramsim_log; 

	static DataStruct *searchForRow(unsigned row, DataStruct *head);
};
}

#endif

