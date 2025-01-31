#ifndef LOGIN_UI_H
#define LOGIN_UI_H

#include <iostream>
#include "SecuritySystem.h"

class LoginUI {
public:
    static bool showLoginScreen(SecuritySystem& security) {
        std::string username, password;
        int attempts = 0;
        const int MAX_ATTEMPTS = 3;

        while (attempts < MAX_ATTEMPTS) {
            std::cout << "\n=== Login Screen ===\n";
            std::cout << "Username: ";
            std::cin >> username;
            std::cout << "Password: ";
            std::cin >> password;

            if (security.login(username, password)) {
                std::cout << "\nLogin successful!\n";
                return true;
            }

            attempts++;
            std::cout << "\nInvalid credentials! Attempts remaining: " 
                      << (MAX_ATTEMPTS - attempts) << "\n";
        }

        std::cout << "\nToo many failed attempts. Please try again later.\n";
        return false;
    }
};
#endif