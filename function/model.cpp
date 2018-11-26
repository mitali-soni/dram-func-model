#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <stdint.h>

namespace DRAMSim {
class Data {
  public:
    unsigned column;
    unsigned bank;
    uint64_t address;
    Data *next;
    unsigned digits[16];

    Data(unsigned col, unsigned bk, uint64_t addr, int * dig);

};

class Bank {
  public:
    unsigned size;
    unsigned cap;
    Data *data;

    Bank(unsigned size);
    Data* find(uint64_t addr, Data* data);
    Data* pimxor(uint64_t addr1, uint64_t addr2, Data* data1, Data* data2);
    void pimRead(uint64_t addr);
    Bank* next;
};


Data::Data(unsigned col, unsigned bk, uint64_t addr, int * dig) {
    column = col;
    bank = bk;
    address = addr;
    if (dig != NULL) {
        for (int i = 0; i < 16; i++) {
            digits[i] = dig[i];
        }
    }
    
}

Bank::Bank(unsigned rows) {
    size = 0;
    cap = rows;
    data = new Data(-1, -1, -1, NULL);
    size = rows;

}

Data* Bank::find(uint64_t addr, Data* data) {
    while (data != NULL) {
        if (data -> address == addr) {
            return data;
        }
        data = data->next;
    }
    return NULL;
}

Data* Bank::pimxor(uint64_t addr1, uint64_t addr2, Data* data1, Data* data2) {
    Data* left = find(addr1, data1);
    Data* right = find(addr2, data2);
    int digits[16] = {0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    Data* res = new Data(-1, -1, -1, digits);

    if (left != NULL && right != NULL) {
        for (int i = 0; i < 10; i++) {
            res -> digits[i] = (left -> digits[i]) ^ (right -> digits[i]);
        }

        return res;
    }

    return NULL;
}



}

int main(int argc, char *argv[]) {
    unsigned num = 10;
    printf("input:\n");
    int digits1[16] = {0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1};
    int digits2[16] = {1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1};
    for (int i = 0; i < 16; i++) {
        printf("%d ", digits1[i]);
    }
    printf("\n");

    DRAMSim::Bank * nbank = new DRAMSim::Bank(num);

    nbank->data->next = new DRAMSim::Data(10, 10, 111, digits1);
    nbank->next = new DRAMSim::Bank(num);

    DRAMSim::Bank * bank1 = new DRAMSim::Bank(2);
    DRAMSim::Bank * bank2 = new DRAMSim::Bank(2);

    bank1->data->next = new DRAMSim::Data(10, 10, 1, digits1);
    bank2->data->next = new DRAMSim::Data(10, 10, 2, digits2);

    DRAMSim::Bank bk = DRAMSim::Bank(num);
    DRAMSim::Data * res = bk.pimxor(1, 2, bank1->data, bank2->data);

    

    for (int i = 0; i < 16; i++) {
        printf("%d ", digits2[i]);
    }
    printf("\n");
    printf("result: \n");

    for (int i = 0; i < 16; i++) {
        printf("%d ", res->digits[i]);
    }
    printf("\n");

   return 0;

}