// Project Platypus
// map.cpp - implements GameMap class

#include "map.h"

GameMap::GameMap(uint rows, uint cols, uint lvls)
{
    _floor = new MapFloor*[z];
    for (int i = 0; i < lvls; i++) {
        _floor[i] = new MapFloor(rows,cols,i);
    }
}

GameMap::~GameMap()
{
    delete [] _floor;
}
