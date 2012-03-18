// Project Platypus
// socket.h - holds SocketHandler base class definition, used by both server and client

#ifndef PLA_NET_SOCKET_H
#define PLA_NET_SOCKET_H

#include "../core/defines.h"
#include "../game/game.h"
#include "packet.h"

class NetPacket;

class SocketHandler
{
protected:

    // socket fd number
    int _sockfd;

    // pointer to a first packet in an queue
    NetPacket* _pfirst;

    // we cant initialize SocketHandler
    SocketHandler(int socket);

public:

    virtual ~SocketHandler();

    // returns socket number
    int GetSockNo();

    // we can cast a class to an int
    // returns socket number
    operator int();

    // add packet to a sending queue
    bool AddPacket(NetPacket* p);

    // send single packet to a client
    // must remove later ?
    bool SendPacket(NetPacket* p);

    // send packets in a queue
    bool SendPackets();

    // recieve packet from a client
    bool RecvPacket();

    // pure virtual function, calls specific function
    virtual bool HandlePacket(NetPacket *p) = 0;

    // SPECIFIC SEND\RECIEVE FUNCTIONS
    // send msg to a client, forms a packet & adds to a queue (?)
    virtual bool SendChatMsg(const char* msg) = 0;

    // get client msg from a recieved packet
    virtual bool RecvChatMsg(NetPacket* p) = 0;

    // send simple ack, optional bool argument
    bool SendAck(NetPacketType ack, bool val = false);

    // pure virtual function, get ack
    virtual bool RecvAck(NetPacket* p) = 0;

    // send file to a client, demo-function
    bool SendFile(const char* msg);

    // recover and print file from a packet, demo-function
    bool RecvFile(NetPacket* p);

};

#endif /* PLA_NET_SOCKET_H */
