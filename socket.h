// Project Platypus
// socket.h - holds ClientSocketArray and ClientSocketHandler class definitions

#ifndef PLA_NET_SOCKET_H
#define PLA_NET_SOCKET_H

#include "defines.h"
#include "packet.h"

class NetPacket;

enum ClientStatus
{
   STATUS_INACTIVE,        // client is not connected nor active
   STATUS_AUTH,            // client is authorizing with server
   STATUS_AUTHORIZED,      // client is authorized
   STATUS_ACTIVE,          // client is active within in the game
   STATUS_END              // always at the end
};

class ClientSocketHandler
{   
   // socket fd number
   int _sockfd;
   
   // pointer to a first packet in queue
   NetPacket* _pfirst;
   
   // status of a client for protection reasons
   ClientStatus _status;
   
   // private constructor, so only ClientSocketArray
   // can create an instance of ClientSocketHandler
   
   public:
   ClientSocketHandler(int socket);
   virtual ~ClientSocketHandler();
   
   // returns socket number
   int GetSockNo();
   
   // we can cast a class to an int
   // returns socket number
   operator int();
   
   // add packet to a sending queue
   bool AddPacket(NetPacket* p);
   
   // send single packet to a client
   // ? must remove later ?
   bool SendPacket(NetPacket* p);
   
   // send packets in a queue
   bool SendPackets();
   
   // recieve packet from a client
   bool RecvPacket();
   
   // calls specific function
   bool HandlePacket(NetPacket *p);
   
   // SPECIFIC SEND\RECIEVE FUNCTIONS
   // send msg to a client, forms a packet & adds to a queue (?)
   bool SendChatMsg(const char* msg);
   
   // get client msg from a recieved packet
   bool RecvChatMsg(NetPacket* p);
   
   // full access, can construct
   friend class ClientSocketArray;
};


class ClientSocketArray
{
   // array of ptrs to DYNAMICALLY allocated ClientSocketHandlers
   ClientSocketHandler* _client_sock[MAX_CLIENTS];
   
   // lenght of an array
   uint _lenght;
   
   public:
   // default constructor of an empty array
   ClientSocketArray();
   
   // deletes ClientSocketHandlers
   ~ClientSocketArray();
   
   // add a client to an array
   bool AddClient(int socket);
   
   // remove a client from an array, automatically moves others
   bool RemoveClient(int socket);
   
   // operator [] to work with this class as an simple array
   ClientSocketHandler* operator [] (const int sockfd);
};


#endif /* PLA_NET_SOCKET_H */


