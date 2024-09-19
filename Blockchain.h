#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include "Transaction.h"
#include "Block.h"

class Blockchain{
    private:
        std::vector<Block> chain;
        std::vector<Transaction> currentTransactions;

        bool isProofValid(int lastProof, int proof);

    public:
        Blockchain(size_t hash, std::string prevHash);

        std::vector<Block> getChain();

        std::vector<Transaction> getCurrentTransactions();

        Block newBlock(int proof, std::string prevHash);

        int newTransaction(std::string sender, std::string recipient, int amount);

        int getLastBlockIndex();

        int proofOfWork(int lastProof);

        int getChainSize();

        std::string hashBlock(Block block);



};