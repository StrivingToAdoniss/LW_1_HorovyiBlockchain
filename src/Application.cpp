#include "Application.hpp"
#include <iostream>
#include <random>
#include <ctime>
#include <iomanip>
#include <fstream>

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
    std::cout << "Blockchain created!\n" << std::endl;

    mineBlocks(blockAmount);
    printChain();

    waitForUserInput();
}

void Application::waitForUserInput() {
    std::cout << "Press Enter to exit...";
    std::string dummy;
    std::getline(std::cin, dummy);
}

void Application::mineBlocks(int blockAmount) {
    for (int i = 0; i < blockAmount; i++) {
        std::cout << "Started mining block " << i + 1 << "..." << std::endl;



        HorovyiBlockchain::Block minedBlock;
        int nonce = 0;
        int nonceCounter = 0;
        int maxNonce = this->maxNonce;
        bool success = true;

        std::string prevHash;
        HorovyiBlockchain::Transaction newTx;
        initializeMining(i, prevHash, newTx);
        success = performProofOfWork(i, prevHash, newTx, minedBlock, nonce, nonceCounter, maxNonce);


        if (!success) {
            std::cout << "Failed to mine block " << i + 1 << " within " << maxNonce << " attempts." << std::endl;
        }
    }
}

void Application::initializeMining(int blockIndex, std::string& prevHash, HorovyiBlockchain::Transaction& newTx) {
    
    if (isChainEmpty()) {
        prevHash = this->genesisPrevCash;
    }
    else {
        const HorovyiBlockchain::Block& lastBlock = blockchain.getChain().back();
        prevHash = blockchain.hashBlock(lastBlock);
    }
    
    int transactionCount = 5;

    for (int i = 0; i < transactionCount; i++) {
        newTx = genRanTransaction();
        blockchain.newTransaction(newTx);
    }

}

bool Application::performProofOfWork(int blockIndex, const std::string& prevHash, HorovyiBlockchain::Transaction& newTx, HorovyiBlockchain::Block& minedBlock, int& nonce, int& nonceCounter, int maxNonce) {
    nonceCounter = 0;
    nonce = this->startingNonce;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, maxNonce);

    bool found = false;

    while (!found && nonceCounter < maxNonce) {
        nonce = dis(gen);
        nonceCounter++;

        HorovyiBlockchain::Block candidateBlock(blockIndex, nonce, prevHash, blockchain.getCurrentTransactions());

        HorovyiBlockchain::IsProofValidResult validationResult = blockchain.isProofValid(candidateBlock);

        if (validationResult.isValid) {
            blockchain.addBlock(candidateBlock);

            blockchain.clearTransactions();

            minedBlock = candidateBlock;

            printMiningDetails(blockIndex, nonce, nonceCounter);

            found = true;
        }
    }

    return found;
}

void Application::printMiningDetails(int blockIndex, int nonce, int nonceCounter) {
    std::cout << "Block " << blockIndex + 1 << " created" << std::endl;
    std::cout << "Nonce: " << nonce << " was found after " << nonceCounter << " tries!\n" << std::endl;
}

void Application::drawLine() {
    int lineLength = 50;
    for (int i = 0; i < lineLength; ++i) {
        std::cout << "-";
    }
    std::cout << std::endl;
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


void Application::printTransactions(const std::vector<HorovyiBlockchain::Transaction>& transactions) {
    for (size_t i = 0; i < transactions.size(); i++) {
        if (i != 0) {
            std::cout << "\n";
        }
        std::cout << i + 1 << ". " << transactions[i];
    }
}

void Application::printChain() {
    std::string genesisMark = "(+GENESIS+)";

    drawLine();

    std::cout << "Chain: \n" << std::endl;

    int blockCount = blockchain.getChainSize();

    for (int j = 0; j < blockCount; j++) {

        std::string blockLine = "Block";
        std::string blockNum = std::to_string(j + 1);

        if (j == 0) {
            blockLine = blockLine + " " + blockNum + " " + genesisMark;
        }
        else {
            blockLine = blockLine + " " + blockNum;
        }

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

        std::cout << blockLine 
            << "\nIndex: " << blockIndex
            << "\nTransactions:\n";
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
    return this->blockchain.getChain().empty();
}

bool Application::loadUsersFromFile(const std::string& filename) {
    std::ifstream infile(filename);
    if (!infile) {
        std::cerr << "Error: Unable to open users file: " << filename << std::endl;
        return false;
    }

    std::string line;
    while (std::getline(infile, line)) {
        // Trim whitespace from both ends
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

    if (users.empty()) {
        std::cerr << "Error: No users loaded from file: " << filename << std::endl;
        return false;
    }

    return true;
}