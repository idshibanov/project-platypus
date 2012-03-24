// Project Platypus
// game.h - holds GameInstance class definition

#ifndef PLA_GAME_H
#define PLA_GAME_H

#include "../core/datatypes.h"
#include "../core/defines.h"
#include "floor.h"
#include "player.h"
#include <vector>

class GameServer;

struct MapCoords {
    uint _row;
    uint _col;
    uint _lvl;
}

class GameInstance {
    // TODO: multilevel map (using GameMap class instead of Floor)
    Floor _map;
    GameServer* _serv;
    std::vector<GamePlayer> _players;
    uint _start_row;
    uint _start_col;

public:

    GameInstance(GameServer* serv, uint rows, uint cols);
    ~GameInstance();
    void AddPlayer(uint sockfd = 0);
    bool RemovePlayer(uint player_id);
    bool MovePlayer(uint player_id, uint direction);
    MapCoords GetPlayerCoords(uint player_id);
    GamePlayer* GetPlayer(uint player_id);
};

#endif /* PLA_GAME_H */
