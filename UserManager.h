#ifndef USER_MANAGER_H
#define USER_MANAGER_H

#include "User.h"
#include "DatabaseConnection.h"

#include <memory>
#include <string>

class UserManager{
private:
    std::shared_ptr<sql::Connection> conn;
    std::unique_ptr<User> currentUser;

public:
    UserManager(std::shared_ptr<sql::Connection> conn);

    bool addUser(const std::string& username, const std::string& password, const std::string& role);
    bool login(const std::string& username, const std::string& password);
    void logout();
    const User* getCurrentUser() const;
    bool isAdmin() const;
};

#endif