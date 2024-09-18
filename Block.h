#pragma once
#include <string>
#include <vector>
#include "Transaction.h"

class Block{
    private:
        int index;
        time_t timestamp;
        std::vector<Transaction> transactions;
        int proof;
        std::string prevHash; 

    public:
        Block(int index, int proof, std::string prevHash, std::vector<Transaction> transactions);

        int getIndex();

        time_t getTimestamp();

        std::vector<Transaction> getTransactions();

        int getProof();

        std::string getPrevHash();

};