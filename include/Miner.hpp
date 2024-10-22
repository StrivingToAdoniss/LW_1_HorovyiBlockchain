// Miner.hpp

#pragma once

#include "Blockchain.hpp"
#include <string>

class Miner {
private:
    HorovyiBlockchain::Blockchain& blockchain;
    int maxNonce;
    int startingNonce;
    std::string genesisPrevCash;

    void initializeMining(int blockIndex, std::string& prevHash, std::vector<HorovyiBlockchain::Transaction> transactions);
    bool performProofOfWork(int blockIndex, const std::string& prevHash, HorovyiBlockchain::Block& minedBlock, int& nonce, int& nonceCounter);

    bool isChainEmpty() const; 

public:
    Miner(HorovyiBlockchain::Blockchain& blockchain, int maxNonce, int startingNonce, const std::string& genesisPrevCash);

    bool mineBlock(int blockIndex, HorovyiBlockchain::Block& minedBlock, const std::vector<HorovyiBlockchain::Transaction>& transactions);

};
