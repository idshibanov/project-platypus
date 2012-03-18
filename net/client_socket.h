// Project Platypus
// client_socket.h - holds ClientSocketHandler class definition

#ifndef PLA_NET_CLIENT_SOCKET_H
#define PLA_NET_CLIENT_SOCKET_H

#include "socket.h"

class GameClient;
class NetPacket;

enum ClientStatus
{
    STATUS_CLIENT_INACTIVE,        // not connected
    STATUS_CLIENT_NOT_AUTH,        // just connected
    STATUS_CLIENT_AUTH,            // succesfully logged in
    STATUS_CLIENT_MAP,             // we are downloading game map
    STATUS_CLIENT_ACTIVE,          // we are playing
    STATUS_CLIENT_END              // always at the end
};

class ClientSocketHandler : public SocketHandler
{
    // status of a connection for protection reasons
    ClientStatus _status;
    GameClient* _gc;

public:

    ClientSocketHandler(int socket, GameClient* gc);   
    virtual ~ClientSocketHandler();
    bool HandlePacket(NetPacket* p);
    bool SendChatMsg(const char* msg);
    bool RecvChatMsg(NetPacket* p);
    bool RecvAck(NetPacket* p);
    bool SendMovement(unsigned int side);
    bool RecvMapData(NetPacket* p);
};

#endif /* PLA_NET_CLIENT_SOCKET_H */

