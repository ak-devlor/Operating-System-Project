#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <queue>
#include <vector>
#include <iostream>
#include "Process.h"

class Scheduler {
private:
    std::vector<Process> processes;
    std::queue<Process> readyQueue;
    int timeQuantum;
    int currentTime;

public:
    Scheduler(int quantum = 2) : timeQuantum(quantum), currentTime(0) {}
    
    void addProcess(Process p) {
        processes.push_back(p);
    }
    
    void executeRoundRobin() {
        // Initialize ready queue
        for (auto& p : processes) {
            if (p.arrivalTime <= currentTime && !p.isCompleted && !p.isInQueue) {
                readyQueue.push(p);
                p.isInQueue = true; // Mark the process as added to the queue
            }
        }

        while (!readyQueue.empty()) {
            Process current = readyQueue.front();
            readyQueue.pop();
            current.isInQueue = false; // Mark the process as removed from the queue

            // Execute for time quantum or remaining time
            int executeTime = std::min(timeQuantum, current.remainingTime);
            current.remainingTime -= executeTime;
            currentTime += executeTime;

            // Check if process is completed
            if (current.remainingTime > 0) {
                readyQueue.push(current);
                current.isInQueue = true; // Mark the process as added to the queue again
            } else {
                current.isCompleted = true;
                std::cout << "Process " << current.pid << " completed at time " << currentTime << std::endl;
            }

            // Add newly arrived processes
            for (auto& p : processes) {
                if (!p.isCompleted && p.arrivalTime <= currentTime && !p.isInQueue) {
                    readyQueue.push(p);
                    p.isInQueue = true; // Mark the process as added to the queue
                }
            }
        }
    }
};

#endif