#ifndef AUDIT_LOGGER_H
#define AUDIT_LOGGER_H

#include <jdbc/mysql_connection.h>
#include <jdbc/cppconn/driver.h>
#include <jdbc/cppconn/prepared_statement.h>
#include <memory>
#include <string>

class AuditLogger {
public:
    AuditLogger(std::shared_ptr<sql::Connection> conn)
        : conn(std::move(conn)) {}

    void logEvent(const std::string& event) {
        try {
            std::unique_ptr<sql::PreparedStatement> pstmt(
                conn->prepareStatement("INSERT INTO AuditLogs (event) VALUES (?)"));
            pstmt->setString(1, event);
            pstmt->executeUpdate();
        } catch (const sql::SQLException& e) {
            std::cerr << "SQL Error during audit logging: " << e.what() << std::endl;
        }
    }

private:
    std::shared_ptr<sql::Connection> conn;
};

#endif
