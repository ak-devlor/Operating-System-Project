#ifndef DISK_SCHEDULER_H
#define DISK_SCHEDULER_H

#include <vector>
#include <algorithm>

class DiskScheduler {
private:
    int currentPosition;
    std::vector<int> queue;

public:
    DiskScheduler(int startPosition = 0) : currentPosition(startPosition) {}

    void addRequest(int position) {
        queue.push_back(position);
    }

    // SCAN (Elevator) Algorithm
    std::vector<int> executeSCAN(bool direction = true) {
        std::vector<int> sequence;
        std::vector<int> greater, lesser;

        for (int pos : queue) {
            if (pos >= currentPosition)
                greater.push_back(pos);
            else
                lesser.push_back(pos);
        }

        std::sort(greater.begin(), greater.end());
        std::sort(lesser.begin(), lesser.end());

        if (direction) {  // Moving towards higher track numbers
            sequence.insert(sequence.end(), greater.begin(), greater.end());
            sequence.insert(sequence.end(), lesser.rbegin(), lesser.rend());
        } else {  // Moving towards lower track numbers
            sequence.insert(sequence.end(), lesser.rbegin(), lesser.rend());
            sequence.insert(sequence.end(), greater.begin(), greater.end());
        }

        return sequence;
    }
};

#endif