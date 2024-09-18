#include "Transaction.h"


Transaction::Transaction(std::string sender, std::string recipient, int amount){
    this->sender = sender;
    this->recipient = recipient;
    this->amount = amount;
}

std::string Transaction::getSender(){
    return this->sender;
}

std::string Transaction::getRecipient(){
    return this->recipient;
}

int Transaction::getAmount(){
    return this->amount;
}