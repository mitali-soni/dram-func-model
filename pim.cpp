#include "buspacket.h"
#include "pim.h"
#include <cstdlib>
#include <iomanip>

using namespace std;
using namespace DRAMSim;
vector<int> pimxor(BusPacket *left, BusPacket *right)
{
	vector<int> res;
	if (left -> data != NULL && right -> data != NULL) {
		vector<int> * p = (vector<int> * )left -> data;
		vector<int> * q = (vector<int> * )right -> data;
        
        	vector<int>::iterator iterp = p -> begin();
		vector<int>::iterator iterq = q -> begin();
		

		while (iterp != p -> end() && iterq != q -> end()) 
		{
			//printf("%d\n", *iterp);
			//printf("%d\n",*iterq);
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


vector<int> pimCorrector(BusPacket * busPacket)
{
	vector<int> res;

	if (busPacket-> data == NULL)
	{
		return res;
	}

	vector<int> *p = (vector<int> *) busPacket -> data;

	int size = 0;
	vector<int>::iterator iterp = p -> begin();
	while (iterp != p -> end()) 
	{       iterp++;
		size++;
	}

	iterp = p -> begin();
	
	while (iterp <= p->end())
	{
		int tmp1 = (*iterp);
		iterp++;
		int tmp2 = (*iterp);
		iterp++;
		if (tmp1 == 0 && tmp2 == 1) {
			res.push_back(0);
		} else if (tmp1 == 1 && tmp2 == 0) {
			res.push_back(1);
		}
	}
	
	if (res.size() ==  size) {
		return res;
	}

	vector<int>::iterator iterq = p -> begin();
	while (res.size() < size) 
	{
		res.push_back(* iterq);
		iterq++;
	}

	return res;
}


vector<int> pimconcate(const BusPacket *left, const BusPacket *right)
{
        vector<int> res;        
	if (left -> data != NULL && right -> data != NULL) {
		
		vector<int> * p = (vector<int> *) left -> data;
		vector<int>::iterator iterp = p -> begin();

		while (iterp != p -> end()) 
		{
			res.push_back(*iterp);
			iterp++;
		}

		vector<int> * q = (vector<int> *) right -> data;
		vector<int>::iterator iterq = q -> begin();
		while (iterq != q -> end())
		{
			res.push_back(*iterq);
			iterq++;

		}

        
	}
	return res;

}

vector<int> pimcomp(const BusPacket *weight, const BusPacket * stream)
{
	vector<int> res;
	if (weight -> data != NULL && stream -> data != NULL) 
	{
		vector<int> * p = (vector<int> *) weight -> data;
		vector<int> * q = (vector<int> *) stream -> data;

		vector<int>::iterator iterp = p -> begin();
		vector<int>::iterator iterq = q -> begin();

		if (p->size() != q->size()) 
		{
			return res;
		}

		for (int i = 0; i < p-> size(); i++) 
		{
			if (*iterq == 1)
			{
				res.push_back(*iterp);
			}
			else
			{
				res.push_back(0);
			}
			iterp++;
			iterq++;
		}
		
	}
	return res;
}


