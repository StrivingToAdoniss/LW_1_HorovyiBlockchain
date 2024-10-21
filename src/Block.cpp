#include "Block.hpp"
#include <chrono>
#include <ctime>    

namespace HorovyiBlockchain {

    Block::Block(int index, int proof, std::string prevHash, std::vector<Transaction> transactions) {
        time_t start = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

        this->index = index;
        this->proof = proof;
        this->prevHash = prevHash;
        this->transactions = transactions;

        time_t end = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

        this->timestamp = end;

    }

    Block::Block(){}

    int Block::getIndex() const {
        return this->index;
    }

    time_t Block::getTimestamp() const {
        return this->timestamp;
    }

    std::vector<Transaction> Block::getTransactions() const {
        return this->transactions;
    }

    int Block::getProof() const {
        return this->proof;
    }

    std::string Block::getPrevHash() const {
        return this->prevHash;
    }
}