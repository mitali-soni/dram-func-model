#include "bank.h"
#include "buspacket.h"
#include <iostream>
#include <fstream>
#include <iomanip>

using namespace DRAMSim;

unsigned NUM_BANKS = 8;

//enum packetType {DATA, DATA_READ, DATA_WRITE};

void printValue(BusPacket * busPacket)
{
    if (busPacket->data == NULL) 
    {
	    return;
    }
    vector<int> * p = (vector<int> *) busPacket -> data;

    vector<int>::iterator iter = p->begin();

    while (iter != p -> end())
    {
	    printf("%d ", *iter);
	    iter++;
    }


}

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

    	unsigned bank0 = 0;
	unsigned banki = 2;
	unsigned bankj = 3;
	unsigned bankk = 4;
	unsigned bankh = 5;

    
    	vector<int> datai;
	vector<int> dataj;
	vector<int> datak;
	vector<int> datam;
	vector<int> weight;

	
        int ii[16] = {0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 1, 1, 1, 1};
	int jj[16] = {1, 0, 0, 1, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 0, 0};
	int kk[16] = {0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 0, 1};
	int mm[16] = {1, 1, 1, 0, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 0};

	int ww[32] = {1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1};

        //copy(ii, ii + 16, datai.begin());
	for (int i = 0; i < 16; i++) 
	{
	    datai.push_back(ii[i]);
      	    dataj.push_back(jj[i]);
	    datak.push_back(kk[i]);
	    datam.push_back(mm[i]);
	}

	for (int i = 0; i < 32; i++)
	{
	    weight.push_back(ww[i]);
	}

	BusPacket *commandI = new BusPacket(bpType, 0, newTransactionColumn, newTransactionRow, 0, banki, NULL, dramsim_log);
	
	commandI->data = (void *)(&datai);
		
	BusPacket *commandJ = new BusPacket(bpType, 0, newTransactionColumn, newTransactionRow, 0, bankj, NULL, dramsim_log);
        commandJ -> data = (void *)(&dataj);

	BusPacket *commandK = new BusPacket(bpType, 0, newTransactionColumn, newTransactionRow, 0, bankk, NULL, dramsim_log);
	commandK -> data = (void *)(&datak);

	BusPacket *commandM = new BusPacket(bpType, 0, newTransactionColumn, newTransactionRow, 0, bankh, NULL, dramsim_log);
	commandM -> data = (void *)(&datam);

	//printValue(commandI);
	//std::cout<<std::endl;
	//printValue(commandJ);
	//std::cout<<std::endl;

        // This part is for xor operations
	BusPacket *outputIJ = new BusPacket(bpType, 0, newTransactionColumn, newTransactionRow, 0, banki, NULL, dramsim_log);
	

	vector<int> dataij = banks[banki].pimxor(commandI, commandJ);
	outputIJ -> data = (void  *)(&dataij);
	printValue(outputIJ);
	std::cout<<std::endl;

	BusPacket *outputKM = new BusPacket(bpType, 0, newTransactionColumn, newTransactionRow, 0, bankk, NULL, dramsim_log);
	

	vector<int> datakm = banks[bankj].pimxor(commandK, commandM);
	outputKM -> data = (void *)(&datakm);
	printValue(outputKM);
	std::cout<<std::endl;
	
	BusPacket *outputIM = new BusPacket(bpType, 0, newTransactionColumn, newTransactionRow, 0, banki, NULL, dramsim_log);
	
	vector<int> dataim = banks[banki].pimconcate(outputIJ, outputKM);
        outputIM -> data = (void *)(&dataim);
	printValue(outputIM);
	std::cout<<std::endl;

        //this part is for corrector
	BusPacket *outputCorrector = new BusPacket(bpType, 0, newTransactionColumn, newTransactionRow, 0, banki, NULL, dramsim_log);
	vector<int> dataCorrector = banks[banki].pimCorrector(outputIM);
        outputCorrector -> data = (void *)(&dataCorrector);
	printValue(outputCorrector);
	std::cout<<std::endl;

     
        /*
        for (int i = 0; i < 3; i++)
	{
		weight.push_back(1);
	}
	for (int i = 0; i < 7; i++)
	{
		weight.push_back(0);
	}
	for (int i = 0; i < 22; i++)
	{
		weight.push_back(1);
	}*/

        // This part is for comparator
        BusPacket *commandWeight = new BusPacket(bpType, 0, newTransactionColumn, newTransactionRow, 0, bank0, NULL, dramsim_log);
	commandWeight -> data = (void *)(&weight);
	vector<int> dataoutput = banks[bankk].pimcomp(commandWeight, outputCorrector);
        
	BusPacket *output = new BusPacket(bpType, 0, newTransactionColumn, newTransactionRow, 0, banki, NULL, dramsim_log);
	output -> data = (void * )(&dataoutput);
	printValue(commandWeight);
	std::cout<<std::endl;
	printValue(output);

	


  fb.close();
}
