#include "Application.h"
#include <iostream>
#include <random>
#include <ctime>
#include <iomanip>

Application::Application(int blockAmount)
    : blockchain(0, "Horovyi"), blockAmount(blockAmount) {
    srand(static_cast<unsigned int>(time(0)));
}

void Application::run() {
    std::cout << "Program started!" << std::endl;
    std::cout << "Blockchain created!\n" << std::endl;

    mineBlocks(blockAmount);
    printChain();
}

void Application::drawLine() {
    int lineLength = 50;
    for (int i = 0; i < lineLength; ++i) {
        std::cout << "-";
    }
    std::cout << std::endl;
}

HorovyiBlockchain::Transaction Application::genRanTransaction() {
    std::vector<std::string> users = { "Charlie", "Rokovyi", "Oleh", "Hlib", "UrMum", "Kuzma", "Olha" };

    static std::random_device rd;
    static std::mt19937 gen(rd());
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

void Application::printTransactions(const std::vector<HorovyiBlockchain::Transaction>& transactions) {
    for (size_t i = 0; i < transactions.size(); i++) {
        if (i != 0) {
            std::cout << "; ";
        }
        std::cout << transactions[i];
    }
}

void Application::printChain() {
    drawLine();

    std::cout << "Chain: \n" << std::endl;

    int blockCount = blockchain.getChainSize();

    for (int j = 0; j < blockCount; j++) {
        HorovyiBlockchain::Block currentBlock = blockchain.getChain()[j];

        int blockIndex = currentBlock.getIndex();
        std::string blockHash = blockchain.hashBlock(currentBlock);
        std::string prevBlockHash = currentBlock.getPrevHash();
        std::time_t timestamp = currentBlock.getTimestamp();
        std::vector<HorovyiBlockchain::Transaction> blockTransactions = currentBlock.getTransactions();

        char timeBuffer[100];
        std::tm timeInfo;
        localtime_s(&timeInfo, &timestamp); // Safe conversion

        // Format the time to "dd.mm.yy HH:MM:SS"
        std::strftime(timeBuffer, sizeof(timeBuffer), "%d.%m.%y %H:%M:%S", &timeInfo);

        std::cout << "Block " << j + 1
            << "\nIndex: " << blockIndex
            << "\nTransactions: ";
        printTransactions(blockTransactions);
        std::cout << "\nHash: " << blockHash
            << "\nPrevious Hash: " << prevBlockHash
            << "\nTimestamp: " << timeBuffer
            << std::endl;

        if (j != blockCount - 1) {
            std::cout << "\t|"
                << "\n\t|"
                << "\n\t|\n";
        }
    }

    drawLine();
}


bool Application::isChainEmpty() const {
    return blockchain.getChain().empty();
}

void Application::initializeMining(int blockIndex, std::string& prevHash, HorovyiBlockchain::Transaction& newTx) {
    // Retrieve the last block
    const HorovyiBlockchain::Block& lastBlock = blockchain.getChain().back();
    prevHash = blockchain.hashBlock(lastBlock);

    // Generate a new transaction
    newTx = genRanTransaction();
    blockchain.newTransaction(newTx);
}

bool Application::performProofOfWork(int blockIndex, const std::string& prevHash, HorovyiBlockchain::Transaction& newTx, HorovyiBlockchain::Block& minedBlock, int& nonce, int& nonceCounter, int maxNonce) {
    // Initialize proof-of-work variables
    nonceCounter = 0;
    nonce = 2402; // Starting nonce value

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, maxNonce);

    bool found = false;

    while (!found && nonceCounter < maxNonce) {
        // Generate a new nonce
        nonce = dis(gen);
        nonceCounter++;

        // Create a new block with the current nonce
        HorovyiBlockchain::Block candidateBlock(blockIndex, nonce, prevHash, blockchain.getCurrentTransactions());

        // Check if the proof is valid
        HorovyiBlockchain::IsProofValidResult validationResult = blockchain.isProofValid(candidateBlock);

        if (validationResult.isValid) {
            // Add the valid block to the chain
            blockchain.addBlock(candidateBlock);

            // Clear current transactions as they are now included in the block
            blockchain.clearTransactions();

            minedBlock = candidateBlock;

            // Output mining details
            printMiningDetails(blockIndex, nonce, nonceCounter);

            found = true;
        }
    }

    return found;
}

void Application::printMiningDetails(int blockIndex, int nonce, int nonceCounter) {
    std::cout << "Block " << blockIndex << " created" << std::endl;
    std::cout << "Nonce: " << nonce << " was found after " << nonceCounter << " tries!\n" << std::endl;
}

void Application::mineBlocks(int blockAmount) {
    for (int i = 0; i < blockAmount; i++) {
        std::cout << "Started mining block " << i + 1 << "..." << std::endl;

        if (isChainEmpty()) {
            std::cout << "Error: Blockchain is empty. Cannot mine new block." << std::endl;
            continue;
        }

        std::string prevHash;
        HorovyiBlockchain::Transaction newTx;
        initializeMining(i, prevHash, newTx);

        HorovyiBlockchain::Block minedBlock;
        int nonce = 0;
        int nonceCounter = 0;
        int maxNonce = 22005;
        bool success = performProofOfWork(i + 1, prevHash, newTx, minedBlock, nonce, nonceCounter, maxNonce);

        if (!success) {
            std::cout << "Failed to mine block " << i + 1 << " within " << maxNonce << " attempts." << std::endl;
        }
    }
}