#ifndef USER_H
#define USER_H

#include<string>

class User{
private:
    std::string username;
    std::string password;
    std::string role;
public:
    User(const std:: string& username, const std::string &password, const std::string &role);

    bool authenticate(const std::string &password) const;
    const std::string &getUsername() const;
    const std::string &getRole() const;
};

#endif