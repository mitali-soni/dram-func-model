#ifndef PIM_H
#define PIM_H

//pim.h
//
//Header file for pim functions
//
#include <iostream>
#include <vector>
#include "buspacket.h"

using namespace std;
namespace DRAMSim
{
vector<int> pimxor(BusPacket *left, BusPacket *right);
vector<int> pimCorrector(BusPacket * busPacket);
vector<int> pimconcate(const BusPacket *left, const BusPacket *right);
vector<int> pimcomp(const BusPacket *weight, const BusPacket * stream);
}
#endif
