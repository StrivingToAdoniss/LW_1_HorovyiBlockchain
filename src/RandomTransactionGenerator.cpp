// RandomTransactionGenerator.cpp

#include "RandomTransactionGenerator.hpp"
#include <iostream>
#include <random>

RandomTransactionGenerator::RandomTransactionGenerator(const std::vector<std::string>& users)
    : users(users), gen(std::random_device{}()) {
    if (users.empty()) {
        std::cerr << "Error: User list is empty. Cannot generate transactions." << std::endl;
        exit(EXIT_FAILURE);
    }
}

HorovyiBlockchain::Transaction RandomTransactionGenerator::generateRandomTransaction() {
    std::uniform_int_distribution<> userDist(0, users.size() - 1);
    std::uniform_int_distribution<> amountDist(1, 1000);

    int senderIndex = userDist(gen);

    int recipientIndex;
    do {
        recipientIndex = userDist(gen);
    } while (recipientIndex == senderIndex);

    int moneyAmount = amountDist(gen);

    return HorovyiBlockchain::Transaction(users[senderIndex], users[recipientIndex], moneyAmount);
}

std::vector<HorovyiBlockchain::Transaction> RandomTransactionGenerator::generateTransactionsWithCoinbase(const std::string& coinbaseRecipient, int coinbaseAmount) {
    std::vector<HorovyiBlockchain::Transaction> transactions;

    // Add Coinbase transaction at the beginning
    transactions.emplace_back("0", coinbaseRecipient, coinbaseAmount);

    // Generate additional random transactions
    int numRandomTransactions = 5;
    for (int i = 0; i < numRandomTransactions; i++) {
        transactions.push_back(generateRandomTransaction());
    }

    return transactions;
}
