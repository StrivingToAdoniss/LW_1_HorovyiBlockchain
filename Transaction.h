#pragma once
#include <string>
#include <vector>

class Transaction{
    private:
        std::string sender;
        std::string recipient;
        int amount;

    public:
        Transaction(std::string sender, std::string recipient, int amount);

        std::string getSender();

        std::string getRecipient();

        int getAmount();

};