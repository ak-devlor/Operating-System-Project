#ifndef PROCESS_H
#define PROCESS_H

#include <string>

class Process {
public:
    int pid;
    std::string name;
    int priority;
    int burstTime;
    int arrivalTime;
    int remainingTime;
    bool isCompleted;
    bool isInQueue;

    Process(int _pid, std::string _name, int _priority, int _burst, int _arrival) {
        pid = _pid;
        name = _name;
        priority = _priority;
        burstTime = _burst;
        arrivalTime = _arrival;
        remainingTime = _burst;
        isCompleted = false;
        isInQueue = false;
    }
};

#endif