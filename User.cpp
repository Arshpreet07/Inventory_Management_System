#include "User.h"

using namespace std;

User::User(const string& username, const string& password, const string& role) 
    : username(username), password(password), role(role) {}

bool User::authenticate(const string& password) const{
    return this->password == password;
}

const string& User::getUsername() const{
    return username;
}

const string& User::getRole() const{
    return role;
}