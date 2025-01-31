#ifndef SECURITY_SYSTEM_H
#define SECURITY_SYSTEM_H

#include <vector>
#include <string>
#include "User.h"

class SecuritySystem
{
private:
    std::vector<User> users;
    User *currentUser;

public:
    struct UserInfo
    {
        std::string username;
        UserRole role;
    };

    std::vector<UserInfo> getUsers() const
    {
        std::vector<UserInfo> userList;
        for (const auto &user : users)
        { // Assuming 'users' is your container of user data
            userList.push_back({user.username, user.role});
        }
        return userList;
    }

    SecuritySystem()
    {
        // Initialize with default admin account
        users.push_back({"admin", "1234", UserRole::ADMIN});
        currentUser = nullptr;
    }

    bool login(const std::string &username, const std::string &password)
    {
        for (User &user : users)
        {
            if (user.username == username && user.password == password)
            {
                currentUser = &user;
                return true;
            }
        }
        return false;
    }

    void logout()
    {
        currentUser = nullptr;
    }

    bool isAdmin() const
    {
        return currentUser && currentUser->role == UserRole::ADMIN;
    }

    bool addUser(const std::string &username, const std::string &password, UserRole role)
    {
        if (!isAdmin())
            return false;

        // Check if username already exists
        for (const User &user : users)
        {
            if (user.username == username)
                return false;
        }

        users.push_back({username, password, role});
        return true;
    }

    User *getCurrentUser()
    {
        return currentUser;
    }
};
#endif