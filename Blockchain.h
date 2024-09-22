#pragma once
#include <vector>
#include <string>
#include "Block.h"
#include "Transaction.h"

namespace HorovyiBlockchain {

    struct ProofOfWorkResult {
        int nonce;
        int iterations;
        std::string proofTest;
    };

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
        Blockchain(size_t hash, std::string prevHash);
        IsProofValidResult isProofValid(int lastProof, int proof) const;
        const std::vector<Block>& getChain() const;
        const std::vector<Transaction>& getCurrentTransactions() const;
        Block newBlock(int proof, const std::string& prevHash);
        int newTransaction(const Transaction& transaction);
        int getLastBlockIndex() const;
        ProofOfWorkResult proofOfWork(int lastProof);
        int getChainSize() const;
        std::string hashBlock(const Block& block) const;

    };
}