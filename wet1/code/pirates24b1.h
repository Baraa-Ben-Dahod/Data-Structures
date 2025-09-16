// 
// 234218 Data Structures 1.
// Semester: 2024B (spring).
// Wet Exercise #1.
// 
// The following header file contains all methods we expect you to implement.
// You MAY add private methods and fields of your own.
// DO NOT erase or modify the signatures of the public methods.
// DO NOT modify the preprocessors in this file.
// DO NOT use the preprocessors in your other code files.
// 

#ifndef PIRRATES24SPRING_WET1_H_
#define PIRRATES24SPRING_WET1_H_

#include "wet1util.h"
#include "Ship.h"

class Ocean {
private:
    AVL<int, std::shared_ptr<Pirate>> Ocean_pirates;
    AVL<int, std::shared_ptr<Ship>> Ocean_ships;
    
public:
    // <DO-NOT-MODIFY> {
    Ocean();

    virtual ~Ocean();
    
    StatusType add_ship(int shipId, int cannons);

    StatusType remove_ship(int shipId);
    
    StatusType add_pirate(int pirateId, int shipId, int treasure);
    
    StatusType remove_pirate(int pirateId);
    
    StatusType treason(int sourceShipId, int destShipId);

    StatusType update_pirate_treasure(int pirateId, int change);

    output_t<int> get_treasure(int pirateId);

    output_t<int> get_cannons(int shipId);

    output_t<int> get_richest_pirate(int shipId);

    StatusType ships_battle(int shipId1,int shipId2);

    // } </DO-NOT-MODIFY>
};

#endif // PIRRATES24SPRING_WET1_H_