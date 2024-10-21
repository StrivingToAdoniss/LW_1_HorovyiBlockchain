#include <asio.hpp>
#include <iostream>
#include <string>
#include <memory>
#include <mutex>
#include <thread>

#include "Blockchain.hpp"
#include "Transaction.hpp"
#include "Block.hpp"

#include <nlohmann/json.hpp>

using asio::ip::tcp;
using json = nlohmann::json;

class HttpSession : public std::enable_shared_from_this<HttpSession> {
public:
    HttpSession(tcp::socket socket, HorovyiBlockchain::Blockchain& blockchain, std::mutex& mutex)
        : socket_(std::move(socket)), blockchain_(blockchain), blockchain_mutex_(mutex) {}

    void start() {
        readRequest();
    }

private:
    void readRequest() {
        auto self(shared_from_this());
        asio::async_read_until(socket_, asio::dynamic_buffer(request_), "\r\n\r\n",
            [this, self](std::error_code ec, std::size_t /*length*/) {
                if (!ec) {
                    parseRequest();
                    handleRequest();
                } else {
                    // Handle error
                }
            });
    }

    void parseRequest() {
        std::istringstream request_stream(request_);
        std::string request_line;
        std::getline(request_stream, request_line);
        std::istringstream request_line_stream(request_line);
        request_line_stream >> method_ >> uri_ >> version_;
        // Additional parsing (headers, body) can be implemented as needed
    }

    void handleRequest() {
        if (method_ == "GET" && uri_ == "/mine") {
            handleMine();
        } else if (method_ == "POST" && uri_ == "/transactions/new") {
            readBody();
        } else if (method_ == "GET" && uri_ == "/chain") {
            handleChain();
        } else {
            sendResponse("HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\n\r\n");
        }
    }

    void readBody() {
        // Read the remaining data as the body
        auto self(shared_from_this());
        asio::async_read(socket_, asio::dynamic_buffer(request_),
            [this, self](std::error_code ec, std::size_t /*length*/) {
                if (!ec || ec == asio::error::eof) {
                    handleNewTransaction();
                } else {
                    // Handle error
                }
            });
    }

    void handleNewTransaction() {
        std::string body = request_.substr(request_.find("\r\n\r\n") + 4);
        auto json_body = json::parse(body, nullptr, false);

        if (json_body.is_discarded()) {
            sendResponse("HTTP/1.1 400 Bad Request\r\nContent-Length: 0\r\n\r\n");
            return;
        }

        // Validate required fields
        if (!json_body.contains("sender") || !json_body.contains("recipient") || !json_body.contains("amount")) {
            sendResponse("HTTP/1.1 400 Bad Request\r\nContent-Length: 0\r\n\r\n");
            return;
        }

        // Create a new transaction
        HorovyiBlockchain::Transaction new_tx(
            json_body["sender"].get<std::string>(),
            json_body["recipient"].get<std::string>(),
            json_body["amount"].get<int>()
        );

        // Add the transaction to the blockchain
        {
            std::lock_guard<std::mutex> lock(blockchain_mutex_);
            blockchain_.newTransaction(new_tx);
        }

        // Send response
        json response_json = {{"message", "Transaction will be added to the next block"}};
        std::string response_body = response_json.dump();
        sendResponse("HTTP/1.1 201 Created\r\nContent-Type: application/json\r\nContent-Length: " +
                     std::to_string(response_body.size()) + "\r\n\r\n" + response_body);
    }

    void handleMine() {
        std::string node_identifier = "node-id";  // Replace with actual node identifier

        // Perform proof of work and add a new block
        HorovyiBlockchain::Block new_block;
        {
            std::lock_guard<std::mutex> lock(blockchain_mutex_);
            // Implement your proof of work algorithm
            const auto& last_block = blockchain_.getChain().back();
            int last_proof = last_block.getProof();
            int proof = blockchain_.proofOfWork(last_proof);

            // Reward for mining
            blockchain_.newTransaction(HorovyiBlockchain::Transaction("0", node_identifier, 1));

            // Create new block
            std::string previous_hash = blockchain_.hashBlock(last_block);
            new_block = HorovyiBlockchain::Block(
                blockchain_.getChainSize() + 1,
                proof,
                previous_hash,
                blockchain_.getCurrentTransactions()
            );

            blockchain_.addBlock(new_block);
            blockchain_.clearTransactions();
        }

        // Prepare response
        json response_json = {
            {"message", "New Block Forged"},
            {"index", new_block.getIndex()},
            {"transactions", json::array()},
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

        std::string response_body = response_json.dump();
        sendResponse("HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nContent-Length: " +
                     std::to_string(response_body.size()) + "\r\n\r\n" + response_body);
    }

    void handleChain() {
        json response_json;
        {
            std::lock_guard<std::mutex> lock(blockchain_mutex_);
            response_json["chain"] = json::array();
            for (const auto& block : blockchain_.getChain()) {
                json block_json = {
                    {"index", block.getIndex()},
                    {"timestamp", block.getTimestamp()},
                    {"proof", block.getProof()},
                    {"previous_hash", block.getPrevHash()},
                    {"transactions", json::array()}
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
        }

        std::string response_body = response_json.dump();
        sendResponse("HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nContent-Length: " +
                     std::to_string(response_body.size()) + "\r\n\r\n" + response_body);
    }

    void sendResponse(const std::string& response) {
        auto self(shared_from_this());
        asio::async_write(socket_, asio::buffer(response),
            [this, self](std::error_code /*ec*/, std::size_t /*length*/) {
                // Gracefully close the socket after sending the response
                socket_.shutdown(tcp::socket::shutdown_both);
            });
    }

    tcp::socket socket_;
    HorovyiBlockchain::Blockchain& blockchain_;
    std::mutex& blockchain_mutex_;
    std::string request_;
    std::string method_;
    std::string uri_;
    std::string version_;
};

class HttpServer {
public:
    HttpServer(asio::io_context& io_context, unsigned short port, HorovyiBlockchain::Blockchain& blockchain, std::mutex& mutex)
        : acceptor_(io_context, tcp::endpoint(tcp::v4(), port)), blockchain_(blockchain), blockchain_mutex_(mutex) {
        acceptConnection();
    }

private:
    void acceptConnection() {
        acceptor_.async_accept(
            [this](std::error_code ec, tcp::socket socket) {
                if (!ec) {
                    std::make_shared<HttpSession>(std::move(socket), blockchain_, blockchain_mutex_)->start();
                }
                acceptConnection();
            });
    }

    tcp::acceptor acceptor_;
    HorovyiBlockchain::Blockchain& blockchain_;
    std::mutex& blockchain_mutex_;
};

int main() {
    try {
        asio::io_context io_context;

        HorovyiBlockchain::Blockchain blockchain;
        std::mutex blockchain_mutex;

        HttpServer server(io_context, 5000, blockchain, blockchain_mutex);

        io_context.run();
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
}
