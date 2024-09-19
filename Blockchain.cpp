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

namespace HorovyiBlockchain {

    Blockchain::Blockchain(size_t hash, std::string prevHash) {
        newBlock(hash, prevHash);
    }

    bool Blockchain::isProofValid(int lastProof, int proof) const {
        // Concatenate lastProof and proof into a string
        std::string guessString = std::to_string(lastProof) + std::to_string(proof);

        // Compute SHA-256 hash of guessString using Crypto++
        std::string hash;
        CryptoPP::SHA256 sha256;

        CryptoPP::StringSource ss(guessString, true,
            new CryptoPP::HashFilter(sha256,
                new CryptoPP::HexEncoder(
                    new CryptoPP::StringSink(hash),
                    false // Set to false to get lowercase hex digits
                )
            )
        );

        // Take the last two characters of the hash
        std::string proofTest = hash.substr(hash.size() - 2);

        // Return true if the last two characters of the hash are "02"
        return proofTest == "02";
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
        int nonce = 0;
        int nonceCounter = 0;

        while (!isProofValid(lastProof, nonce)) {
            nonce++;
            nonceCounter++;
        }

        ProofOfWorkResult result;
        result.nonce = nonce;
        result.iterations = nonceCounter;

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
        for (const auto& transaction : block.getTransactions()) {
            blockData += transaction.getSender() +
                transaction.getRecipient() +
                std::to_string(transaction.getAmount());
        }

        // Compute SHA-256 hash
        std::string hash;
        CryptoPP::SHA256 sha256;
        CryptoPP::StringSource ss(blockData, true,
            new CryptoPP::HashFilter(sha256,
                new CryptoPP::HexEncoder(
                    new CryptoPP::StringSink(hash),
                    false // Lowercase hex digits
                )
            )
        );

        return hash;
    }
}