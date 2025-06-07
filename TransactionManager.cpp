#include "TransactionManager.h"

#include <jdbc/cppconn/prepared_statement.h>
#include <iostream>

void logTransaction(std::shared_ptr<sql::Connection> conn, const Transaction& transaction){
    try {
        std::unique_ptr<sql::PreparedStatement> pstmt(
            conn->prepareStatement("INSERT INTO Transactions (item_id, quantity, price, type, timestamp) VALUES (?, ?, ?, ?, NOW())")
        );

        pstmt->setInt(1, transaction.itemId);
        pstmt->setInt(2, transaction.quantity);
        pstmt->setDouble(3, transaction.price);
        pstmt->setString(4, transaction.type);
        pstmt->executeUpdate();

    } catch (const sql::SQLException &e) {
        std::cerr << "SQL error during transaction logging: " << e.what() << std::endl;
    }
}