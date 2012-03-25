// Project Platypus
// player.cpp - implements GamePlayer and GameNPC classes

#include "../core/defines.h"
#include "player.h"
#include "floor.h"

#include <stdio.h>

uint GameCharacter::_id_count = 1;

GameCharacter::GameCharacter(MapFloor* lvl, uint row, uint col)
{
    assert(lvl != (MapFloor*)0);
    _floor = lvl;
    
    assert(row <= _floor->GetRows());
    assert(col <= _floor->GetCols());
    assert(_floor->GetCell(row, col)->_obj == CELL_FLOOR);
    
    _row = row;
    _col = col;
    _char_id = _id_count++;
}

GameCharacter::GameCharacter(MapFloor* lvl, uint row, uint col, uint char_id)
{
    assert(lvl != (MapFloor*)0);
    _floor = lvl;
    
    assert(row <= _floor->GetRows());
    assert(col <= _floor->GetCols());
    assert(_floor->GetCell(row, col)->_obj == CELL_FLOOR);
    assert(char_id > _id_count);
    
    _row = row;
    _col = col;
    _char_id = char_id;
    _floor->GetCell(_row, _col)->_player = _char_id;
}

GameCharacter::~GameCharacter()
{

}

uint GameCharacter::GetID()
{
    return _char_id;
}

uint GameCharacter::GetRow()
{
    return _row;
}

uint GameCharacter::GetCol()
{
    return _col;
}

bool GameCharacter::MoveChar(uint direction)
{
    bool retval = false;
    switch (direction) {
    case 1:
        retval = MoveChar(_row+1, _col-1);
        break;
    case 2:
        retval = MoveChar(_row+1, _col);
        break;
    case 3:
        retval = MoveChar(_row+1, _col+1);
        break;
    case 4:
        retval = MoveChar(_row, _col-1);
        break;
    case 6:
        retval = MoveChar(_row, _col+1);
        break;
    case 7:
        retval = MoveChar(_row-1, _col-1);
        break;
    case 8:
        retval = MoveChar(_row-1, _col);
        break;
    case 9:
        retval = MoveChar(_row-1, _col+1);
        break;
    }
    return retval;
}

bool GameCharacter::MoveChar(uint row, uint col)
{
    bool retval = false;
    if (_floor->CellExists(row, col)) {
        MapCell* next = _floor->GetCell(row, col);
        if (next->_obj == CELL_FLOOR && next->_player == 0) {
            _floor->GetCell(_row, _col)->_player = 0;
            _row = row;
            _col = col;
            next->_player = _char_id;
            printf("Cell %d %d is busy by %d\n", _col, _row, _char_id);
            retval = true;
        }
    }
    return retval;
}

void GameCharacter::CleanMap()
{
    _floor->GetCell(_row, _col)->_player = 0;
}

// GamePlayer class implementation

GamePlayer::GamePlayer(MapFloor* lvl, uint row, uint col, uint sock_id)
                        : GameCharacter(lvl, row, col)
{
    _sock_id = sock_id;
}

GamePlayer::GamePlayer(MapFloor* lvl, uint row, uint col, uint sock_id, uint char_id )
                        : GameCharacter(lvl, row, col, char_id)
{
    _sock_id = sock_id;
}

GamePlayer::~GamePlayer()
{

}

uint GamePlayer::GetNetID()
{
    return _sock_id;
}

// GameNPC class implementation
    
GameNPC::GameNPC(MapFloor* lvl, uint row, uint col, uint type)
                        : GameCharacter(lvl, row, col)
{
    _type = type;
}

GameNPC::GameNPC(MapFloor* lvl, uint row, uint col, uint type, uint char_id )
                        : GameCharacter(lvl, row, col, char_id)
{
    _type = type;
}

GameNPC::~GameNPC()
{

}


