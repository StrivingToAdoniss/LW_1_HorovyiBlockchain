#pragma once

#include "Blockchain.hpp"
#include "Transaction.hpp"
#include "Block.hpp"
#include <vector>
#include <string>

class Application {
private:
    HorovyiBlockchain::Blockchain blockchain;
    int blockAmount;
    std::vector<std::string> users;

    const int startingNonce = 2402;
    const int maxNonce = 22005;
    const std::string genesisPrevCash = "Horovyi";


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

    void waitForUserInput();

public:
    Application(int blockAmount);

    void run();
};

