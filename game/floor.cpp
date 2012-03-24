// Project Platypus
// floor.cpp - implements MapFloor class

#include "../core/defines.h"
#include "floor.h"

MapCell::MapCell(MapCellObjectType obj)
{
    _obj = obj;
    _items = 0;
    _player = 0;
}

MapCell::MapCell(MapCellObjectType obj, uint item, uint player)
{
    _obj = obj;
    if (obj == CELL_FLOOR) {
        _items = item;
        _player = player;
    } else {
        _items = 0;
        _player = 0;
    }
}

MapCell::~MapCell()
{

}

// MapFloor class implementation

MapFloor::MapFloor(uint rows, uint cols, uint lvl)
{
    _rows = rows;
    _cols = cols;
    _lvl = lvl;
    _data = new MapCell[rows*cols];
}

MapFloor::~MapFloor()
{
    delete [] _data;
}

uint MapFloor::GetRows()
{
    return _rows;
}

uint MapFloor::GetCols()
{
    return _cols;
}

void MapFloor::FillWith(MapCellObjectType obj)
{
    // TODO: remove function, can corrupt map if used after init
    for(int i = 0; i < _rows*_cols; i++)
        _data[i]._obj = obj;
}

bool MapFloor::CellExists(uint row, uint col) const
{
    bool retval = true;
    if (row > _row || col > _col)
        retval = false;
    return retval;
}

bool MapFloor::IsEmptyCell(uint row, uint col) const
{
    bool retval = false;
    
    // foolproof, you must check first if Cell exists
    assert(row <= _rows && col <= _cols);
    
    MapCell* next = _data[row*col+col];
    if (next->_obj == CELL_FLOOR && next->_player == 0) {
        retval = true;
    }
     
    return retval;
}

MapCell* MapFloor::GetCell (uint row, uint col)
{
    if (row >= _rows || col >= _cols)
        return 0;
    return &_data[_cols*row + col];
}

MapCell* MapFloor::GetPlayerCell (uint player_id)
{
    MapCell* retval = 0;
    
    for (int i = 0; i < _rows * _cols; i++) {
        if (_data[i]._player == player_id) {
            retval = &_data[i];
            break;
        }
    }    
    return retval;
}

MapCell* MapFloor::operator() (uint row, uint col)
{
    if (row >= _rows || col >= _cols)
        return 0;
    return &_data[_cols*row + col];
}

