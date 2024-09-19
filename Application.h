#pragma once

#include "Blockchain.h"
#include "Transaction.h"
#include <vector>
#include <string>

class Application {
public:
    Application(int blockAmount);
    void run();

private:
    HorovyiBlockchain::Blockchain blockchain;
    int blockAmount;

    void drawLine();
    HorovyiBlockchain::Transaction genRanTransaction();
    void printTransactions(const std::vector<HorovyiBlockchain::Transaction>& transactions);
    void printChain();
    void mineBlocks();
};

