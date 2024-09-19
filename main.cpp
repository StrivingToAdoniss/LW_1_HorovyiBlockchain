#include "Blockchain.h"
#include <iostream>

void drawLine() {
    int lineLength = 50; // Set the desired length of the line
    for (int i = 0; i < lineLength; ++i) {
        std::cout << "-";
    }
    std::cout << std::endl; // Move to the next line after drawing
}

int main(){

    std:: cout << "Program started!" << std::endl;

    int blockCount = 10;

    Blockchain blockchain(0, "Horovyi");

    std:: cout << "Blockchain created" << std::endl;

    for(int i = 0; i < blockCount; i++){

        std::cout << "Started mining block " << i + 1 << "..." << std::endl;

        Block lastBlock = blockchain.getChain().back();
        int lastProof = lastBlock.getProof();
        int proof = blockchain.proofOfWork(lastProof);
        std::string prevHash = blockchain.hashBlock(lastBlock);

        Block newBlock = blockchain.newBlock(proof, prevHash);

        std::cout << "Block " << i + 1 << " created" << std::endl;

        drawLine();

        std::cout << "Chain: \n" << std::endl;

        int blockCount = blockchain.getChainSize();

        for(int j = 0; j < blockCount; j++){

            Block currentBlock = blockchain.getChain()[j];

            int blockIndex = currentBlock.getIndex();
            std::string blockHash = blockchain.hashBlock(currentBlock);
            std::string prevBlockHash = currentBlock.getPrevHash();
            const std::time_t timestamp = currentBlock.getTimestamp();

            char timeBuffer[100]; // Allocate a larger buffer
            std::tm timeInfo;
            localtime_s(&timeInfo, &timestamp); // Converts time_t to tm struct

            // Format the time to "dd.mm.yy HH:MM:SS"
            std::strftime(timeBuffer, sizeof(timeBuffer), "%d.%m.%y %H:%M:%S", &timeInfo);


            std::cout << "Block " << j + 1
                << "\nIndex: " << blockIndex
                << "\nHash: " << blockHash 
                << "\nPrevious Hash: " << prevBlockHash 
                << "\nTimestapm: " << timeBuffer
                << std::endl;
            if (j != blockCount - 1) {
                std::cout << "\t|"
                    << "\n\t|"
                    << "\n\t|\n";
            }

        }

        drawLine();

    }

    return 0;
}