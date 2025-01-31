#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include <vector>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <numeric>

class MemoryBlock {
public:
    int start;
    int size;
    bool allocated;
    std::string processName;

    MemoryBlock(int _start, int _size, bool _allocated = false, 
                const std::string& _processName = "")
        : start(_start), size(_size), 
          allocated(_allocated), processName(_processName) {}
};

class MemoryManager {
private:
    int totalMemorySize;
    std::vector<MemoryBlock> memoryBlocks;

public:
    MemoryManager(int totalSize = 1024) : totalMemorySize(totalSize) {
        // Initialize with one large free block
        memoryBlocks.emplace_back(0, totalSize, false);
    }

    // Improved memory allocation with process name
    int allocateMemory(int size, const std::string& processName) {
        for (auto& block : memoryBlocks) {
            if (!block.allocated && block.size >= size) {
                // If block is larger, split it
                if (block.size > size) {
                    MemoryBlock newBlock(
                        block.start + size, 
                        block.size - size, 
                        false
                    );
                    block.size = size;
                    memoryBlocks.push_back(newBlock);
                }

                // Mark block as allocated
                block.allocated = true;
                block.processName = processName;
                return block.start;
            }
        }
        return -1; // Allocation failed
    }

    // Improved deallocation
    bool deallocateMemory(int startAddress) {
        auto it = std::find_if(memoryBlocks.begin(), memoryBlocks.end(), 
            [startAddress](const MemoryBlock& block) {
                return block.start == startAddress && block.allocated;
            });

        if (it != memoryBlocks.end()) {
            it->allocated = false;
            it->processName = "";
            
            // Merge with adjacent free blocks
            mergeAdjacentFreeBlocks();
            return true;
        }
        return false;
    }

    // Display memory map
    void displayMemoryMap() {
        std::cout << "\n=== Memory Map ===\n";
        std::cout << std::left 
                  << std::setw(10) << "Start" 
                  << std::setw(10) << "Size" 
                  << std::setw(15) << "Status" 
                  << "Process\n";
        std::cout << "---------------------------------------------\n";

        // Sort blocks by start address
        std::sort(memoryBlocks.begin(), memoryBlocks.end(), 
            [](const MemoryBlock& a, const MemoryBlock& b) {
                return a.start < b.start;
            });

        for (const auto& block : memoryBlocks) {
            std::cout << std::left 
                      << std::setw(10) << block.start 
                      << std::setw(10) << block.size 
                      << std::setw(15) << (block.allocated ? "Allocated" : "Free")
                      << (block.allocated ? block.processName : "N/A") << "\n";
        }

        // Calculate and display memory usage
        int allocatedMemory = calculateAllocatedMemory();
        std::cout << "\nTotal Memory: " << totalMemorySize << " MB\n";
        std::cout << "Allocated Memory: " << allocatedMemory << " MB\n";
        std::cout << "Free Memory: " << (totalMemorySize - allocatedMemory) << " MB\n";
    }

private:
    // Merge adjacent free memory blocks
    void mergeAdjacentFreeBlocks() {
        std::sort(memoryBlocks.begin(), memoryBlocks.end(), 
            [](const MemoryBlock& a, const MemoryBlock& b) {
                return a.start < b.start;
            });

        for (auto it = memoryBlocks.begin(); it != memoryBlocks.end(); ) {
            auto next = std::next(it);
            
            // If current and next blocks are free and adjacent
            if (next != memoryBlocks.end() && 
                !it->allocated && !next->allocated &&
                it->start + it->size == next->start) {
                
                // Merge blocks
                it->size += next->size;
                memoryBlocks.erase(next);
                // Don't increment iterator as vector has changed
            } else {
                ++it;
            }
        }
    }

    // Calculate total allocated memory
    int calculateAllocatedMemory() {
        return std::accumulate(memoryBlocks.begin(), memoryBlocks.end(), 0,
            [](int total, const MemoryBlock& block) {
                return total + (block.allocated ? block.size : 0);
            });
    }
};
#endif