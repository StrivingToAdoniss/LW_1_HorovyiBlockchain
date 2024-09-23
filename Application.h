#pragma once

#include "Blockchain.h"
#include "Transaction.h"
#include "Block.h"
#include <vector>
#include <string>

class Application {
private:
    HorovyiBlockchain::Blockchain blockchain;
    int blockAmount;
    std::vector<std::string> users;


    bool isChainEmpty() const;
    void initializeMining(int blockIndex, std::string& prevHash, HorovyiBlockchain::Transaction& newTx);
    void mineBlocks(int blockAmount);
    bool performProofOfWork(int blockIndex, const std::string& prevHash, HorovyiBlockchain::Transaction& newTx, HorovyiBlockchain::Block& minedBlock, int& nonce, int& nonceCounter, int maxNonce);

    HorovyiBlockchain::Transaction genRanTransaction();
    void printTransactions(const std::vector<HorovyiBlockchain::Transaction>& transactions);
    void printMiningDetails(int blockIndex, int nonce, int nonceCounter);
    void printChain();
    void drawLine();
    bool loadUsersFromFile(const std::string& filename);

public:
    Application(int blockAmount);

    void run();
};

