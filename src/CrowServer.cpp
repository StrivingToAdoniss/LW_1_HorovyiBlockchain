// CrowServer.cpp

#include "CrowServer.hpp"
#include <mutex>
#include <filesystem>  // C++17 feature for managing paths

CrowServer::CrowServer(HorovyiBlockchain::Blockchain& blockchain, Miner& miner, RandomTransactionGenerator& transactionGenerator)
    : blockchain_(blockchain), miner_(miner), transactionGenerator_(transactionGenerator) {}

void CrowServer::run() {
    crow::SimpleApp app;
    setupRoutes(app);

    // Get the absolute path of the Swagger UI folder
    std::string swagger_ui_path = std::filesystem::absolute("swagger-ui").string();
    std::string swagger_json_path = std::filesystem::absolute("swagger.json").string();

    // Serve Swagger UI static files using the absolute path
    CROW_ROUTE(app, "/swagger/<path>")
    ([swagger_ui_path](const crow::request&, crow::response& res, std::string path) {
        std::ifstream in(swagger_ui_path + "/" + path, std::ifstream::in);
        if (!in) {
            res.code = 404;
            res.end("File not found");
            return;
        }
        std::ostringstream contents;
        contents << in.rdbuf();
        res.write(contents.str());
        res.end();
    });

    // Serve the OpenAPI (Swagger) specification as JSON
    CROW_ROUTE(app, "/swagger.json")
    ([swagger_json_path](const crow::request&, crow::response& res) {
        std::ifstream in(swagger_json_path, std::ifstream::in);
        if (!in) {
            res.code = 404;
            res.end("File not found");
            return;
        }
        std::ostringstream contents;
        contents << in.rdbuf();
        res.write(contents.str());
        res.end();
    });

    // Start the app on port 8080
    app.port(8080).multithreaded().run();
}

void CrowServer::setupRoutes(crow::SimpleApp& app) {
    std::mutex blockchain_mutex;

    CROW_ROUTE(app, "/").methods(crow::HTTPMethod::GET)([&]() {
        return crow::response(200, "Welcome to HorovyiBlockchain API!");
    });

    // Endpoint to create a new transaction
    CROW_ROUTE(app, "/transactions/new").methods(crow::HTTPMethod::POST)([&](const crow::request& req) {
        std::lock_guard<std::mutex> lock(blockchain_mutex);

        auto json_body = nlohmann::json::parse(req.body, nullptr, false);
        if (json_body.is_discarded()) {
            return crow::response(400, "Invalid JSON");
        }

        if (!json_body.contains("sender") || !json_body.contains("recipient") || !json_body.contains("amount")) {
            return crow::response(400, "Missing transaction details");
        }

        HorovyiBlockchain::Transaction new_tx(json_body["sender"], json_body["recipient"], json_body["amount"]);
        int block_index = blockchain_.newTransaction(new_tx);

        nlohmann::json response_json = {{"message", "Transaction will be added to Block " + std::to_string(block_index)}};
        return crow::response(201, response_json.dump());
    });

    // Endpoint to mine a new block
    CROW_ROUTE(app, "/mine").methods(crow::HTTPMethod::GET)([&]() {
        std::lock_guard<std::mutex> lock(blockchain_mutex);

        std::string coinbaseRecipient = "node-123";  // Static node identifier for now
        int coinbaseAmount = 1;  // Reward for mining

        // Generate transactions with a Coinbase transaction at the beginning
        auto transactions = transactionGenerator_.generateTransactionsWithCoinbase(coinbaseRecipient, coinbaseAmount);

        HorovyiBlockchain::Block new_block;
        if (miner_.mineBlock(blockchain_.getChainSize(), new_block, transactions)) {
            nlohmann::json response_json = {
                {"message", "New Block Forged"},
                {"index", new_block.getIndex()},
                {"transactions", nlohmann::json::array()},
                {"proof", new_block.getProof()},
                {"previous_hash", new_block.getPrevHash()}
            };

            for (const auto& tx : new_block.getTransactions()) {
                response_json["transactions"].push_back({
                    {"sender", tx.getSender()},
                    {"recipient", tx.getRecipient()},
                    {"amount", tx.getAmount()}
                });
            }

            return crow::response(200, response_json.dump());
        }

        return crow::response(500, "Mining failed");
    });
    // Endpoint to retrieve the full blockchain
    CROW_ROUTE(app, "/chain").methods(crow::HTTPMethod::GET)([&]() {
        std::lock_guard<std::mutex> lock(blockchain_mutex);

        nlohmann::json response_json;
        response_json["chain"] = nlohmann::json::array();

        for (const auto& block : blockchain_.getChain()) {
            nlohmann::json block_json = {
                {"index", block.getIndex()},
                {"timestamp", block.getTimestamp()},
                {"proof", block.getProof()},
                {"previous_hash", block.getPrevHash()},
                {"transactions", nlohmann::json::array()}
            };

            for (const auto& tx : block.getTransactions()) {
                block_json["transactions"].push_back({
                    {"sender", tx.getSender()},
                    {"recipient", tx.getRecipient()},
                    {"amount", tx.getAmount()}
                });
            }

            response_json["chain"].push_back(block_json);
        }

        response_json["length"] = blockchain_.getChainSize();
        return crow::response(200, response_json.dump());
    });
}
