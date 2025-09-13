#include "pirates24b1.h"

Ocean::Ocean() = default;

Ocean::~Ocean() = default;

StatusType Ocean::add_ship(int shipId, int cannons) {
    if (shipId <= 0 || cannons < 0) {
        return StatusType::INVALID_INPUT;
    }

    if (Ocean_ships.find(shipId)) {
        return StatusType::FAILURE;
    }

    try {
        auto newShip = std::make_shared<Ship>(shipId, cannons);
        Ocean_ships.insert(shipId, newShip);
    } catch (const std::bad_alloc&) {
        return StatusType::ALLOCATION_ERROR;
    }

    return StatusType::SUCCESS;
}

StatusType Ocean::remove_ship(int shipId) {
    if (shipId <= 0) {
        return StatusType::INVALID_INPUT;
    }

    auto shipNode = Ocean_ships.find(shipId);
    if (!shipNode) {
        return StatusType::FAILURE;
    }
    
    try {
        if ((*shipNode)->numPirates > 0) {
            return StatusType::FAILURE;
        }
        Ocean_ships.remove(shipId);
    } catch (const std::bad_alloc&) {
        return StatusType::ALLOCATION_ERROR;
    }

    return StatusType::SUCCESS;
}

StatusType Ocean::add_pirate(int pirateId, int shipId, int treasure) {
    if (pirateId <= 0 || shipId <= 0) {
        return StatusType::INVALID_INPUT;
    }
  
    if (Ocean_pirates.find(pirateId)) {
        return StatusType::FAILURE;
    }

    auto shipNode = Ocean_ships.find(shipId);
    if (!shipNode) {
        return StatusType::FAILURE;
    }

    try {
        auto currentShip = *shipNode;
        currentShip->orderCounter++;
        const int order = currentShip->orderCounter;
        
        auto newPirate = std::make_shared<Pirate>(pirateId, shipId, treasure, order);
        Ocean_pirates.insert(pirateId, newPirate);
        currentShip->Ship_pirates.insert(pirateId, newPirate);
        currentShip->pirates_Order.insert(order, newPirate);
        
        const int adjustedTreasure = treasure - currentShip->extraTreasure;
        newPirate->treasure = adjustedTreasure;
        
        auto treasureNode = currentShip->pirates_Treasure.find(adjustedTreasure);
        if (!treasureNode) {
            auto newTreasureTree = std::make_shared<AVL<int, std::shared_ptr<Pirate>>>();
            newTreasureTree->insert(pirateId, newPirate);
            currentShip->pirates_Treasure.insert(adjustedTreasure, newTreasureTree);
        } else {
            (*treasureNode)->insert(pirateId, newPirate);
        }

        currentShip->numPirates++;
        currentShip->updateRichestPirate();
        
    } catch (const std::bad_alloc&) {
        return StatusType::ALLOCATION_ERROR;
    }

    return StatusType::SUCCESS;
}

StatusType Ocean::remove_pirate(int pirateId) {
    if (pirateId <= 0) {
        return StatusType::INVALID_INPUT;
    }

    auto pirateNode = Ocean_pirates.find(pirateId);
    if (!pirateNode) {
        return StatusType::FAILURE;
    }
   
    auto currentPirate = *pirateNode;
    auto shipNode = Ocean_ships.find(currentPirate->shipId);
    if (!shipNode) {
        return StatusType::FAILURE;
    }
    
    auto currentShip = *shipNode;
    
    try {
        Ocean_pirates.remove(pirateId);
        currentShip->Ship_pirates.remove(pirateId);
        currentShip->pirates_Order.remove(currentPirate->orderInShip);
        
        const int adjustedTreasure = currentPirate->treasure;
        auto treasureNode = currentShip->pirates_Treasure.find(adjustedTreasure);
        if (treasureNode) {
            auto treasureTree = *treasureNode;
            treasureTree->remove(pirateId);
            if (treasureTree->isEmpty()) {
                currentShip->pirates_Treasure.remove(adjustedTreasure);
            }
        }

        currentShip->numPirates--;
        currentShip->updateRichestPirate();

    } catch (const std::bad_alloc&) {
        return StatusType::ALLOCATION_ERROR;
    }

    return StatusType::SUCCESS;
}

StatusType Ocean::treason(int sourceShipId, int destShipId) {
    if (sourceShipId <= 0 || destShipId <= 0 || sourceShipId == destShipId) {
        return StatusType::INVALID_INPUT;
    }

    auto sourceShipNode = Ocean_ships.find(sourceShipId);
    auto destShipNode = Ocean_ships.find(destShipId);

    if (!sourceShipNode || !destShipNode) {
        return StatusType::FAILURE;
    }

    auto sourceShip = *sourceShipNode;
    auto destShip = *destShipNode;

    if (sourceShip->numPirates == 0) {
        return StatusType::FAILURE;
    }

    try {
        auto firstPirateNode = sourceShip->pirates_Order.getSmallest();
        if (!firstPirateNode || !firstPirateNode->value) {
            return StatusType::FAILURE;
        }
        
        auto pirateToMove = *(firstPirateNode->value);
        const int pirateId = pirateToMove->id;
        const int oldOrder = pirateToMove->orderInShip;
        const int originalTreasure = pirateToMove->getTreasure(sourceShip);

        // Remove from source ship
        sourceShip->pirates_Order.remove(oldOrder);
        sourceShip->Ship_pirates.remove(pirateId);

        auto treasureNode = sourceShip->pirates_Treasure.find(pirateToMove->treasure);
        if (treasureNode) {
            auto treasureTree = *treasureNode;
            treasureTree->remove(pirateId);
            if (treasureTree->isEmpty()) {
                sourceShip->pirates_Treasure.remove(pirateToMove->treasure);
            }
        }
        sourceShip->numPirates--;

        // Add to destination ship
        pirateToMove->shipId = destShipId;
        destShip->orderCounter++;
        pirateToMove->orderInShip = destShip->orderCounter;

        destShip->pirates_Order.insert(pirateToMove->orderInShip, pirateToMove);
        destShip->Ship_pirates.insert(pirateId, pirateToMove);

        const int adjustedTreasure = originalTreasure - destShip->extraTreasure;
        pirateToMove->treasure = adjustedTreasure;
        
        treasureNode = destShip->pirates_Treasure.find(adjustedTreasure);
        if (!treasureNode) {
            auto newTreasureTree = std::make_shared<AVL<int, std::shared_ptr<Pirate>>>();
            newTreasureTree->insert(pirateId, pirateToMove);
            destShip->pirates_Treasure.insert(adjustedTreasure, newTreasureTree);
        } else {
            (*treasureNode)->insert(pirateId, pirateToMove);
        }
        
        destShip->numPirates++;
        sourceShip->updateRichestPirate();
        destShip->updateRichestPirate();
        
    } catch (const std::bad_alloc&) {
        return StatusType::ALLOCATION_ERROR;
    }

    return StatusType::SUCCESS;
}

StatusType Ocean::update_pirate_treasure(int pirateId, int change) {
    if (pirateId <= 0) {
        return StatusType::INVALID_INPUT;
    }

    if (change == 0) {
        return StatusType::SUCCESS;
    }

    auto pirateNode = Ocean_pirates.find(pirateId);
    if (!pirateNode) {
        return StatusType::FAILURE;
    }

    auto currentPirate = *pirateNode;
    auto shipNode = Ocean_ships.find(currentPirate->shipId);
    if (!shipNode) {
        return StatusType::FAILURE;
    }
    
    auto currentShip = *shipNode;

    try {
        // Remove from old treasure tree
        const int oldTreasure = currentPirate->treasure;
        auto treasureNode = currentShip->pirates_Treasure.find(oldTreasure);
        if (treasureNode) {
            auto treasureTree = *treasureNode;
            treasureTree->remove(pirateId);
            if (treasureTree->isEmpty()) {
                currentShip->pirates_Treasure.remove(oldTreasure);
            }
        }
        
        // Update treasure and add to new tree
        currentPirate->treasure += change;
        const int newTreasure = currentPirate->treasure;
        
        auto newTreasureNode = currentShip->pirates_Treasure.find(newTreasure);
        if (!newTreasureNode) {
            auto newTreasureTree = std::make_shared<AVL<int, std::shared_ptr<Pirate>>>();
            newTreasureTree->insert(pirateId, currentPirate);
            currentShip->pirates_Treasure.insert(newTreasure, newTreasureTree);
        } else {
            (*newTreasureNode)->insert(pirateId, currentPirate);
        }
        
        currentShip->updateRichestPirate();
        
    } catch (const std::bad_alloc&) {
        return StatusType::ALLOCATION_ERROR;
    }

    return StatusType::SUCCESS;
}

output_t<int> Ocean::get_treasure(int pirateId) {
    if (pirateId <= 0) {
        return StatusType::INVALID_INPUT;
    }

    try {
        auto pirateNode = Ocean_pirates.find(pirateId);
        if (!pirateNode) {
            return StatusType::FAILURE;
        }
       
        auto currentPirate = *pirateNode;
        auto shipNode = Ocean_ships.find(currentPirate->shipId);
        if (!shipNode) {
            return StatusType::FAILURE;
        }
        
        return currentPirate->getTreasure(*shipNode);
        
    } catch (const std::bad_alloc&) {
        return StatusType::ALLOCATION_ERROR;
    }
}

output_t<int> Ocean::get_cannons(int shipId) {
    if (shipId <= 0) {
        return StatusType::INVALID_INPUT;
    }

    try {
        auto shipNode = Ocean_ships.find(shipId);
        if (!shipNode) {
            return StatusType::FAILURE;
        }
        
        return (*shipNode)->cannons;
        
    } catch (const std::bad_alloc&) {
        return StatusType::ALLOCATION_ERROR;
    }
}

output_t<int> Ocean::get_richest_pirate(int shipId) {
    if (shipId <= 0) {
        return StatusType::INVALID_INPUT;
    }

    try {
        auto shipNode = Ocean_ships.find(shipId);
        if (!shipNode) {
            return StatusType::FAILURE;
        }
        
        auto currentShip = *shipNode;
        if (currentShip->numPirates <= 0) {
            return StatusType::FAILURE;
        }
        
        return currentShip->richestPirateId;
        
    } catch (const std::bad_alloc&) {
        return StatusType::ALLOCATION_ERROR;
    }
}

StatusType Ocean::ships_battle(int shipId1, int shipId2) {
    if (shipId1 <= 0 || shipId2 <= 0 || shipId1 == shipId2) {
        return StatusType::INVALID_INPUT;
    }

    try {
        auto ship1Node = Ocean_ships.find(shipId1);
        auto ship2Node = Ocean_ships.find(shipId2);
        
        if (!ship1Node || !ship2Node) {
            return StatusType::FAILURE;
        }

        auto ship1 = *ship1Node;
        auto ship2 = *ship2Node;

        const int power1 = std::min(ship1->cannons, ship1->numPirates);
        const int power2 = std::min(ship2->cannons, ship2->numPirates);

        if (power1 == power2) {
            return StatusType::SUCCESS; // Draw
        }

        if (power1 > power2) {
            ship1->extraTreasure += ship2->numPirates;
            ship2->extraTreasure -= ship1->numPirates;
        } else {
            ship2->extraTreasure += ship1->numPirates;
            ship1->extraTreasure -= ship2->numPirates;
        }

    } catch (const std::bad_alloc&) {
        return StatusType::ALLOCATION_ERROR;
    }

    return StatusType::SUCCESS;
}