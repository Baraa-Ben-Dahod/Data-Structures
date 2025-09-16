# Data Structures Homework 1 – Pirates and Ships

## Overview
This project is part of the Technion *Data Structures 1 (234218)* course.  
It implements an object-oriented C++ system to manage **pirates and ships** in the Caribbean.  
Each pirate and ship has a unique ID, and the system supports efficient operations 
such as adding/removing ships, managing pirates, updating treasure, betrayal events, 
and simulating battles.  

All data is stored using **AVL trees** (self-balanced binary search trees), implemented from scratch 
without STL containers, as required by the assignment.

---

## Functionality

### Constructor & Destructor
- **`Ocean()`**: Initialize an empty data structure (no pirates, no ships).  
  **Time:** O(1)  
  **Space:** O(1)

- **`~Ocean()`**: Free all allocated memory, including ships and pirates.  
  **Time:** O(n + m), where *n* = number of pirates, *m* = number of ships.  
  **Space:** O(1) extra (smart pointers handle cleanup).

---

### Ship Management
- **`add_ship(int shipId, int cannons)`**  
  Add a new ship with a given ID and number of cannons.  
  - Fails if ship already exists or invalid input.  
  **Time:** O(log m) (AVL insertion).  
  **Space:** O(1)

- **`remove_ship(int shipId)`**  
  Remove a ship only if it has **no pirates on board**.  
  - Fails if the ship does not exist or still has pirates.  
  **Time:** O(log m)  
  **Space:** O(1)

- **`get_cannons(int shipId)`**  
  Return the number of cannons on the ship.  
  **Time:** O(log m)  
  **Space:** O(1)

---

### Pirate Management
- **`add_pirate(int pirateId, int shipId, int treasure)`**  
  Add a pirate with given ID and initial treasure to a ship.  
  - Fails if pirate already exists or ship does not exist.  
  **Time:** O(log n + log m)  
  **Space:** O(1)

- **`remove_pirate(int pirateId)`**  
  Remove a pirate from the system.  
  - Fails if pirate does not exist.  
  **Time:** O(log n)  
  **Space:** O(1)

- **`update_pirate_treasure(int pirateId, int change)`**  
  Modify a pirate’s treasure (can go negative).  
  **Time:** O(log n)  
  **Space:** O(1)

- **`get_treasure(int pirateId)`**  
  Return the amount of treasure a pirate currently owns.  
  **Time:** O(log n)  
  **Space:** O(1)

---

### Interactions
- **`treason(int sourceShipId, int destShipId)`**  
  Move the **oldest pirate** from one ship to another (queue-like behavior).  
  - Fails if source ship is empty or ships don’t exist.  
  **Time:** O(log m + log n)  
  **Space:** O(1)

- **`get_richest_pirate(int shipId)`**  
  Return the ID of the richest pirate on a ship.  
  - If multiple pirates tie, return the one with the **largest ID**.  
  **Time:** O(log m) + O(1) to track richest in ship structure.  
  **Space:** O(1)

- **`ships_battle(int shipId1, int shipId2)`**  
  Simulate a battle between two ships:  
  - Winner is decided by comparing `min(cannons, #pirates)` of each ship.  
  - Each pirate on the winning ship steals 1 coin from each pirate on the losing ship.  
  **Time:** O(log m + p), where *p* = number of pirates affected.  
  **Space:** O(1)

---

## Data Structures
- **AVL Trees** – used to index ships and pirates by ID.  
  - Ensures O(log n) or O(log m) operations.  
  - Implemented from scratch (`AVL.h`) with insert, remove, find, rotations.  
- **Linked lists / queues inside ships** – used to track pirates by order of arrival (for betrayal).  
- **Smart pointers (`std::shared_ptr`)** – used for safe memory management.

---

## Complexity Summary
- Ship operations: O(log m)  
- Pirate operations: O(log n)  
- Mixed operations (pirate+ship): O(log n + log m)  
- Battle & betrayal: O(log m + log n + affected pirates)  
- Space: O(n + m), linear in the number of pirates and ships.

---

## What We Learned
By completing this assignment, we gained hands-on experience with:
- **AVL Trees** – implementing rotations, balancing, insert/remove/find.  
- **Manual memory management** – preventing leaks using smart pointers.  
- **Designing composite data structures** – combining trees and lists to support complex operations.  
- **Complexity analysis** – ensuring each function meets required bounds.  
- **Object-Oriented Design in C++** – clean separation between `Ocean`, `Ship`, and `Pirate` entities.

---

## Folder Structure
```
wet1/
│
├── code/       # C++ source and header files
├── tests/      # Input/output test files
└── README.md   # This documentation
```

---

## Compilation & Running
On Linux (CSL3 server environment as required):
```bash
g++ -std=c++11 -DNDEBUG -Wall code/*.cpp -o pirates
./pirates < tests/test1.in > tests/test1.out
```

---

## Notes
- This project is a **homework solution** for the Technion Data Structures 1 course.  
- STL containers and algorithms were **not used** (per assignment rules).  
- Focus was on **AVL trees, queues, and custom data structures**.
