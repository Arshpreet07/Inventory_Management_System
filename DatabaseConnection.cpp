#include "DatabaseConnection.h"

std::shared_ptr<sql::Connection> DatabaseConnection::connection = nullptr;
std::mutex DatabaseConnection::connectionMutex;

std::shared_ptr<sql::Connection> DatabaseConnection::getConnection() {
    std::lock_guard<std::mutex> lock(connectionMutex);

    if (!connection || connection->isClosed()) {
        try {
            sql::mysql::MySQL_Driver* driver = sql::mysql::get_mysql_driver_instance();
            std::shared_ptr<sql::Connection> conn(
                driver->connect("tcp://127.0.0.1:3306", "root", "12345678"));
            conn->setSchema("inventory");
            connection = conn;
        } catch (const sql::SQLException& e) {
            std::cerr << "MySQL connection error: " << e.what() << std::endl;
            throw; // rethrow if you want to stop execution on failure
        }
    }

    return connection;
}
