#include "Block.h"

Block::Block(int index, int proof, std::string prevHash, std::vector<Transaction> transactions){
    this-> index = index;
    this-> proof = proof;
    this-> prevHash = prevHash;
    this-> transactions = transactions;
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