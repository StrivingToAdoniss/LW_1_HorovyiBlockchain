#include "Blockchain.h"
#include <iostream>

int main(){

    std:: cout << "Program started!" << std::endl;

    int blockCount = 2;

    Blockchain blockchain(0, "Horovyi");

    std:: cout << "Blockchain created" << std::endl;

    for(int i = 0; i < blockCount; i++){

        std:: cout << "Started mining block " << i + 1 << "..." << std::endl;

        blockchain.newBlock(blockchain.proofOfWork(blockchain.getLastBlockIndex()), std::to_string(blockchain.hashBlock(blockchain.getChain().back())));

        std::cout << "Block " << i + 1 << " created" << std::endl;

        std::cout << "Chain: " << std::endl;

        for(int j = 0; j < blockchain.getChain().size(); j++){
            std::cout << "Block[" << "Index: " << blockchain.getChain()[i].getIndex()<< ", Hash: " << blockchain.hashBlock(blockchain.getChain()[i]) << ", Previous Hash: " << blockchain.getChain()[i].getPrevHash() << "]" << std::endl;
        }

    }

    return 0;
}