// Blockchain.cpp

#include "Blockchain.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <algorithm>
#include "sha.h"
#include "hex.h"
#include "filters.h"
#include <random>

namespace HorovyiBlockchain {

    // Constructor: Initializes the blockchain with a genesis block
    Blockchain::Blockchain(size_t hash, const std::string& prevHash) {
        // Create the genesis block
        Block genesisBlock(0, static_cast<int>(hash), prevHash, this->currentTransactions);
        this->currentTransactions.clear();
        addBlock(genesisBlock);
    }

    // Method to add a block to the chain
    void Blockchain::addBlock(const Block& block) {
        this->chain.push_back(block);
    }

    // Method to check if a block's proof is valid
    IsProofValidResult Blockchain::isProofValid(const Block& block) const {
        // Compute the hash of the block
        std::string hash = hashBlock(block);

        // Extract the last two characters of the hash
        std::string proofTest = hash.substr(hash.size() - 2);

        // Check if the proof meets the required condition
        bool isValid = (proofTest == "02");

        // Prepare the result
        IsProofValidResult result;
        result.isValid = isValid;
        result.proofTest = hash;

        return result;
    }

    // Method to add a new transaction
    int Blockchain::newTransaction(const Transaction& transaction) {
        this->currentTransactions.push_back(transaction);
        return this->getLastBlockIndex() + 1;
    }

    void Blockchain::clearTransactions() {
        this->currentTransactions.clear();
    }

    // Getter for the chain
    const std::vector<Block>& Blockchain::getChain() const {
        return this->chain;
    }

    // Getter for current transactions
    const std::vector<Transaction>& Blockchain::getCurrentTransactions() const {
        return this->currentTransactions;
    }

    // Method to get the last block's index
    int Blockchain::getLastBlockIndex() const {
        if (this->chain.empty()) {
            return -1; // Indicates that the chain is empty
        }
        return static_cast<int>(this->chain.size()) - 1;
    }

    // Method to get the size of the chain
    int Blockchain::getChainSize() const {
        return static_cast<int>(this->chain.size());
    }

    // Method to hash a block
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

        // Compute SHA-256 hash
        std::string hash = computeSHA256(blockData);

        return hash;
    }

    // Helper method to compute SHA-256 hash
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
