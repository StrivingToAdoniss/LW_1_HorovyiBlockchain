// Application.hpp

#pragma once

#include "Transaction.hpp"
#include "Blockchain.hpp"
#include "Miner.hpp"
#include "CrowServer.hpp"
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

    bool loadUsersFromFile(const std::string& filename);
    HorovyiBlockchain::Transaction genRanTransaction();

public:
    Application(int blockAmount);
    void run();
};
