// Project Platypus
// server.h - holds GameServer class definition

#ifndef PLA_SERVER_H
#define PLA_SERVER_H

#include "game.h"
#include "./net/server_socket.h"
#include <sys/select.h>

class GameServer 
{  
   fd_set _readfds, _testfds;
   int _port;
   int _serv_fd;
   int _min_client_fd;
   int _max_client_fd;
   bool _connected;
   
   GameInstance* _game;              // DYNAMICALLY allocated, holds game data
   ServerSocketArray* _client_sock;  // DYNAMICALLY holds our clients
   
   public:
   
   // default & non-default constructor
   GameServer(int port = SERV_PORT);
   
   ~GameServer();
   
   bool start_service();
   
   // private:
   
   void stop_service();
   bool run_select();
   bool accept_client();
   bool kick_client(int fd);
   bool kill_client(int fd);
   bool parse_cmd();
   void broadcast(const char* str, int fd = -1);
};

#endif /* PLA_NET_SERVER_SOCKET_H */
