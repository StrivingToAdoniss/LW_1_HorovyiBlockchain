#include "Application.h"
#include <iostream>
#include <random>
#include <ctime>
#include <iomanip>

Application::Application(int blockAmount)
    : blockchain(0, "Horovyi"), blockAmount(blockAmount) { // cтворюю генезис
    srand(static_cast<unsigned int>(time(0)));
}

void Application::run() {
    std::cout << "Program started!" << std::endl;
    std::cout << "Blockchain created!\n" << std::endl;

    mineBlocks();
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


void Application::mineBlocks() {
    for (int i = 0; i < blockAmount; i++) {
        std::cout << "Started mining block " << i + 1 << "..." << std::endl;

        HorovyiBlockchain::Block lastBlock = blockchain.getChain().back();
        int lastProof = lastBlock.getProof();
        HorovyiBlockchain::ProofOfWorkResult powResult = blockchain.proofOfWork(lastProof);
        std::string prevHash = blockchain.hashBlock(lastBlock);

        blockchain.newTransaction(genRanTransaction());

        HorovyiBlockchain::Block newBlock = blockchain.newBlock(powResult.nonce, prevHash);

        std::cout << "Block " << i + 1 << " created" << std::endl;
        std::cout << "Nonce: " << powResult.nonce << " was found after " << powResult.iterations << " tries!\n" << std::endl;
    }
}
