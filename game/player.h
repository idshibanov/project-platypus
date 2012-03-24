// Project Platypus
// player.h - holds GamePlayer and GameNPC class definitions

#ifndef PLA_GAME_PLAYER_H
#define PLA_GAME_PLAYER_H

#include "../core/datatypes.h"
#include "floor.h"

class GameCharacter {
protected:
    uint _char_id;
    uint _row;
    uint _col;
    MapFloor* _floor;
    GameCharacter(MapFloor* lvl, uint row, uint col);
    GameCharacter(MapFloor* lvl, uint row, uint col, uint char_id);
public:
    static uint _id_count;
    virtual ~GameCharacter();
    uint GetID();
    bool MoveChar(uint direction);
    bool MoveChar(uint row, uint col);
    void CleanMap();
};

class GamePlayer : public GameCharacter {
    uint _sock_id;
public:
    GamePlayer(MapFloor* lvl, uint row, uint col, uint sock_id);
    GamePlayer(MapFloor* lvl, uint row, uint col, uint sock_id, uint char_id );
    virtual ~GamePlayer();
    
};

class GameNPC : public GameCharacter {
    uint _type;
public:
    GameNPC(MapFloor* lvl, uint row, uint col, uint type);
    GameNPC(MapFloor* lvl, uint row, uint col, uint type, uint char_id );
    virtual ~GameNPC();
    
};

#endif
