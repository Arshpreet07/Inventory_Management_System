#ifndef DATABASE_CONNECTION_H
#define DATABASE_CONNECTION_H

#include <jdbc/mysql_driver.h>
#include <jdbc/cppconn/connection.h>
#include <jdbc/cppconn/exception.h>
#include <jdbc/cppconn/resultset.h>
#include <jdbc/cppconn/statement.h>
#include <memory>
#include <mutex>

class DatabaseConnection {
public:
    static std::shared_ptr<sql::Connection> getConnection();

private:
    static std::shared_ptr<sql::Connection> connection;
    static std::mutex connectionMutex;

    // Disallow creation outside the class
    DatabaseConnection() = default;
};

#endif
