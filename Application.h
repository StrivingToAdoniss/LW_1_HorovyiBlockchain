#pragma once

#include "Blockchain.h"
#include "Transaction.h"
#include "Block.h"
#include <vector>
#include <string>

class Application {
public:
    // Constructor
    Application(int blockAmount);

    // Method to run the application
    void run();

private:
    // Helper methods for mining
    bool isChainEmpty() const;
    void initializeMining(int blockIndex, std::string& prevHash, HorovyiBlockchain::Transaction& newTx);
    bool performProofOfWork(int blockIndex, const std::string& prevHash, HorovyiBlockchain::Transaction& newTx, HorovyiBlockchain::Block& minedBlock, int& nonce, int& nonceCounter, int maxNonce);


    // Other existing methods
    void mineBlocks(int blockAmount);
    HorovyiBlockchain::Transaction genRanTransaction();
    void printTransactions(const std::vector<HorovyiBlockchain::Transaction>& transactions);
    void printMiningDetails(int blockIndex, int nonce, int nonceCounter);
    void printChain();
    void drawLine();

    HorovyiBlockchain::Blockchain blockchain;
    int blockAmount;
};

