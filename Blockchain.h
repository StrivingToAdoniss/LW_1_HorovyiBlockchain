#pragma once
#include <vector>
#include <string>
#include "Block.h"
#include "Transaction.h"

namespace HorovyiBlockchain {

    struct IsProofValidResult {
        bool isValid;
        std::string proofTest;
    };

    class Blockchain {
    private:
        std::vector<Block> chain;
        std::vector<Transaction> currentTransactions;
        std::string computeSHA256(const std::string& data) const;

    public:
        Blockchain(size_t hash, const std::string& prevHash);
        Blockchain();
        IsProofValidResult isProofValid(const Block& block) const;
        const std::vector<Block>& getChain() const;
        const std::vector<Transaction>& getCurrentTransactions() const;
        void addBlock(const Block& block);
        int newTransaction(const Transaction& transaction);
        int getLastBlockIndex() const;
        int getChainSize() const;
        std::string hashBlock(const Block& block) const;
        void clearTransactions();

    };
}