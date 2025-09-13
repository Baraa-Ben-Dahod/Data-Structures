#pragma once
#include "wet2util.h"
#include "Fleet.h"
#include <memory>

class oceans_t {
private:
    HashTable<int, Fleet> ocean_t_fleets;
    HashTable<int, Pirate> ocean_t_pirates;

    std::shared_ptr<Fleet> find_set(std::shared_ptr<Fleet> fleet);

public:
    oceans_t();
    virtual ~oceans_t();

    StatusType add_fleet(const int fleetId);
    StatusType add_pirate(const int pirateId, const int fleetId);
    StatusType pay_pirate(const int pirateId, const int salary);
    output_t<int> num_ships_for_fleet(const int fleetId);
    output_t<int> get_pirate_money(const int pirateId);
    StatusType unite_fleets(const int fleetId1, const int fleetId2);
    StatusType pirate_argument(const int pirateId1, const int pirateId2);
};

