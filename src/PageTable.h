#ifndef PAGE_TABLE_H
#define PAGE_TABLE_H

#include <vector>
#include <unordered_map>

struct PageTableEntry {
    int frameNumber;
    bool valid;
    bool dirty;
    bool referenced;
};

class PageTable {
private:
    std::unordered_map<int, PageTableEntry> pageTable;
    std::vector<int> freeFrames;
    int totalFrames;

public:
    PageTable(int numFrames) : totalFrames(numFrames) {
        for (int i = 0; i < numFrames; i++) {
            freeFrames.push_back(i);
        }
    }

    bool allocatePage(int pageNumber) {
        if (freeFrames.empty()) {
            return false;
        }

        int frame = freeFrames.back();
        freeFrames.pop_back();

        pageTable[pageNumber] = {
            frame,      // frameNumber
            true,       // valid
            false,      // dirty
            false       // referenced
        };
        return true;
    }

    bool accessPage(int pageNumber, bool isWrite = false) {
        auto it = pageTable.find(pageNumber);
        if (it == pageTable.end() || !it->second.valid) {
            return false; // Page fault
        }

        it->second.referenced = true;
        if (isWrite) {
            it->second.dirty = true;
        }
        return true;
    }
};

#endif