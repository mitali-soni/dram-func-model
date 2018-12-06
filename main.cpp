#include "bank.h"
#include "buspacket.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <time.h>
#include <stdlib.h>
#include <stdint.h>
#include <sstream>
#include <string>
#include <assert.h>

using namespace DRAMSim;
#define NUM_BANKS_RAND 4
#define NUM_COLS_ACC 16
//#define NUM_COLS_ACC 32
//#define NUM_COLS_ACC 64
//#define NUM_COLS_ACC 128
//#define NUM_COLS_ACC 256
//#define NUM_COLS_ACC 512

//To generate various granularity of traces, change the filename and NUM_COLS_ACC
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

int search(int * arr, int elem, int size) {
	for (int i=0; i< size; i++) {
		if (arr[i] == elem) {
			return 1; 
		}
	}
	return 0;
}

int main( ) {

	BusPacket *outgoingDataPacket;
	vector<BusPacket *> readReturnPacket;

	std::filebuf fb;
	fb.open ("testout.log",std::ios::out);

	ofstream tracefile;
  	tracefile.open ("trace_16B.trc");
  	tracefile << "#Bank Row Col\n";

	ostream dramsim_log(&fb);
	vector<Bank> banks(NUM_BANKS, Bank(dramsim_log));
   
	BusPacketType bpType; //Dummy code for asserts
	bpType = READ;

	unsigned newTransactionChan, newTransactionRank, newTransactionBank, newTransactionRow, newTransactionCol;
	BusPacket *command = new BusPacket(bpType, 0, newTransactionCol, newTransactionRow, 0, 0, NULL, dramsim_log);

	unsigned int random_bank_num = 0;
	banks[random_bank_num].read(command);
	//cout<<std::hex<<command->data<<endl;

	//logic to parse the input file
	//Format of the input file : 3 bit bank indes, 13 bit row index, 0x1234 0x4567 0x1256 ... 1024 values
	std::ifstream infile("input_data.txt");

	std::string line;
	const char *hexstring = "0xabcdef0";
	int number = (int)strtol(hexstring, NULL, 0);

	while (std::getline(infile, line))
	{
		cout<< "about to read" << endl;
    		std::istringstream iss(line);
    		int a, b, data1;
		int count = 0; 
		cout << line << endl;
    		iss >> std::hex >> a >> b; 
		cout << "Bank Index: " << a << endl;
		cout << "Row Index: " << b << endl;
		while(iss>> std::hex >> data1) {
		     int * data = (int*)calloc(1, sizeof(int));
		     *data = data1;
                     cout << "Data" << *data << endl;
		     newTransactionCol = count; 
                     newTransactionRow = b;
		     BusPacket *command = new BusPacket(bpType, 0, newTransactionCol, newTransactionRow, 0, 0, (void*)data, dramsim_log);
		     banks[a].write(command);
		     count++;
		}
		//assert(count == NUM_COLS);
		//cout << "Data: " << c << endl;
	}

	srand(time(NULL));   // Initialization, should only be called once.
	//int r = rand() % (max_number + 1 - minimum_number) + minimum_number
	
	//int count = 256; //FIXME: Set this value for 256 random number and bit streams 
	int count = 256; //FIXME: Set this correctly
	int r; 
	while(count>0) {
		int count1 = 0;
		int a[NUM_BANKS_RAND];
		while(count1 < NUM_BANKS_RAND) {
			r = rand() % 7 + 1; 
			if (search(a, r, count1)) {
				continue;
			}
			else {
				a[count1] = r;
				count1++;
				//printf("count1 = %d lfsr = %d\n", count1, r);
			}
		}
		for (int i=0; i<NUM_BANKS_RAND; i++) {
			newTransactionCol = banks[a[i]].lastAccessedCol; 
                        newTransactionRow = banks[a[i]].lastAccessedRow;
			BusPacket *command = new BusPacket(bpType, 0, newTransactionCol, newTransactionRow, 0, 0, NULL, dramsim_log);
			banks[a[i]].read(command);
			if (banks[a[i]].lastAccessedCol >= NUM_COLS) {
                        	banks[a[i]].lastAccessedRow++;
				banks[a[i]].lastAccessedCol = 0;  
			}
			banks[a[i]].printTrace(a[i], NUM_COLS_ACC, tracefile);
			banks[a[i]].lastAccessedCol += NUM_COLS_ACC;

		}

		//Special case of bank that contains weights, in our case that's bank 0, this part is only for the traces

		newTransactionCol = banks[0].lastAccessedCol; 
                newTransactionRow = banks[0].lastAccessedRow;
		BusPacket *command = new BusPacket(bpType, 0, newTransactionCol, newTransactionRow, 0, 0, NULL, dramsim_log);
		banks[0].read(command);
		if (banks[0].lastAccessedCol >= NUM_COLS) {
                	banks[0].lastAccessedRow++;
			banks[0].lastAccessedCol = 0;  
		}
		banks[0].printTrace(0, 2, tracefile); //we need to print only the first column address for the weights bank
		banks[0].lastAccessedCol += 2;
		count--;
	}


	//Logic to initialize the banks with the above mentioned data
	
	
	
	//    	unsigned bank0 = 0;
	//	unsigned banki = 2;
	//	unsigned bankj = 3;
	//	unsigned bankk = 4;
	//	unsigned bankh = 5;
	//
	//    
	//    	vector<int> datai;
	//	vector<int> dataj;
	//	vector<int> datak;
	//	vector<int> datam;
	//	vector<int> weight;
	//
	//	
	//        int ii[16] = {0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 1, 1, 1, 1};
	//	int jj[16] = {1, 0, 0, 1, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 0, 0};
	//	int kk[16] = {0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 0, 1};
	//	int mm[16] = {1, 1, 1, 0, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 0};
	//
	//	int ww[32] = {1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1};
	//
	//        //copy(ii, ii + 16, datai.begin());
	//	for (int i = 0; i < 16; i++) 
	//	{
	//	    datai.push_back(ii[i]);
	//      	    dataj.push_back(jj[i]);
	//	    datak.push_back(kk[i]);
	//	    datam.push_back(mm[i]);
	//	}
	//
	//	for (int i = 0; i < 32; i++)
	//	{
	//	    weight.push_back(ww[i]);
	//	}
	//
	//	BusPacket *commandI = new BusPacket(bpType, 0, newTransactionColumn, newTransactionRow, 0, banki, NULL, dramsim_log);
	//	
	//	commandI->data = (void *)(&datai);
	//		
	//	BusPacket *commandJ = new BusPacket(bpType, 0, newTransactionColumn, newTransactionRow, 0, bankj, NULL, dramsim_log);
	//        commandJ -> data = (void *)(&dataj);
	//
	//	BusPacket *commandK = new BusPacket(bpType, 0, newTransactionColumn, newTransactionRow, 0, bankk, NULL, dramsim_log);
	//	commandK -> data = (void *)(&datak);
	//
	//	BusPacket *commandM = new BusPacket(bpType, 0, newTransactionColumn, newTransactionRow, 0, bankh, NULL, dramsim_log);
	//	commandM -> data = (void *)(&datam);
	//
	//	//printValue(commandI);
	//	//std::cout<<std::endl;
	//	//printValue(commandJ);
	//	//std::cout<<std::endl;
	//
	//        // This part is for xor operations
	//	BusPacket *outputIJ = new BusPacket(bpType, 0, newTransactionColumn, newTransactionRow, 0, banki, NULL, dramsim_log);
	//	
	//
	//	vector<int> dataij = banks[banki].pimxor(commandI, commandJ);
	//	outputIJ -> data = (void  *)(&dataij);
	//	printValue(outputIJ);
	//	std::cout<<std::endl;
	//
	//	BusPacket *outputKM = new BusPacket(bpType, 0, newTransactionColumn, newTransactionRow, 0, bankk, NULL, dramsim_log);
	//	
	//
	//	vector<int> datakm = banks[bankj].pimxor(commandK, commandM);
	//	outputKM -> data = (void *)(&datakm);
	//	printValue(outputKM);
	//	std::cout<<std::endl;
	//	
	//	BusPacket *outputIM = new BusPacket(bpType, 0, newTransactionColumn, newTransactionRow, 0, banki, NULL, dramsim_log);
	//	
	//	vector<int> dataim = banks[banki].pimconcate(outputIJ, outputKM);
	//        outputIM -> data = (void *)(&dataim);
	//	printValue(outputIM);
	//	std::cout<<std::endl;
	//
	//        //this part is for corrector
	//	BusPacket *outputCorrector = new BusPacket(bpType, 0, newTransactionColumn, newTransactionRow, 0, banki, NULL, dramsim_log);
	//	vector<int> dataCorrector = banks[banki].pimCorrector(outputIM);
	//        outputCorrector -> data = (void *)(&dataCorrector);
	//	printValue(outputCorrector);
	//	std::cout<<std::endl;
	//
	//     
	//        /*
	//        for (int i = 0; i < 3; i++)
	//	{
	//		weight.push_back(1);
	//	}
	//	for (int i = 0; i < 7; i++)
	//	{
	//		weight.push_back(0);
	//	}
	//	for (int i = 0; i < 22; i++)
	//	{
	//		weight.push_back(1);
	//	}*/
	//
	//        // This part is for comparator
	//        BusPacket *commandWeight = new BusPacket(bpType, 0, newTransactionColumn, newTransactionRow, 0, bank0, NULL, dramsim_log);
	//	commandWeight -> data = (void *)(&weight);
	//	vector<int> dataoutput = banks[bankk].pimcomp(commandWeight, outputCorrector);
	//        
	//	BusPacket *output = new BusPacket(bpType, 0, newTransactionColumn, newTransactionRow, 0, banki, NULL, dramsim_log);
	//	output -> data = (void * )(&dataoutput);
	//	printValue(commandWeight);
	//	std::cout<<std::endl;
	//	printValue(output);




	fb.close();
  	tracefile.close();
}
