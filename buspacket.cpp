#include "buspacket.h"
#include <fstream>
#include <iostream>

using namespace std;
using namespace DRAMSim;

BusPacket::BusPacket(BusPacketType packtype, uint64_t physicalAddr, 
		unsigned col, unsigned rw, unsigned r, unsigned b, void *dat, 
		ostream &dramsim_log_) :
	dramsim_log(dramsim_log_),
	busPacketType(packtype),
	column(col),
	row(rw),
	bank(b),
	rank(r),
	physicalAddress(physicalAddr),
	data(dat)
{}

void BusPacket::printData() const 
{
	if (data == NULL)
	{
		cout<<"NO DATA"<<endl;
		return;
	}
	//PRINTN("'" << hex);
	for (int i=0; i < 4; i++)
	{
		cout<<((uint64_t *)data)[i]<<endl;
	}
	//PRINTN("'" << dec);
}
