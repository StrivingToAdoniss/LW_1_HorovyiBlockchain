#include "Blockchain.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <sstream>
#include "sha.h"
#include "hex.h"
#include "filters.h"
#include <random>

namespace HorovyiBlockchain {

    Blockchain::Blockchain(size_t hash, std::string prevHash) {
        newBlock(hash, prevHash);
    }

    IsProofValidResult Blockchain::isProofValid(int lastProof, int nonce) const {

        std::string guessString = std::to_string(lastProof) + std::to_string(nonce);

        std::string hash = computeSHA256(guessString);

        // Take the last two characters of the hash
        std::string proofTest = hash.substr(hash.size() - 2);

        IsProofValidResult result;
        result.isValid = proofTest == "02";
        result.proofTest = hash;

        return result;

    }

    const std::vector<Block>& Blockchain::getChain() const {
        return this->chain;
    }

    const std::vector<Transaction>& Blockchain::getCurrentTransactions() const {
        return this->currentTransactions;
    }

    Block Blockchain::newBlock(int proof, const std::string& prevHash) {
        Block block(this->getLastBlockIndex() + 1, proof, prevHash, this->currentTransactions);
        this->currentTransactions.clear();
        this->chain.push_back(block);

        return block;
    }

    int Blockchain::newTransaction(const Transaction& transaction) {
        this->currentTransactions.push_back(transaction);

        return this->getLastBlockIndex() + 1;
    }

    int Blockchain::getLastBlockIndex() const {
        if (this->chain.empty()) {
            return -1;
        }
        return static_cast<int>(this->chain.size()) - 1;
    }

    ProofOfWorkResult Blockchain::proofOfWork(int lastProof) {
        int nonceCounter = 0;
        int nonce = 2402;
        int maxNonce = 22005;
        IsProofValidResult validationResult;

        std::random_device rd;  // Seed generator
        std::mt19937 gen(rd()); // Mersenne Twister engine
        std::uniform_int_distribution<> dis(0, maxNonce); // Uniform distribution over the range of int

        validationResult = isProofValid(lastProof, nonce);
        nonceCounter++;

        while (!validationResult.isValid) {
            nonce = dis(gen); // Generate a new nonce
            validationResult = isProofValid(lastProof, nonce);
            nonceCounter++;
        }

        ProofOfWorkResult result;
        result.nonce = nonce;
        result.iterations = nonceCounter;
        result.proofTest = validationResult.proofTest;

        return result;
    }


    int Blockchain::getChainSize() const {
        return static_cast<int>(this->chain.size());
    }

    std::string Blockchain::hashBlock(const Block& block) const {
        // Serialize the block data
        std::string blockData = std::to_string(block.getIndex()) +
            std::to_string(block.getTimestamp()) +
            std::to_string(block.getProof()) +
            block.getPrevHash();

        // Serialize transactions
        for (const Transaction& transaction : block.getTransactions()) {
            blockData += transaction.getSender() +
                transaction.getRecipient() +
                std::to_string(transaction.getAmount());
        }

        std::string hash = computeSHA256(blockData);

        return hash;
    }

    std::string Blockchain::computeSHA256(const std::string& data) const {
        std::string hash;
        CryptoPP::SHA256 sha256;

        CryptoPP::StringSource ss(data, true,
            new CryptoPP::HashFilter(sha256,
                new CryptoPP::HexEncoder(
                    new CryptoPP::StringSink(hash),
                    false // Use lowercase hex digits
                )
            )
        );

        return hash;
    }
}