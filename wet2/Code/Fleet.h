#pragma once
#include "HashTable.h"
#include <memory>

class Pirate {
public:
    const int pirate_id;
    const int rank;
    int money;
    const int fleet_id;

    Pirate(int id, int r, int f_id)
        : pirate_id(id), rank(r), money(0), fleet_id(f_id) {}
};

class Fleet : public std::enable_shared_from_this<Fleet> {
public:
    const int fleet_id;
    size_t num_ships;
    size_t num_pirates;
    HashTable<int, Pirate> fleet_pirates;

    std::weak_ptr<Fleet> next; // Use weak_ptr to avoid cycles
    int extra_rank;

    Fleet(int id)
        : fleet_id(id), num_ships(1), num_pirates(0), extra_rank(0) {}
};
