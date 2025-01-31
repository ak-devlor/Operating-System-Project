#ifndef USER_H
#define USER_H

#include <string>

enum class UserRole {
    GUEST,
    USER,
    ADMIN
};

struct User {
    std::string username;
    std::string password;
    UserRole role;
};
#endif