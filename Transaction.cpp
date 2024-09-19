#include "Transaction.h"

namespace HorovyiBlockchain {

    Transaction::Transaction(std::string sender, std::string recipient, int amount) {
        this->sender = sender;
        this->recipient = recipient;
        this->amount = amount;
    }

    std::string Transaction::getSender() const {
        return this->sender;
    }

    std::string Transaction::getRecipient() const {
        return this->recipient;
    }

    int Transaction::getAmount() const {
        return this->amount;
    }


    std::ostream& operator<<(std::ostream& os, const Transaction& transaction) {
        os << "Sender: " << transaction.getSender()
            << ", Recipient: " << transaction.getRecipient()
            << ", Amount: " << transaction.getAmount();
        return os;
    }
}