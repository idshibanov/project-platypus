// Project Platypus
// game.cpp - implements GameInstance class

#include "game.h"
using namespace std;

GameInstance::GameInstance(GameServer* serv, uint rows, uint cols)
                            : _map(rows, cols)
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
    x = _start_row;
    y = _start_col;
    x_max = _map.GetRows();
    y_max = _map.GetCols();

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
    _players.push_back(GamePlayer(_map, x, y, sockfd));
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
        if (vi->GetID() == player_id) {
            vi->MoveChar(direction);
        }
    }
    return retval;
}

MapCoords GameInstance::GetPlayer(uint player_id)
{

}

GamePlayer* GameInstance::GetPlayer(uint player_id)
{
    vector<GamePlayer>::iterator vi;
    for ( vi = _players.begin(); vi < _players.end(); vi++ ) {
        if (vi->GetID() == player_id) {
            return vi;
        }
    }
    return (GamePlayer*)0;
}

