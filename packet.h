// Project Platypus
// packet.h - holds NetPacket class definition

#ifndef PLA_NET_PACKET_H
#define PLA_NET_PACKET_H

#include "datatypes.h"
#include "socket.h"

class ClientSocketHandler;

typedef uint16 PacketSize; // for readability

enum NetPacketType
{
   PACKET_CLIENT_WELCOME,     // client sends username
   PACKET_SERVER_WELCOME,     // server responds with id
   PACKET_CLIENT_CHAT,        // client sends a message
   PACKET_SERVER_CHAT,        // server sends a message
   PACKET_END                 // always at the end
};

class NetPacket
{
   // ptr to associated socket, only for reading packets
   ClientSocketHandler* _read_sock;
   
   public:
   
   // public variables - so ClientSocketHandler can access them
   // we possibly can move everything to private and add a line:
   // friend class ClientSocketHandler;   
   
   // size of a packet in bytes (2 bytes itself)
   PacketSize _size;
   
   // current read\write position in the buffer
   PacketSize _pos;

   // pointer to DYNAMICALLY allocated data that actually will be send
   uchar* _buffer;
   
   // pointer used for queuing packets
   NetPacket* _next;
   
   const static uint _sizeof_sizetype = sizeof(PacketSize) + sizeof(uchar);


   // 2 constructors - for read and write packets
   NetPacket(ClientSocketHandler* sock);
   NetPacket(NetPacketType type);
   
   // deallocating buffer
   virtual ~NetPacket();
   
   // run before sending packet
   void PrepareToSend();
   void ReadSize();
   bool send_string(const char* data);
   bool recv_string(char* buf, uint16 size);
};

#endif /* PLA_NET_PACKET_H */
