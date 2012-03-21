// Project Platypus
// client_socket.h - holds ClientSocketHandler class definition

#ifndef PLA_NET_CLIENT_SOCKET_H
#define PLA_NET_CLIENT_SOCKET_H

#include "socket.h"
#include <string>

class GameScreen;
class NetPacket;

enum ClientStatus
{
    STATUS_CLIENT_OFFLINE,        // initial state
    STATUS_CLIENT_REGISTRATION,   // trying to register
    STATUS_CLIENT_CONNECTED,      // just connected
    STATUS_CLIENT_AUTHORIZED,     // succesfully logged in
    STATUS_CLIENT_JOINED,         // joined game lobby (intermission)
    STATUS_CLIENT_GAME_STARTED,   // game is starting
    STATUS_CLIENT_GAME_INIT,      // downloading game map data
    STATUS_CLIENT_GAME_READY,     // ready to go
    STATUS_CLIENT_GAME_ACTIVE,    // playing
    STATUS_CLIENT_END             // always at the end
};

class ClientSocketHandler : public SocketHandler
{
   // status of a connection for protection reasons
   ClientStatus _status;
   GameScreen* _gs;
  
   public:
   
   ClientSocketHandler(int socket, GameScreen* gs);   
   virtual ~ClientSocketHandler();
   bool HandlePacket(NetPacket* p);
   bool SendChatMsg(const char* msg);
   bool RecvChatMsg(NetPacket* p);
   bool RecvAck(NetPacket* p);
   bool SendMovement(unsigned int side);
   bool RecvMovement(NetPacket* p);
   bool SendAuthRequest(std::string usr, std::string pwd);
   bool RecvCharData(NetPacket* p);
};

#endif /* PLA_NET_CLIENT_SOCKET_H */

