// CrowServer.hpp

#pragma once

#include <vector>
#include <crow.h>
#include <nlohmann/json.hpp>
#include "Blockchain.hpp"
#include "Miner.hpp"
#include "RandomTransactionGenerator.hpp"

class CrowServer {
private:
    HorovyiBlockchain::Blockchain& blockchain_;
    Miner& miner_;
    void setupRoutes(crow::SimpleApp& app);
    RandomTransactionGenerator& transactionGenerator_;

public:
    CrowServer(HorovyiBlockchain::Blockchain& blockchain, Miner& miner, RandomTransactionGenerator& transactionGenerator);
    void run();
};
