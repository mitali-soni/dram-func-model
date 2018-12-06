#include "bank.h"
#include "buspacket.h"
#include "pim.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <time.h>
#include <stdlib.h>
#include <stdint.h>
#include <sstream>
#include <string>
#include <assert.h>
#include <math.h> 

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

void printVector(vector <int> vec) {

		vector<int>::iterator iter = vec.begin();
	while (iter != vec.end())
	{
		//printf("%d ", *iter);
		iter++;
	}
	//cout <<endl;
}


vector<int> pim_xor(BusPacket *left, BusPacket *right)
{
	vector<int> res;
	if (left -> data != NULL && right -> data != NULL) {
		vector<int> * p = (vector<int> * )left -> data;
		vector<int> * q = (vector<int> * )right -> data;
        
        	vector<int>::iterator iterp = p -> begin();
		vector<int>::iterator iterq = q -> begin();
		

		while (iterp != p -> end() && iterq != q -> end()) 
		{
			int num1 = (* iterp);
			int num2 = (*iterq);
			if (num1 == 1 && num2 == 0)
			{
				res.push_back(1);
			} 
			else if (num1 == 0 && num2 == 1) 
			{
				res.push_back(1);
			}
			else
			{
				res.push_back(0);
			}
			 
			//:printf("%d \n", tmp);
			//res.push_back(tmp);
			iterp++;
			iterq++;
		}

		


	}
	return res;

}

vector<int> pim_Corrector(vector <int> data)
{
	vector<int> res;
	vector<int> lvl1;
	vector<int> lvl2;
	vector<int> lvl3;
        //Encoding of I : 00 -> 2, 11 ->3
	
	int size = 0;
	vector<int>::iterator iterp = data.begin();
	while (iterp != data.end()) 
	{       iterp++;
		size++;
	}

	iterp = data.begin();
	//cout << "Inside corrector, printing input" << endl;
	//printVector(data);

	while (iterp + 1 <= data.end())
	{
		int tmp1 = (*iterp);
		iterp++;
		int tmp2 = (*iterp);
		iterp++;
		if (tmp1 == 0 && tmp2 == 1) {
			lvl1.push_back(0);
		} else if (tmp1 == 1 && tmp2 == 0) {
			lvl1.push_back(1);
		} else if (tmp1 == 1 && tmp2 == 1) {
			lvl1.push_back(3);
		} else if (tmp1 == 0 && tmp2 == 0) {
			lvl1.push_back(2);
		}
	}
        //cout << "Level1 output: " << endl;
	//printVector(lvl1);
	
	iterp = lvl1.begin();
	while (iterp + 1 <= lvl1.end())
	{
		int tmp1 = (*iterp);
		iterp++;
		int tmp2 = (*iterp);
		iterp++;
		if ((tmp1 == 2 || tmp1 == 3) && (tmp2 == 2 || tmp2 == 3)) {
			lvl2.push_back(tmp1);
			lvl2.push_back(tmp2);
		}
		else if ((tmp1 == 2 || tmp1 == 3) && (tmp2 == 0 || tmp2 == 1)) {
			lvl2.push_back(tmp2);
			lvl2.push_back(tmp1);
		}
		else {
			lvl2.push_back(tmp1);
			lvl2.push_back(tmp2);
		}
	}
        //cout << "Level2 output: " << endl;
	//printVector(lvl2);

	iterp = lvl2.begin();
	while (iterp + 3 <= lvl2.end())
	{
		int tmp1 = (*iterp);
		iterp++;
		int tmp2 = (*iterp);
		iterp++;
		int tmp3 = (*iterp);
		iterp++;
		int tmp4 = (*iterp);
		iterp++;
		if ((tmp1 == 2 || tmp1 == 3) && (tmp3 == 2 || tmp3 == 3)) {
			lvl3.push_back(tmp1);
			lvl3.push_back(tmp2);
			lvl3.push_back(tmp3);
			lvl3.push_back(tmp4);
		}
		else if ((tmp1 == 2 || tmp1 == 3) && (tmp3 == 0 || tmp3 == 1)) {
			lvl3.push_back(tmp3);
			lvl3.push_back(tmp4);
			lvl3.push_back(tmp1);
			lvl3.push_back(tmp2);
		}
		else {
			lvl3.push_back(tmp1);
			lvl3.push_back(tmp2);
			lvl3.push_back(tmp3);
			lvl3.push_back(tmp4);
		}
	}
        //cout << "Level3 output: " << endl;
	//printVector(lvl3);


	iterp = lvl3.begin();
	while (iterp <= lvl3.end())
	{
		int tmp1 = (*iterp);
		iterp++;
		if (tmp1 == 2) {
			res.push_back(0);
		}
		else if (tmp1 == 3) { 
			res.push_back(1);
		}
		else {
			res.push_back(tmp1);
		}
	}
        //cout << "Final output: " << endl;
	//printVector(res);

	return res;
}


vector<int> pim_concatenate (vector<int> left, vector<int> right )
{
        vector<int> res;        
	vector<int>::iterator iterp = left.begin();

	while (iterp != left.end()) 
	{
		res.push_back(*iterp);
		iterp++;
	}

	vector<int>::iterator iterq = right.begin();
	while (iterq != right.end())
	{
		res.push_back(*iterq);
		iterq++;

	}

	return res;

}

int pim_comp(float weight, vector <int> stream, int num_col_acc)
{
	int res;
	vector<int>::iterator iterq = stream.begin();

	float scaled_weight = weight * (pow( 2, num_col_acc) - 1);
        //Need logic to convert the binary vector back to the decimal domain for easy comparison
	//Comparison logic:
	//if (weight >= random num) output bit = 1
	//else output bit =0
	int value = 0;
	while(iterq != stream.end()){
        	value |= *iterq;
        	value <<= 1;
		iterq++;
	}
	//cout << "Weight " << scaled_weight << endl; 
	//cout << "Random val: " << value << endl; 
        if (scaled_weight >= value) {
		res = 1; 
	}
	else {
		res = 0;
	}
	return res;
}
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


	ofstream bsfile;
  	bsfile.open ("bitstreams.txt");
  	bsfile << "#Weight Bitstream\n";

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

	float weights[16] = {0.5, 0.75, 0.60, 0.25, 0.1, 0.3, 0.9, 0.8, 0.43, 0.69, 0.39, 0.81, 0.89, 0.77, 0.99, 0.55};   
	while (std::getline(infile, line))
	{
		//cout<< "about to read" << endl;
    		std::istringstream iss(line);
    		int a, b, data1;
		int count = 0; 
		//cout << line << endl;
    		iss >> std::hex >> a >> b; 
		//cout << "Bank Index: " << a << endl;
		//cout << "Row Index: " << b << endl;
		while(iss>> std::hex >> data1) {
		     int * data = (int*)calloc(1, sizeof(int));
		     *data = data1;
                     //cout << "Data" << *data << endl;
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
	
	//int count = 16; //FIXME: Set this value for 256 random number and bit streams 
	int count = 2; //FIXME: Set this correctly
	int r; 
	while(count>0) { //Outermost while loop, control the number of final output bitstreams generated
		int count1 = 0;
		int a[NUM_BANKS_RAND];
		while(count1 < NUM_BANKS_RAND) {  //selects the four banks randomly
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
		int cntl_granularity = NUM_COLS_ACC/2; // if NUM_COLS_ACC is 16, then we will read the 2B from each of the four banks 8 times
                //Caution: While printing traces, make sure you set this to 1, because printing trace logic is smart one and alredy accounts for granularity of accesses
		bsfile << weights[count]<< " "; 
		while (cntl_granularity > 0 ) { 
			BusPacket* vals[NUM_BANKS_RAND]; //stores values corresponding to the four randomly chosen banks
			for (int i=0; i<NUM_BANKS_RAND; i++) { //reads 2B from each of the 4 banks chosen by the random logic
				newTransactionCol = banks[a[i]].lastAccessedCol; 
        		        newTransactionRow = banks[a[i]].lastAccessedRow;
				BusPacket *command = new BusPacket(bpType, 0, newTransactionCol, newTransactionRow, 0, 0, NULL, dramsim_log);
				banks[a[i]].read(command);
				//vals[i] = *(int*)command->data;
				vals[i] = command;
				if (banks[a[i]].lastAccessedCol >= NUM_COLS) {
        		        	banks[a[i]].lastAccessedRow++;
					banks[a[i]].lastAccessedCol = 0;  
				}
				banks[a[i]].printTrace(a[i], NUM_COLS_ACC, tracefile);
				banks[a[i]].lastAccessedCol += NUM_COLS_ACC;
				//cout << "Bank Index: " << a[i] << endl;
			}

			//generate the bit vector of each data
			vector<int> data[NUM_BANKS_RAND];
			//cout << "Printing contents" << endl;
			for (int i=0; i<NUM_BANKS_RAND; i++) {
				//int2bitvector(*(int*)(vals[i]);
				int value = *(int*)(vals[i]->data);
				for(int j=0; j < 16; j++) {
					data[i].insert(data[i].begin(), (value >> j) & 1);
				}
				vals[i]->data = (void*)(&data[i]);
				//cout<<value<<endl;
			}
        		//post process the data read from each bank -> xor, concat, corrector
			vector<int> xor1 = pim_xor(vals[0], vals[1]);
			vector<int> xor2 = pim_xor(vals[2], vals[3]);
        		printVector(data[0]); 
        		printVector(data[1]); 
        		printVector(xor1); 
        		printVector(data[2]); 
        		printVector(data[3]); 
        		printVector(xor2); 
			vector<int> concatenate = pim_concatenate(xor1, xor2);
        		printVector(concatenate); 
			//cout<< "Concatenate Output" << endl;
			vector<int> dataCorrector = pim_Corrector(concatenate);
			//cout<<"Corrector Output" << endl;
        		printVector(dataCorrector);
			int outputbit = pim_comp(weights[count], dataCorrector, NUM_COLS_ACC);
			//cout<<"Final Output Bit: " <<outputbit<< endl;
			cntl_granularity --;
			bsfile << outputbit ;

		}
		bsfile << "\n";
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

	fb.close();
  	tracefile.close();
  	bsfile.close();
}
