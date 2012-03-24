// Project Platypus
// player.cpp - implements GamePlayer and GameNPC classes

#include "../core/defines.h"
#include "player.h"
#include "floor.h"

uint GameCharacter::_id_count = 0;

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
}

GameCharacter::~GameCharacter()
{

}

uint GameCharacter::GetID()
{
    return _char_id;
}

bool GameCharacter::MoveChar(uint direction)
{
    bool retval = false;
    switch (direction) {
    case 1:
        retval = MoveChar(row+1, col-1);
        break;
    case 2:
        retval = MoveChar(row+1, col);
        break;
    case 3:
        retval = MoveChar(row+1, col+1);
        break;
    case 4:
        retval = MoveChar(row, col-1);
        break;
    case 6:
        retval = MoveChar(row, col+1);
        break;
    case 7:
        retval = MoveChar(row-1, col-1);
        break;
    case 8:
        retval = MoveChar(row-1, col);
        break;
    case 9:
        retval = MoveChar(row-1, col+1);
        break;
    }
    return false;
}

bool GameCharacter::MoveChar(uint row, uint col)
{
    bool retval = false;
    if (_floor->CellExists(row, col)) {
        MapCell* next = _floor->GetCell(row, col);
        if (next->_obj == CELL_FLOOR && next->_player == 0) {
            _row = row;
            _col = col;
            next->_player = _char_id;
            retval = true;
        }
    }
    return retval;
}

void GameCharacter::CleanMap()
{
    _floor->GetCell(row, col)->_player = 0;
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


