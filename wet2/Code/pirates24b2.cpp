#include "pirates24b2.h"
#include <memory>
#include <algorithm>

oceans_t::oceans_t() = default;
oceans_t::~oceans_t() = default;

// Union-Find with path compression
std::shared_ptr<Fleet> oceans_t::find_set(std::shared_ptr<Fleet> fleet) {
    if (!fleet) return nullptr;
    if (fleet->next.expired()) return fleet;

    auto parent = fleet->next.lock();
    auto root = find_set(parent);

    if (root) {
        fleet->extra_rank += parent->extra_rank;
        fleet->next = root;
    }
    return root;
}

StatusType oceans_t::add_fleet(const int fleetId) {
    if (fleetId <= 0) return StatusType::INVALID_INPUT;
    if (ocean_t_fleets.find(fleetId)) return StatusType::FAILURE;

    auto newFleet = std::make_shared<Fleet>(fleetId);
    ocean_t_fleets.insert(fleetId, newFleet);
    return StatusType::SUCCESS;
}

StatusType oceans_t::add_pirate(const int pirateId, const int fleetId) {
    if (pirateId <= 0 || fleetId <= 0) return StatusType::INVALID_INPUT;
    if (ocean_t_pirates.find(pirateId)) return StatusType::FAILURE;

    auto fleet = ocean_t_fleets.find(fleetId);
    if (!fleet) return StatusType::FAILURE;

    auto rootFleet = find_set(fleet);
    if (rootFleet != fleet) return StatusType::FAILURE;

    auto newPirate = std::make_shared<Pirate>(pirateId, fleet->num_pirates + 1, fleetId);
    ocean_t_pirates.insert(pirateId, newPirate);
    fleet->num_pirates++;
    fleet->fleet_pirates.insert(pirateId, newPirate);

    return StatusType::SUCCESS;
}

StatusType oceans_t::pay_pirate(const int pirateId, const int salary) {
    if (pirateId <= 0 || salary <= 0) return StatusType::INVALID_INPUT;
    auto pirate = ocean_t_pirates.find(pirateId);
    if (!pirate) return StatusType::FAILURE;

    pirate->money += salary;
    return StatusType::SUCCESS;
}

output_t<int> oceans_t::num_ships_for_fleet(const int fleetId) {
    if (fleetId <= 0) return StatusType::INVALID_INPUT;
    auto fleet = ocean_t_fleets.find(fleetId);
    if (!fleet) return StatusType::FAILURE;

    auto rootFleet = find_set(fleet);
    if (!rootFleet || rootFleet != fleet) return StatusType::FAILURE;

    return static_cast<int>(rootFleet->num_ships);
}

output_t<int> oceans_t::get_pirate_money(const int pirateId) {
    if (pirateId <= 0) return StatusType::INVALID_INPUT;
    auto pirate = ocean_t_pirates.find(pirateId);
    if (!pirate) return StatusType::FAILURE;

    return pirate->money;
}

StatusType oceans_t::unite_fleets(const int fleetId1, const int fleetId2) {
    if (fleetId1 <= 0 || fleetId2 <= 0 || fleetId1 == fleetId2)
        return StatusType::INVALID_INPUT;

    auto fleet1 = ocean_t_fleets.find(fleetId1);
    auto fleet2 = ocean_t_fleets.find(fleetId2);
    if (!fleet1 || !fleet2) return StatusType::FAILURE;

    auto root1 = find_set(fleet1);
    auto root2 = find_set(fleet2);
    if (!root1 || !root2) return StatusType::FAILURE;
    if (root1 != fleet1 || root2 != fleet2) return StatusType::FAILURE;
    if (fleet1->num_pirates == 0 || fleet2->num_pirates == 0) return StatusType::FAILURE;

    std::shared_ptr<Fleet> primary = (fleet1->num_pirates >= fleet2->num_pirates) ? fleet1 : fleet2;
    std::shared_ptr<Fleet> secondary = (primary == fleet1) ? fleet2 : fleet1;

    secondary->next = primary;
    secondary->extra_rank += primary->num_pirates;
    primary->num_pirates += secondary->num_pirates;
    primary->num_ships += secondary->num_ships;

    return StatusType::SUCCESS;
}

StatusType oceans_t::pirate_argument(const int pirateId1, const int pirateId2) {
    if (pirateId1 <= 0 || pirateId2 <= 0 || pirateId1 == pirateId2)
        return StatusType::INVALID_INPUT;

    auto pirate1 = ocean_t_pirates.find(pirateId1);
    auto pirate2 = ocean_t_pirates.find(pirateId2);
    if (!pirate1 || !pirate2) return StatusType::FAILURE;

    auto fleet1 = ocean_t_fleets.find(pirate1->fleet_id);
    auto fleet2 = ocean_t_fleets.find(pirate2->fleet_id);
    auto root1 = find_set(fleet1);
    auto root2 = find_set(fleet2);
    if (!root1 || !root2 || root1 != root2) return StatusType::FAILURE;

    int extra1 = pirate1->rank + fleet1->extra_rank;
    int extra2 = pirate2->rank + fleet2->extra_rank;
    int d = abs(extra1 - extra2);

    if (extra1 > extra2) {
        pirate1->money -= d;
        pirate2->money += d;
    } else {
        pirate1->money += d;
        pirate2->money -= d;
    }

    return StatusType::SUCCESS;
}
