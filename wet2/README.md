# Data Structures Homework 2 – Pirate Fleets

## Overview
This project is part of the Technion *Data Structures 1 (234218)* course.  
It extends Homework 1: now the government organizes **fleets** made of ships and pirates.  
Each fleet and pirate has a unique ID, and the system must support dynamic management of fleets, pirates, and their interactions.

All data is stored using **custom hash tables** and a **union–find style structure** to support fleet merging.  
No STL containers were used (per assignment rules).

---

## Functionality

### Constructor & Destructor
- **`oceans_t()`**: Initialize an empty data structure (no fleets, no pirates).  
  **Time:** O(1)  
  **Space:** O(1)

- **`~oceans_t()`**: Free all allocated memory (all fleets and pirates).  
  **Time:** O(n + m), where *n* = number of pirates, *m* = number of fleets.  
  **Space:** O(1) extra.

---

### Fleet Management
- **`add_fleet(int fleetId)`**  
  Add a new fleet with a given ID. It starts with one ship and no pirates.  
  - Fails if fleet already exists or input invalid.  
  **Time:** O(1) amortized (hash table insertion).  
  **Space:** O(1)

- **`unite_fleets(int fleetId1, int fleetId2)`**  
  Merge two fleets:  
  - New fleet keeps the ID of the larger one (by #pirates).  
  - Ships and pirates are merged, ranks of smaller fleet pirates increase.  
  - Fails if either fleet does not exist.  
  **Time:** O(log m) amortized (hash table + merging).  
  **Space:** O(1)

- **`num_ships_for_fleet(int fleetId)`**  
  Return the number of ships in the fleet.  
  **Time:** O(log m) amortized  
  **Space:** O(1)

---

### Pirate Management
- **`add_pirate(int pirateId, int fleetId)`**  
  Recruit a pirate into a fleet. Pirate gets a rank = (#pirates in fleet + 1).  
  **Time:** O(log m) amortized (hash table insertion).  
  **Space:** O(1)

- **`pay_pirate(int pirateId, int salary)`**  
  Give salary to a pirate. Updates pirate’s money balance.  
  - Salary must be > 0.  
  **Time:** O(1) amortized  
  **Space:** O(1)

- **`get_pirate_money(int pirateId)`**  
  Return the money owned by a pirate.  
  **Time:** O(1) amortized  
  **Space:** O(1)

- **`pirate_argument(int pirateId1, int pirateId2)`**  
  Simulate an argument between two pirates of the same fleet:  
  - Higher-ranked pirate pays the lower-ranked pirate an amount = rank difference.  
  - Pirates can go negative in money.  
  **Time:** O(log m) amortized  
  **Space:** O(1)

---

## Data Structures
- **Hash Table (custom implementation)** – used to map fleet IDs and pirate IDs to objects.  
  - Implements dynamic resizing, chaining for collisions.  
  - Supports O(1) expected time for insert/find.  

- **Fleet class** – manages number of ships, pirates, and references to members.  

- **Union–Find (Disjoint Set Union)** – used indirectly to merge fleets efficiently (`find_set` with path compression).  

- **Pirate objects** – store ID, rank, money, and reference to fleet.

---

## Complexity Summary
- Fleet operations: O(1) amortized  
- Pirate operations: O(1) amortized  
- Fleet merging: O(log m) amortized  
- Space: O(n + m), linear in pirates + fleets.

---

## What We Learned
By completing this assignment, we gained hands-on experience with:
- **Hash Tables** – implementing collision handling and dynamic resizing.  
- **Union–Find (Disjoint Sets)** – managing merges of fleets and maintaining rank consistency.  
- **Object-oriented design** – composing `oceans_t`, `Fleet`, and `Pirate` classes.  
- **Amortized analysis** – proving efficiency of hash operations.  
- **Memory safety** – using smart pointers to avoid leaks.  

---

## Folder Structure
```
wet2/
│
├── code/       # C++ source and header files
└── README.md   # This documentation
```

---

## Compilation & Running
On Linux (CSL3 server environment as required):
```bash
g++ -std=c++11 -DNDEBUG -Wall code/*.cpp -o fleets
./fleets < input.in > output.out
```

---

## Notes
- This project is a **homework solution** for the Technion Data Structures 1 course.  
- STL containers and algorithms were **not used** (per assignment rules).  
- Focus was on **hash tables, disjoint sets, and amortized analysis**.
