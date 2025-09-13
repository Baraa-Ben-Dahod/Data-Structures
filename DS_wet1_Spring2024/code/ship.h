#ifndef DS_WET1_SPRING2024_SHIP_H
#define DS_WET1_SPRING2024_SHIP_H

#include "AVL.h"
#include <memory>
#include <algorithm>

// Forward declarations
class Ship;
class Pirate;

class Pirate {
public:
    int id;
    int shipId;
    int treasure;  // Adjusted treasure (actual - ship's extraTreasure)
    int orderInShip;

    Pirate(int id, int shipId, int treasure, int order) 
        : id(id), shipId(shipId), treasure(treasure), orderInShip(order) {}
    
    // Get actual treasure including ship's bonus/penalty
    int getTreasure(const std::shared_ptr<Ship>& ship) const;
};

class Ship {
public:
    const int id;
    const int cannons;
    int numPirates;
    int orderCounter;
    int extraTreasure;  // Bonus/penalty from battles
    int richestPirateId;

    // Primary index: pirate ID -> pirate
    AVL<int, std::shared_ptr<Pirate>> Ship_pirates;
    
    // Secondary index: order -> pirate (for treason operation)
    AVL<int, std::shared_ptr<Pirate>> pirates_Order;
    
    // Tertiary index: treasure -> (pirateID -> pirate)
    // For efficient richest pirate queries
    AVL<int, std::shared_ptr<AVL<int, std::shared_ptr<Pirate>>>> pirates_Treasure;

    Ship(int id, int cannons)
        : id(id), cannons(cannons), numPirates(0), orderCounter(0), 
          extraTreasure(0), richestPirateId(-1) {}

    // Update the richest pirate after modifications
    void updateRichestPirate() {
        auto highestTreasureNode = pirates_Treasure.getBiggest();
        if (highestTreasureNode && highestTreasureNode->value) {
            auto& highestTreasureTree = *(highestTreasureNode->value);
            auto richestPirateNode = highestTreasureTree->getBiggest();
            if (richestPirateNode) {
                richestPirateId = richestPirateNode->key;
                return;
            }
        }
        richestPirateId = -1;
    }
};

// Implementation of Pirate's getTreasure method
inline int Pirate::getTreasure(const std::shared_ptr<Ship>& ship) const {
    return treasure + (ship ? ship->extraTreasure : 0);
}

#endif // DS_WET1_SPRING2024_SHIP_H