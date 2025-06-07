#include "UserManager.h"
#include "User.h"
#include "AuditLogger.h"
#include "DatabaseConnection.h"

#include <jdbc/cppconn/resultset.h>
#include <jdbc/cppconn/prepared_statement.h>
#include <iostream>

using namespace std;

UserManager::UserManager(shared_ptr<sql::Connection> conn)
    : conn(std::move(conn)) {}

bool UserManager::addUser(const string& username, const string& password, const string& role){
    try {
        unique_ptr<sql::PreparedStatement> pstmt(
            conn->prepareStatement("INSERT INTO Users (username, password, role) VALUES (?, ?, ?)"));
        pstmt->setString(1, username);
        pstmt->setString(2, password); 
        pstmt->setString(3, role);
        pstmt->executeUpdate();

        AuditLogger(conn).logEvent("New user registered: " + username);
        cout << "User " << username << " added successfully.\n";
        return true;
    } catch (const sql::SQLException& e) {
        cerr << "SQL Error adding user: " << e.what() << endl;
        return false;
    }
}

bool UserManager::login(const string& username, const string& password){
    try {
        unique_ptr<sql::PreparedStatement> pstmt(
            conn->prepareStatement("SELECT password, role FROM Users WHERE username = ?"));
        pstmt->setString(1, username);
        unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

        if (res->next()) {
            string storedPassword = res->getString("password");
            string role = res->getString("role");

            if (storedPassword == password) {  
                currentUser = make_unique<User>(username, password, role);
                AuditLogger(conn).logEvent("User logged in: " + username);
                cout << "Login successful. Welcome, " << username << ".\n";
                return true;
            }
        }

        cout << "Invalid username or password.\n";
        return false;
    } catch (const sql::SQLException& e) {
        cerr << "SQL Error during login: " << e.what() << endl;
        return false;
    }
}

void UserManager::logout(){
    if (currentUser) {
        AuditLogger(conn).logEvent("User logged out: " + currentUser->getUsername());
        cout << "Goodbye " << currentUser->getUsername() << "!" << endl;
    } else {
        cout << "No user is currently logged in!" << endl;
    }
    currentUser.reset();
}

const User* UserManager::getCurrentUser() const{
    return currentUser.get();
}
bool UserManager::isAdmin() const{
    return (currentUser && currentUser->getRole() == "admin");
}