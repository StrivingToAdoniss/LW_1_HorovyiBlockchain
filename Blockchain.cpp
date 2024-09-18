#include "Blockchain.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <iomanip>
#include <sstream>
#include "sha.h"
#include "hex.h"
#include "filters.h"

Blockchain::Blockchain(size_t hash, std::string prevHash){
    newBlock(hash, prevHash);
    srand(time(0));
}

bool Blockchain::isProofValid(int lastProof, int proof) {
    // Concatenate lastProof and proof into a string
    std::string guessString = std::to_string(lastProof) + std::to_string(proof);

    // Compute SHA-256 hash of guessString using Crypto++
    std::string hash;
    CryptoPP::SHA256 sha256;

    CryptoPP::StringSource ss(guessString, true,
        new CryptoPP::HashFilter(sha256,
            new CryptoPP::HexEncoder(
                new CryptoPP::StringSink(hash),
                false // Set to false to get lowercase hex digits
            )
        )
    );

    // Take the last three characters of the hash
    std::string proofTest = hash.substr(hash.size() - 3);

    // Return true if the last three characters of the hash are "02"
    return proofTest == "02";
}

std::vector<Block> Blockchain::getChain(){
    return this->chain;
}

std::vector<Transaction> Blockchain::getCurrentTransactions(){
    return this->currentTransactions;
}

Block Blockchain::newBlock(int proof, std::string prevHash){

    Block block(this->getLastBlockIndex() + 1, proof, prevHash, this->getCurrentTransactions());
    this->currentTransactions.clear();
    this->chain.push_back(block);

    return block;
}

int Blockchain::newTransaction(std::string sender, std::string recipient, int amount){
    this->currentTransactions.push_back(Transaction(sender, recipient, amount));

    return this->getLastBlockIndex() + 1;
}

int Blockchain::getLastBlockIndex(){
    return this->chain.size() - 1;
}

int Blockchain::proofOfWork(int lastProof){
    int nonce = 2402;
    int tempNonce = nonce;
    int nonceCounter = 0;
    std::vector<int> usedNonces;

    while(!isProofValid(lastProof, nonce)){

        tempNonce = rand() % 22006;

        if (std::find(usedNonces.begin(), usedNonces.end(), tempNonce) == usedNonces.end()){
            usedNonces.push_back(tempNonce);
            nonce = tempNonce;
            nonceCounter++;
        }

            std::cout << "New nouce: " << nonce << std::endl;
        


    }

    std::cout << "Nonce: " << nonce << " was found after " << nonceCounter << " tries!" << std::endl;

    return nonce;
}

size_t Blockchain::hashBlock(Block block){
    std::hash<std::string> stringHasher;

    size_t blockHash = stringHasher(std::to_string(block.getIndex())+ std::to_string(block.getTimestamp()) + std::to_string(block.getProof()) + block.getPrevHash());

    return blockHash;
}