// Project Platypus
// packet.h - holds NetPacket class definition

#ifndef PLA_NET_PACKET_H
#define PLA_NET_PACKET_H

#include "../core/datatypes.h"
#include <iostream>
#include <sstream>

class SocketHandler;

typedef uint16 PacketSize; // for readability

enum NetPacketType
{
    // FORMAT: "packet" - who sends it - purpose

    PACKET_SERVER_WELCOME,        // server welcomes client
    PACKET_SERVER_FULL,           // server rejects client (server is full)
    PACKET_CLIENT_CONNECT,        // client connects

    PACKET_SERVER_SHUTDOWN,       // server exits
    PACKET_CLIENT_DISCONNECT,     // client exits

    PACKET_SERVER_KICK,           // server kicks client
    PACKET_SERVER_BAN,            // server bans client

    PACKET_SERVER_ERROR,          // server responds if got wrong packet
    PACKET_CLIENT_ERROR,          // client responds if got wrong packet

    // login/registration
    PACKET_CLIENT_AUTH_REQUEST,   // client sends login information
    PACKET_SERVER_AUTH_RESPONSE,  // server responds with passed/failed
    PACKET_CLIENT_REG_CONNECT,    // client asks registration status (allowed/denied?)
    PACKET_SERVER_REG_STATUS,     // server responds with T/F
    PACKET_CLIENT_REG_REQUEST,    // client sends account information
    PACKET_SERVER_REG_RESPONSE,   // server responds with T/F

    // intermission phase
    PACKET_CLIENT_JOIN_ROOM,      // client asks to join game
    PACKET_SERVER_JOIN_DATA,      // server sends client list
    PACKET_CLIENT_JOIN_READY,     // client ready to start
    PACKET_SERVER_JOIN_STARTSIN,  // server sends "Game starts in ..."
    PACKET_SERVER_JOIN_MAPFIRST,  // server starting the game
    PACKET_SERVER_JOIN_MAPDATA,   // server sends initial map
    PACKET_SERVER_JOIN_MAPLAST,   // server sends error checking packet
    PACKET_CLIENT_JOIN_INIT,      // client is active

    // action phase
    PACKET_SERVER_CHARDATA,       // char sync
    PACKET_SERVER_MAPDATA,        // map sync
    PACKET_CLIENT_MOVE_REQUEST,   // client tries to move
    PACKET_SERVER_MOVE_RESPONSE,  // server allows/denies

    PACKET_CLIENT_CHAT,           // client sends a message
    PACKET_SERVER_CHAT,           // server sends a message

    PACKET_CLIENT_FILE,           // client sends a file
    PACKET_SERVER_FILE,           // server sends a file

    PACKET_END                    // always at the end
};

class NetPacket
{
    // ptr to associated socket, only for reading packets
    SocketHandler* _read_sock;

    // size of a packet in bytes (2 bytes itself)
    PacketSize _size;

    // current read\write position in the buffer
    PacketSize _pos;

    // pointer to DYNAMICALLY allocated data that actually will be send
    std::stringstream _buffer;

    // pointer used for queuing packets
    NetPacket* _next;

    // it always must be exactly 3 bytes, but.. everything happens
    const static uint _sizeof_sizetype = sizeof(PacketSize) + sizeof(uchar);

    // 2 constructors - for read/write packets
    NetPacket(SocketHandler* sock);
    NetPacket(NetPacketType type);

    // deallocating buffer
    virtual ~NetPacket();

    // run before sending packet
    void PrepareToSend();

    // get raw size from buffer while reading
    void ReadSize();

    // send\recieve functions, to fill\extract data from packets
    bool SendBool(bool data);
    bool RecvBool();
    bool SendUchar(uchar data);
    uchar RecvUchar();
    bool SendUint(uint data);
    uint RecvUint();
    bool SendString(const char* data);
    bool RecvString(char* buf);

    // SocketHandler owns it and can access data directly
    friend class SocketHandler;

    // HACK - friendship is not inherited
    friend class ClientSocketHandler;
    friend class ServerSocketHandler;
};

#endif /* PLA_NET_PACKET_H */
