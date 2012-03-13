// Project Platypus
// game.h - holds GameInstance class definition

#ifndef PLA_GAME_H
#define PLA_GAME_H

#include "../core/defines.h"

// RAW draft of a GameInstance class, only for movement testing purposes

class GameServer;

struct Coords {
   unsigned int x, y;
   Coords(unsigned int fst = 2, unsigned int snd = 2)
   {
      x = fst;
      y = snd;
   }
};

struct GameCharacter {
	int _x, _y;
	int _sockfd;
	GameCharacter(int sockfd)
	{
	   _sockfd = sockfd;
	   _x = _y = 2;
	}
	
   bool MovePlayer(unsigned int direction)
   {
      bool retval = false;
      switch(direction)
      {
      case 2:
         if ( _y < 5 ) {
            _y++;
            retval = true;
         }
         break;
      case 4:
         if ( _x > 2 ) {
            _x--;
            retval = true;
         }         
         break;
      case 6:
         if ( _x < 78) {
            _x++;
            retval = true;
         }
         break;
      case 8:
         if ( _y > 1 ) {
            _y--;
            retval = true;
         }
         break;
      }
      
      return retval;
   }	
};

class GameInstance
{
   // holds all game data (map, chars, npc, etc)
   
   int _map[7][80];
   GameCharacter* _players[MAX_CLIENTS];
   GameServer* _serv;
   int _length;
   
   public:
   
   // and main functions
   
   GameInstance(GameServer* serv)
   {
      _length = 0;
      _serv = serv;
   }
   
   ~GameInstance()
   {
      int i;
      for(i = 0; i < _length && _players[i]; i++)
         delete _players[i];
   }
   
   void AddPlayer(int sockfd)
   {
      if (_length < MAX_CLIENTS)
         _players[_length++] = new GameCharacter(sockfd);
   }
   
   bool RemovePlayer(int sockfd)
   {
      bool retval = false;
      int i;
   
      for(i = 0; i < _length && _players[i]; i++)
      {
         if (_players[i]->_sockfd == sockfd)
         {
            delete _players[i];
            for(i += 1; i < _length && _players[i]; i++)
               _players[i-1] = _players[i];

            _length--;
            retval = true;
            break;
         }
      }

      return retval;
   }
   
   bool MovePlayer(int sockfd, unsigned int direction)
   {
      bool retval = false;
      for(int i = 0;  i < _length; i++)
      {
         if(_players[i]->_sockfd == sockfd)
         {
            retval = _players[i]->MovePlayer(direction);
            break;
         }
      }
      return retval;
   }
   
   Coords GetPlayer(int sockfd)
   {
      for(int i = 0; i < _length; i++)
      {
         if(_players[i]->_sockfd == sockfd)
            return Coords(_players[i]->_x, _players[i]->_y);
      }
   }
};

#endif /* PLA_GAME_H */
