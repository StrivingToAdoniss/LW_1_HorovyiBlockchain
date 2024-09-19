#include "Block.h"
#include <chrono>
#include <ctime>    


Block::Block(int index, int proof, std::string prevHash, std::vector<Transaction> transactions){
    time_t start = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    
    this-> index = index;
    this-> proof = proof;
    this-> prevHash = prevHash;
    this-> transactions = transactions;

    time_t end = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

    this->timestamp = end;

}

int Block::getIndex(){
    return this->index;
}

time_t Block::getTimestamp(){
    return this->timestamp;
}

std::vector<Transaction> Block::getTransactions(){
    return this->transactions;
}

int Block::getProof(){
    return this->proof;
}

std::string Block::getPrevHash(){
    return this->prevHash;
}