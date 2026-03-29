# Hospital Fund Allocator

> A C++ e-wallet system that converts transactions into micro-donations, pooling funds and allocating them to hospitals based on need level.

---

## Overview

The **Hospital Fund Allocator** is a console-based C++ application that simulates a micro-donation ecosystem. Every transaction made through the system triggers a small donation that flows into a centralized healthcare pool. Funds are then intelligently allocated to registered hospitals based on their priority and need level.

---

## Concepts & Data Structures Used

| Concept | Usage |
|---|---|
| **Singleton** | Centralized Donation Pool — only one instance exists system-wide |
| **OOP** | Classes for Donors, Hospitals, Transactions, and Allocation |
| **Linked List** | Donor and transaction chain management |
| **Stack** | Undo last allocation functionality |
| **Queue** | Hospital funding request queue |
| **Recursion** | Fund allocation logic across hospitals |

---

## Modules

### Module 1 — Donor & Transaction Management
Handles donor registration and tracks all transactions. Each transaction automatically triggers a micro-donation to the central pool.

### Module 2 — Hospital & Funding Request Management
Manages hospital registration and funding requests. Hospitals are assigned a need level that determines allocation priority.

### Module 3 — Donation Pool & Allocation Manager
The core of the system. A Singleton donation pool collects all micro-donations. Funds are recursively allocated to the highest-need hospital. Supports undo of the last allocation using a Stack.

### Module 4 — Audit & Transparency Report
Logs all transactions and allocations for full transparency. Generates audit reports to track fund flow from donor to hospital.

---

## 🗂️ Project Structure

```
PROJECT/
├── Module1_Donor_Transaction_Management/
│   └── Mod1.cpp
├── Module2_Hospital_Funding_Request/
│   └── Hospital_Funding_Request_Management.cpp
├── Module3_Donation_Pool_Allocation/
│   └── donation_pool_allocation_manager.cpp
├── Module4_Audit_Transparency/
│   └── auditlogfinalcode.cpp
├── Integrated_Final_Compilation.cpp
├── .gitignore
└── README.md
```

---

##  How to Run

### Prerequisites
- g++ compiler (MinGW-W64 recommended for Windows)

### Compile & Run
```bash
g++ -g "Integrated_Final_Compilation.cpp" -o output.exe
output.exe
```

### Menu Options
```
1. Add Funds to Donation Pool
2. Add a Hospital
3. Allocate to Highest Need Hospital
4. Undo Last Allocation
5. Show Current Pool Balance
6. Exit
```

---




