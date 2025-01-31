#include "Process.h"
#include "Scheduler.h"
#include "MemoryManager.h"
#include "PageTable.h"
#include "DiskScheduler.h"
#include "ConsoleUI.h"
#include "SecuritySystem.h"
#include "LoginUI.h"
#include <vector>
#include <iostream>
#include <thread>
#include <chrono>

class OSSimulation
{
private:
    ConsoleUI ui;
    SecuritySystem security;
    Scheduler scheduler;
    MemoryManager memoryManager;
    PageTable pageTable;
    DiskScheduler diskScheduler;
    std::vector<Process> processes;
    int nextPID;

public:
    OSSimulation() : scheduler(2),
                     memoryManager(1024),
                     pageTable(256),
                     diskScheduler(0),
                     nextPID(1) {}

    void run()
    {
        while (true)
        {
            ui.showHeader();

            if (!security.getCurrentUser())
            {
                if (!LoginUI::showLoginScreen(security))
                {
                    continue;
                }
            }

            int choice = ui.showMainMenu(security.isAdmin());

            switch (choice)
            {
            case 1:
                handleProcessManagement();
                break;
            case 2:
                if (security.isAdmin())
                {
                    handleMemoryManagement(memoryManager);
                }
                else
                {
                    std::cout << "Access denied! Admin rights required.\n";
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                }
                break;
            case 3:
                if (security.isAdmin())
                {
                    handleDiskManagement();
                }
                else
                {
                    std::cout << "Access denied! Admin rights required.\n";
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                }
                break;
            case 4:
                if (security.isAdmin())
                {
                    handlePageTableOperations();
                }
                else
                {
                    std::cout << "Access denied! Admin rights required.\n";
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                }
                break;
            case 5:
                if (security.isAdmin())
                {
                    handleUserManagement();
                }
                else
                {
                    std::cout << "Access denied! Admin rights required.\n";
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                }
                break;
            case 6:
                security.logout();
                std::cout << "Logged out successfully!\n";
                std::this_thread::sleep_for(std::chrono::seconds(1));
                break;
            case 7:
                std::cout << "Exiting simulation...\n";
                return;
            default:
                std::cout << "Invalid choice! Please try again.\n";
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
        }
    }

private:
    void handleUserManagement()
    {
        while (true)
        {
            ui.showHeader();
            std::cout << "=== User Management ===\n\n";
            std::cout << "1. Add New User\n";
            std::cout << "2. List Users\n";
            std::cout << "3. Return to Main Menu\n\n";
            std::cout << "Enter your choice (1-3): ";

            int choice;
            std::cin >> choice;

            switch (choice)
            {
            case 1:
            {
                std::string username, password;
                int roleChoice;

                std::cout << "Enter username: ";
                std::cin >> username;
                std::cout << "Enter password: ";
                std::cin >> password;
                std::cout << "Select role (1: Guest, 2: User, 3: Admin): ";
                std::cin >> roleChoice;

                UserRole role;
                switch (roleChoice)
                {
                case 1:
                    role = UserRole::GUEST;
                    break;
                case 2:
                    role = UserRole::USER;
                    break;
                case 3:
                    role = UserRole::ADMIN;
                    break;
                default:
                    role = UserRole::GUEST;
                }

                if (security.addUser(username, password, role))
                {
                    std::cout << "User added successfully!\n";
                }
                else
                {
                    std::cout << "Failed to add user!\n";
                }
                std::this_thread::sleep_for(std::chrono::seconds(1));
                break;
            }
            case 2:
            {
                std::cout << "\nUser List:\n";
                std::cout << "Username\tRole\n";
                std::cout << "------------------------\n";

                const auto &users = security.getUsers(); // You'll need to add this method to SecuritySystem
                for (const auto &user : users)
                {
                    std::string roleStr;
                    switch (user.role)
                    {
                    case UserRole::GUEST:
                        roleStr = "Guest";
                        break;
                    case UserRole::USER:
                        roleStr = "User";
                        break;
                    case UserRole::ADMIN:
                        roleStr = "Admin";
                        break;
                    }
                    std::cout << user.username << "\t\t" << roleStr << "\n";
                }

                std::cout << "\nPress Enter to continue...";
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cin.get();
                break;
            }
            case 3:
                return;
            }
        }
    }

private:
    void handleProcessManagement()
    {
        while (true)
        {
            int choice = ui.showProcessMenu();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear input buffer

            switch (choice)
            {
            case 1:
            {
                std::string name;
                int priority, burstTime;

                std::cout << "Enter process name: ";
                std::getline(std::cin, name);
                std::cout << "Enter priority (1-5): ";
                std::cin >> priority;
                std::cout << "Enter burst time: ";
                std::cin >> burstTime;

                Process newProcess(nextPID++, name, priority, burstTime, 0);
                processes.push_back(newProcess);
                scheduler.addProcess(newProcess);

                std::cout << "Process created successfully!\n";
                std::this_thread::sleep_for(std::chrono::seconds(1));
                break;
            }
            case 2:
            {
                std::cout << "\nProcess List:\n";
                std::cout << "PID\tName\tPriority\tBurst Time\n";
                std::cout << "----------------------------------------\n";
                for (const auto &p : processes)
                {
                    std::cout << p.pid << "\t"
                              << p.name << "\t"
                              << p.priority << "\t\t"
                              << p.burstTime << "\n";
                }
                std::cout << "\nPress Enter to continue...";
                std::cin.get();
                break;
            }
            case 3:
            {
                std::cout << "\nExecuting Round Robin Scheduling...\n";
                scheduler.executeRoundRobin();
                std::cout << "\nPress Enter to continue...";
                std::cin.get();
                break;
            }
            case 4:
                return;
            default:
                std::cout << "Invalid choice! Please try again.\n";
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
        }
    }

    void handleMemoryManagement(MemoryManager &memoryManager)
    {
        while (true)
        {
            std::cout << "\n=== Memory Management ===\n";
            std::cout << "1. Allocate Memory\n";
            std::cout << "2. Deallocate Memory\n";
            std::cout << "3. Display Memory Map\n";
            std::cout << "4. Return to Main Menu\n";
            std::cout << "Enter choice: ";

            int choice;
            std::cin >> choice;

            switch (choice)
            {
            case 1:
            {
                int size;
                std::string processName;
                std::cout << "Enter memory size to allocate (MB): ";
                std::cin >> size;
                std::cout << "Enter process name: ";
                std::cin >> processName;

                int address = memoryManager.allocateMemory(size, processName);
                if (address != -1)
                {
                    std::cout << "Memory allocated at address "
                              << address << " for " << processName << "\n";
                }
                else
                {
                    std::cout << "Memory allocation failed!\n";
                }
                break;
            }
            case 2:
            {
                int address;
                std::cout << "Enter memory address to deallocate: ";
                std::cin >> address;

                if (memoryManager.deallocateMemory(address))
                {
                    std::cout << "Memory at address "
                              << address << " deallocated successfully!\n";
                }
                else
                {
                    std::cout << "Memory deallocation failed!\n";
                }
                break;
            }
            case 3:
                memoryManager.displayMemoryMap();
                break;
            case 4:
                return;
            }
        }
    }

    void handleDiskManagement()
    {
        std::cout << "\nDisk Management\n";
        std::cout << "Enter disk request positions (enter -1 to stop):\n";

        int position;
        while (true)
        {
            std::cout << "Position: ";
            std::cin >> position;

            if (position == -1)
                break;
            diskScheduler.addRequest(position);
        }

        std::vector<int> sequence = diskScheduler.executeSCAN();
        std::cout << "\nSCAN Algorithm Sequence: ";
        for (int pos : sequence)
        {
            std::cout << pos << " ";
        }
        std::cout << "\n\nPress Enter to continue...";
        std::cin.ignore();
        std::cin.get();
    }

    void handlePageTableOperations()
    {
        std::cout << "\nPage Table Operations\n";
        int pageNum;
        std::cout << "Enter page number to allocate: ";
        std::cin >> pageNum;

        if (pageTable.allocatePage(pageNum))
        {
            std::cout << "Page " << pageNum << " allocated successfully!\n";
        }
        else
        {
            std::cout << "Page allocation failed!\n";
        }

        std::cout << "\nPress Enter to continue...";
        std::cin.ignore();
        std::cin.get();
    }
};

int main()
{
    OSSimulation simulation;
    simulation.run();
    return 0;
}