#pragma once
#include <string>
#include <vector>
#include <iostream>

namespace HorovyiBlockchain {

class Transaction{
    private:
        std::string sender;
        std::string recipient;
        int amount;

    public:
        Transaction(std::string sender, std::string recipient, int amount);

        Transaction();

        std::string getSender() const;

        std::string getRecipient() const;

        int getAmount() const;

};

std::ostream& operator<<(std::ostream& os, const Transaction& transaction);

}