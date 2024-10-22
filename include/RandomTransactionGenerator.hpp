// RandomTransactionGenerator.hpp

#pragma once

#include <vector>
#include <string>
#include <random>
#include "Transaction.hpp"

class RandomTransactionGenerator {
public:
    RandomTransactionGenerator(const std::vector<std::string>& users);

    HorovyiBlockchain::Transaction generateRandomTransaction();
    std::vector<HorovyiBlockchain::Transaction> generateTransactionsWithCoinbase(const std::string& coinbaseRecipient, int coinbaseAmount);

private:
    const std::vector<std::string>& users;
    std::mt19937 gen;  // Random number generator
};
