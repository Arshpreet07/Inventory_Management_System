#ifndef TRANSACTION_MANAGER_H
#define TRANSACTION_MANAGER_H

#include "Transaction.h"
#include <memory>
#include "DatabaseConnection.h"

void logTransaction(std::shared_ptr<sql::Connection> conn, const Transaction& transaction);

#endif