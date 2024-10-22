// Application.cpp

#include "Application.hpp"
#include <iostream>
#include <random>
#include <ctime>

Application::Application(int blockAmount)
    : blockAmount(blockAmount) {
    srand(static_cast<unsigned int>(time(0)));

    if (!loadUsersFromFile("users.txt")) {
        std::cerr << "Failed to load users. Exiting application." << std::endl;
        exit(EXIT_FAILURE); 
    }
}

void Application::run() {
    std::cout << "Program started!" << std::endl;

    RandomTransactionGenerator transactionGenerator(users);

    // Initialize the miner and CrowServer with the current blockchain and settings
    Miner miner(blockchain, maxNonce, startingNonce, genesisPrevCash);
    CrowServer server(blockchain, miner, transactionGenerator);

    // Start the server, which handles transactions and mining via API
    server.run();
}

bool Application::loadUsersFromFile(const std::string& filename) {
    std::ifstream infile(filename);
    if (!infile) {
        std::cerr << "Error: Unable to open users file: " << filename << std::endl;
        return false;
    }

    std::string line;
    while (std::getline(infile, line)) {
        size_t start = line.find_first_not_of(" \t\r\n");
        size_t end = line.find_last_not_of(" \t\r\n");
        if (start != std::string::npos && end != std::string::npos) {
            std::string name = line.substr(start, end - start + 1);
            if (!name.empty()) {
                users.push_back(name);
            }
        }
    }

    infile.close();
    return !users.empty();
}

HorovyiBlockchain::Transaction Application::genRanTransaction() {
    if (users.empty()) {
        std::cerr << "Error: User list is empty. Cannot generate transaction." << std::endl;
        exit(EXIT_FAILURE); // Exit if no users are available
    }

    static std::random_device rd;
    static std::mt19937 gen(rd());

    // Create uniform distributions based on the size of the users vector
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