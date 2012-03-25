// Project Platypus
// game.cpp - implements GameInstance class

#include "game.h"
#include <stdio.h>
using namespace std;

GameInstance::GameInstance(GameServer* serv, uint rows, uint cols)
                            : _map(rows, cols, 0)
{
    _serv = serv;
    _start_row = 1;
    _start_col = 1;
    _map.FillWith(CELL_FLOOR);
}

GameInstance::~GameInstance()
{

}

void GameInstance::AddPlayer(uint sockfd)
{
    uint x = _start_row;
    uint y = _start_col;
    uint x_max = _map.GetRows();
    uint y_max = _map.GetCols();
    
    while(!_map.IsEmptyCell(x,y)) {
        y++;
        if (y == y_max) {
            y = _start_col;
            x++;
        }
        if (x == x_max) {
            x = _start_row;
            break;
        }            
    }
    _players.push_back(GamePlayer(&_map, x, y, sockfd));
}

bool GameInstance::RemovePlayer(uint player_id)
{
    bool retval = false;
    vector<GamePlayer>::iterator vi;
    for ( vi = _players.begin(); vi < _players.end(); vi++ ) {
        if (vi->GetID() == player_id) {
            vi->CleanMap();
            _players.erase(vi);
            retval = true;
        }
    }
    return retval;
}

bool GameInstance::MovePlayer(uint player_id, uint direction)
{
    bool retval = false;
    vector<GamePlayer>::iterator vi;
    for ( vi = _players.begin(); vi < _players.end(); vi++ ) {
        if (vi->GetNetID() == player_id) {
            retval = vi->MoveChar(direction);
        }
    }
    return retval;
}

MapCoords GameInstance::GetPlayer(uint player_id)
{
    vector<GamePlayer>::iterator vi;
    for ( vi = _players.begin(); vi < _players.end(); vi++ ) {
        if (vi->GetNetID() == player_id) {
            uint row = vi->GetRow();
            uint col = vi->GetCol();
            return MapCoords(row,col,0);
        }
    }
}



