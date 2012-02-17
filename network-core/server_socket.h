// Project Platypus
// server_socket.h - holds ServerSocketArray and ServerSocketHandler class definitions

#ifndef PLA_NET_SERVER_SOCKET_H
#define PLA_NET_SERVER_SOCKET_H

#include "socket.h"

class NetPacket;

enum ServerStatus
{
   STATUS_SERVER_INACTIVE,        // client is just connected and not authorized
   STATUS_SERVER_AUTHORIZED,      // client is authorized
   STATUS_SERVER_JOINING,         // client tries to join
   STATUS_SERVER_ACTIVE,          // client is active in the game
   STATUS_SERVER_END              // always at the end
};

class ServerSocketHandler : public SocketHandler
{
   // status of a client for protection reasons
   ServerStatus _status;
   
   // client is associated with this game instance
   GameInstance* _game;

   // private constructor, so only ServerSocketArray
   // can create an instance of ServerSocketHandler

   ServerSocketHandler(int socket, GameInstance* game);
   
   public:
   
   virtual ~ServerSocketHandler();   
   bool HandlePacket(NetPacket *p);
   bool RecvAck(NetPacket* p);
   bool SendChatMsg(const char* msg);
   bool RecvChatMsg(NetPacket* p);
   bool RecvClientLogin(NetPacket* p);
   
   friend class ServerSocketArray;
};

class ServerSocketArray
{
   // array of ptrs to DYNAMICALLY allocated SocketHandlers
   ServerSocketHandler* _client_sock[MAX_CLIENTS];

   // lenght of an array
   uint _lenght;
   
   // array is associated with this game instance
   GameInstance* _game;

   public:
   // default constructor of an empty array
   ServerSocketArray(GameInstance* game);

   // deletes SocketHandlers
   ~ServerSocketArray();

   // add a client to an array
   bool AddClient(int socket);

   // remove a client from an array, automatically moves others
   bool RemoveClient(int socket);

   // returns current number of clients in the array
   int Lenght();

   // we can cast a class to an int
   // returns current number of clients in the array
   operator int();

   // operator [] to work with this class as an simple array
   ServerSocketHandler* operator [] (const int sockfd);
};


#endif /* PLA_NET_SERVER_SOCKET_H */

