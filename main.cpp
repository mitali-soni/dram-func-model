#include "bank.h"
#include "buspacket.h"
#include <iostream>
#include <fstream>
#include <iomanip>

using namespace DRAMSim;

unsigned NUM_BANKS = 8;

//enum packetType {DATA, DATA_READ, DATA_WRITE};

int main( ) {
	//BusPacketType bpType = transaction->getBusPacketType();
	//MemoryController *memoryController;
	//(*ranks)[outgoingCmdPacket->rank]->receiveFromBus(outgoingCmdPacket);
	
	BusPacket *outgoingDataPacket;
	vector<BusPacket *> readReturnPacket;

  std::filebuf fb;
  fb.open ("testout.log",std::ios::out);
//  std::ostream os(&fb);
//  os << "Test sentence\n";

	ostream dramsim_log(&fb);
	vector<Bank> banks(NUM_BANKS, Bank(dramsim_log));
	
	BusPacketType bpType; //Dummy code for asserts
	bpType = READ;

	//BusPacket *command = new BusPacket(bpType, transaction->address,
	//		newTransactionColumn, newTransactionRow, newTransactionRank,
	//		newTransactionBank, transaction->data, dramsim_log);
        
	//map address to rank,bank,row,col
	unsigned newTransactionChan, newTransactionRank, newTransactionBank, newTransactionRow, newTransactionColumn;
	BusPacket *command = new BusPacket(bpType, 0, newTransactionColumn, newTransactionRow, 0, 0, NULL, dramsim_log);

	// Packet field used in read() : column, row for returning data: data, busPacketType
	// Packet field used in write() : column, row, data
        
	//FixMe: Logic to select random bank numbers
	unsigned int random_bank_num = 0;
	banks[random_bank_num].read(command);
	cout<<std::hex<<command->data<<endl;
  fb.close();
}
