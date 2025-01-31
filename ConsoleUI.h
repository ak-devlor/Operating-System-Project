#ifndef CONSOLE_UI_H
#define CONSOLE_UI_H

#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <limits>

class ConsoleUI {
private:
    void clearScreen() {
        std::cout << "\033[2J\033[1;1H";
    }

public:
    void showHeader() {
        clearScreen();
        std::cout << "====================================\n";
        std::cout << "     Operating System Simulation     \n";
        std::cout << "====================================\n\n";
    }

    int showMainMenu() {
        std::cout << "1. Process Management\n";
        std::cout << "2. Memory Management\n";
        std::cout << "3. Disk Management\n";
        std::cout << "4. Page Table Operations\n";
        std::cout << "5. Exit\n\n";
        std::cout << "Enter your choice (1-5): ";
        
        int choice;
        std::cin >> choice;
        return choice;
    }

    int showProcessMenu() {
        clearScreen();
        std::cout << "=== Process Management ===\n\n";
        std::cout << "1. Create New Process\n";
        std::cout << "2. Show All Processes\n";
        std::cout << "3. Run Round Robin Scheduling\n";
        std::cout << "4. Return to Main Menu\n\n";
        std::cout << "Enter your choice (1-4): ";
        
        int choice;
        std::cin >> choice;
        return choice;
    }

    int showMemoryMenu() {
    clearScreen();
    std::cout << "=== Memory Management ===\n\n";
    std::cout << "1. Allocate Memory\n";
    std::cout << "2. Deallocate Memory\n";
    std::cout << "3. Show Memory Map\n";
    std::cout << "4. Return to Main Menu\n\n";
    std::cout << "Enter your choice (1-4): ";
    
    int choice;
    std::cin >> choice;
    return choice;
}
    
    int showMainMenu(bool isAdmin) {
        std::cout << "1. Process Management\n";
        if (isAdmin) {
            std::cout << "2. Memory Management\n";
            std::cout << "3. Disk Management\n";
            std::cout << "4. Page Table Operations\n";
            std::cout << "5. User Management\n";
        }
        std::cout << "6. Logout\n";
        std::cout << "7. Exit\n\n";
        std::cout << "Enter your choice: ";
        
        int choice;
        std::cin >> choice;
        return choice;
    }
};

#endif