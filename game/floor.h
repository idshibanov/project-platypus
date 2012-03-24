// Project Platypus
// floor.h - holds MapFloor class definition

#ifndef PLA_GAME_FLOOR_H
#define PLA_GAME_FLOOR_H

#include "../core/datatypes.h"

enum MapCellObjectType {
    CELL_NOTHING,
    CELL_GROUND,
    CELL_HOLE,
    CELL_FLOOR,
    CELL_PILLAR,
    CELL_WALL,
    CELL_STAIRS_UP1,
    CELL_STAIRS_UP2,
    CELL_STAIRS_DOWN1,
    CELL_STAIRS_DOWN2
};

struct MapCell {
    uint _obj;
    uint _items;
    uint _player;
    MapCell(MapCellObjectType obj = CELL_NOTHING);
    MapCell(MapCellObjectType obj, uint item, uint player);
    ~MapCell();
};

class MapFloor {
    uint _lvl;
    uint _rows;
    uint _cols;
    MapCell* _data;
public:
    MapFloor(uint rows, uint cols, uint lvl);
    virtual ~MapFloor();
    uint GetRows();
    uint GetCols();
    void FillWith(MapCellObjectType obj); // not safe, remove later
    bool CellExists(uint row, uint col) const;
    bool IsEmptyCell(uint row, uint col) const;
    MapCell* GetCell (uint row, uint col);
    MapCell* GetPlayerCell (uint player_id);
    MapCell* operator() (uint row, uint col);
};


#endif
