#pragma once
#include <string>
#include <vector>
#include "Transaction.h"

namespace HorovyiBlockchain {

    class Block {
    private:
        int index;
        time_t timestamp;
        std::vector<Transaction> transactions;
        int proof;
        std::string prevHash;

    public:
        Block(int index, int proof, std::string prevHash, std::vector<Transaction> transactions);

        int getIndex() const;

        time_t getTimestamp() const;

        std::vector<Transaction> getTransactions() const;

        int getProof() const;

        std::string getPrevHash() const;

    };
}