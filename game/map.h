// Project Platypus
// floor.h - holds MapFloor class definition

#ifndef PLA_GAME_MAP_H
#define PLA_GAME_MAP_H

#include "../core/defines.h"
#include "floor.h"

class GameMap {
    MapFloor* _floor;
public:
    GameMap(uint rows, uint cols, uint lvls = 1);
    virtual ~GameMap();
}

#endif
