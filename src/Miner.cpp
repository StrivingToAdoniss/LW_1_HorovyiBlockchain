// Miner.cpp

#include "Miner.hpp"
#include <iostream>
#include <random>

Miner::Miner(HorovyiBlockchain::Blockchain& blockchain, int maxNonce, int startingNonce, const std::string& genesisPrevCash)
    : blockchain(blockchain), maxNonce(maxNonce), startingNonce(startingNonce), genesisPrevCash(genesisPrevCash) {}

bool Miner::mineBlock(int blockIndex, HorovyiBlockchain::Block& minedBlock, const std::vector<HorovyiBlockchain::Transaction>& transactions) {
    std::cout << "Started mining block " << blockIndex + 1 << "..." << std::endl;

    int nonce = 0;
    int nonceCounter = 0;
    bool success = true;
    std::string prevHash;
    HorovyiBlockchain::Transaction newTx;

    initializeMining(blockIndex, prevHash, transactions);
    success = performProofOfWork(blockIndex, prevHash, minedBlock, nonce, nonceCounter);

    if (!success) {
        std::cout << "Failed to mine block " << blockIndex + 1 << " within " << maxNonce << " attempts." << std::endl;
    }

    return success;
}

void Miner::initializeMining(int blockIndex, std::string& prevHash, std::vector<HorovyiBlockchain::Transaction> transactions) {
    if (isChainEmpty()) {
        prevHash = this->genesisPrevCash;
    } else {
        const HorovyiBlockchain::Block& lastBlock = blockchain.getChain().back();
        prevHash = blockchain.hashBlock(lastBlock);
    }

    for (int i = 0; i < transactions.size(); i++) {
        HorovyiBlockchain::Transaction newTx = transactions[i];
        blockchain.newTransaction(newTx);
    }
}

bool Miner::performProofOfWork(int blockIndex, const std::string& prevHash, HorovyiBlockchain::Block& minedBlock, int& nonce, int& nonceCounter) {
    nonceCounter = 0;
    nonce = this->startingNonce;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, maxNonce);

    bool found = false;

    while (!found && nonceCounter < maxNonce) {
        nonce = dis(gen);
        nonceCounter++;

        HorovyiBlockchain::Block candidateBlock(blockIndex, nonce, prevHash, blockchain.getCurrentTransactions());

        HorovyiBlockchain::IsProofValidResult validationResult = blockchain.isProofValid(candidateBlock);

        if (validationResult.isValid) {
            blockchain.addBlock(candidateBlock);
            blockchain.clearTransactions();
            minedBlock = candidateBlock;
            found = true;
        }
    }

    return found;
}

bool Miner::isChainEmpty() const {
    return blockchain.getChain().empty();
}