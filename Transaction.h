#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>

struct Transaction{
    int itemId;
    int quantity;
    double price;
    std::string type;
};

#endif