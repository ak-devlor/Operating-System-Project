# Operating-System-Project

# Operating System Simulation Project
## Comprehensive Documentation & Algorithm Guide

## Table of Contents
1. Project Overview
2. Process Management & Scheduling
3. Memory Management
4. Page Table Management
5. Disk Scheduling
6. Security System

---

## 1. Project Overview

### Project Description
This C++ based Operating System simulation implements core OS concepts including:
- Process scheduling
- Memory allocation and management
- Page table operations
- Disk scheduling
- Security and authentication

### System Architecture
```mermaid
graph TD
    A[User Interface] --> B[Security System]
    B --> C[Process Manager]
    B --> D[Memory Manager]
    B --> E[Page Table]
    B --> F[Disk Scheduler]
    C --> G[Round Robin Scheduler]
    D --> H[First Fit Allocator]
    E --> I[Page Allocation]
    F --> J[SCAN Algorithm]
```

---

## 2. Process Management & Scheduling

### Round Robin Scheduling Algorithm

#### Concept
Round Robin is a preemptive scheduling algorithm that assigns a fixed time quantum to each process in a circular queue.

#### Visual Representation
```mermaid
stateDiagram-v2
    [*] --> Ready
    Ready --> Running: Process Selected
    Running --> Ready: Time Quantum Expired
    Running --> Terminated: Process Complete
    Terminated --> [*]
```

#### How It Works
1. Time Quantum: 2 units
2. Process States:
   - Ready: Waiting for CPU
   - Running: Currently executing
   - Terminated: Execution complete

#### Example Scenario
```mermaid
gantt
    title Round Robin Scheduling (Time Quantum = 2)
    dateFormat X
    axisFormat %s

    section Process 1
    CPU Burst :0, 2
    Waiting   :2, 4
    CPU Burst :4, 5

    section Process 2
    Waiting   :0, 2
    CPU Burst :2, 4
    CPU Burst :5, 6

    section Process 3
    Waiting   :0, 4
    CPU Burst :4, 5
    CPU Burst :6, 7
```

#### Implementation Highlights
```cpp
void executeRoundRobin() {
    while (!readyQueue.empty()) {
        Process current = readyQueue.front();
        readyQueue.pop();
        
        // Execute for time quantum
        int executeTime = min(timeQuantum, current.remainingTime);
        current.remainingTime -= executeTime;
        
        // Requeue if not complete
        if (current.remainingTime > 0) {
            readyQueue.push(current);
        }
    }
}
```

---

## 3. Memory Management

### First Fit Memory Allocation

#### Concept
First Fit searches from the beginning of memory and allocates the first block that's large enough to accommodate the request.

#### Visual Representation
```mermaid
graph TD
    subgraph Memory Blocks
        A[Free: 200MB] --> B[Allocated: 300MB]
        B --> C[Free: 400MB]
        C --> D[Allocated: 100MB]
    end
```

#### Memory Block States
```mermaid
stateDiagram-v2
    [*] --> Free
    Free --> Allocated: Memory Request
    Allocated --> Free: Deallocation
    Free --> Split: Large Block
    Split --> Allocated: Use First Part
    Split --> Free: Remaining Part
```

#### Memory Map Example
```
=== Memory Map ===
Start    Size     Status     Process
0        200MB    Free       N/A
200      300MB    Allocated  Process1
500      400MB    Free       N/A
900      100MB    Allocated  Process2
```

#### Implementation Highlights
```cpp
int allocateMemory(int size, string processName) {
    for (auto& block : memoryBlocks) {
        if (!block.allocated && block.size >= size) {
            // Split if necessary
            if (block.size > size) {
                splitBlock(block, size);
            }
            block.allocated = true;
            block.processName = processName;
            return block.start;
        }
    }
    return -1;
}
```

---

## 4. Page Table Management

### Virtual Memory Paging

#### Concept
Maps virtual page numbers to physical frame numbers, managing memory access and protection.

#### Visual Representation
```mermaid
graph LR
    subgraph Virtual Pages
        VP1[Page 0]
        VP2[Page 1]
        VP3[Page 2]
    end
    subgraph Page Table
        PT1[Entry 0]
        PT2[Entry 1]
        PT3[Entry 2]
    end
    subgraph Physical Frames
        PF1[Frame 0]
        PF2[Frame 1]
        PF3[Frame 2]
    end
    VP1 --> PT1 --> PF2
    VP2 --> PT2 --> PF1
    VP3 --> PT3 --> PF3
```

#### Page Table Entry Structure
```cpp
struct PageTableEntry {
    int frameNumber;  // Physical frame number
    bool valid;       // Is page in memory?
    bool dirty;       // Has page been modified?
    bool referenced;  // Has page been accessed?
};
```

#### Implementation Example
```cpp
bool allocatePage(int pageNumber) {
    if (freeFrames.empty()) return false;
    
    int frame = freeFrames.back();
    freeFrames.pop_back();
    
    pageTable[pageNumber] = {
        frame,  // frameNumber
        true,   // valid
        false,  // dirty
        false   // referenced
    };
    return true;
}
```

---

## 5. Disk Scheduling

### SCAN (Elevator) Algorithm

#### Concept
The disk head moves in one direction servicing requests until it reaches the end, then reverses direction.

#### Visual Representation
```mermaid
graph TD
    subgraph Disk Track Layout
        T0[Track 0] --> T1[Track 20]
        T1 --> T2[Track 40]
        T2 --> T3[Track 60]
        T3 --> T4[Track 80]
        T4 --> T5[Track 100]
    end
    
    style T2 fill:#ff9999
    style T4 fill:#ff9999
```

#### Head Movement Example
```mermaid
xychart-beta
    title "Disk Head Movement (SCAN)"
    x-axis [0, 20, 40, 60, 80, 100]
    y-axis "Time" 0 --> 5
    line [50, 65, 80, 95, 80, 60, 45, 30]
```

#### Implementation Highlights
```cpp
vector<int> executeSCAN(bool direction = true) {
    vector<int> sequence;
    vector<int> greater, lesser;
    
    // Sort requests by location
    for (int pos : requests) {
        if (pos >= currentPosition)
            greater.push_back(pos);
        else
            lesser.push_back(pos);
    }
    
    // Build sequence based on direction
    if (direction) {
        sequence = greater + reverse(lesser);
    } else {
        sequence = reverse(lesser) + greater;
    }
    return sequence;
}
```

---

## 6. Security System

### Role-Based Access Control

#### Authentication Flow
```mermaid
sequenceDiagram
    participant User
    participant Security
    participant System
    
    User->>Security: Login Request
    Security->>Security: Verify Credentials
    alt Valid Credentials
        Security->>System: Grant Access
        System->>User: Show Menu
    else Invalid Credentials
        Security->>User: Error Message
    end
```

#### Access Levels
```mermaid
graph TD
    A[User Access Levels] --> B[Guest]
    A --> C[User]
    A --> D[Admin]
    B --> E[View Processes]
    C --> E
    C --> F[Basic Operations]
    D --> E
    D --> F
    D --> G[System Management]
    D --> H[User Management]
```

#### Implementation Example
```cpp
struct User {
    string username;
    string password;
    UserRole role;
};

bool authenticate(string username, string password) {
    for (User& user : users) {
        if (user.username == username && 
            user.password == password) {
            currentUser = &user;
            return true;
        }
    }
    return false;
}
```

This comprehensive documentation covers all major algorithms and concepts implemented in the project. Each section includes:
- Theoretical explanation
- Visual representation
- Implementation details
- Example scenarios

Read this and enjoy the project.
</antArtifact>
