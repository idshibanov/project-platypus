// Project Platypus
// server_socket.cpp - implements ServerSocketArray and ServerSocketHandler classes

#include "../core/defines.h"
#include "../game/game.h"
#include "../server/server.h"
#include "server_socket.h"
#include <stdio.h>

ServerSocketHandler::ServerSocketHandler (int socket, GameServer* serv, GameInstance* game)
    : SocketHandler(socket)
{
    _status = STATUS_SERVER_INACTIVE;
    _serv = serv;
    _game = game;
}

ServerSocketHandler::~ServerSocketHandler()
{

}

bool ServerSocketHandler::HandlePacket(NetPacket* p)
{
    // DEBUG: it is not a null pointer
    assert(p != (NetPacket *)0);

    //printf("got %d bytes, packet %d\n", p->_size, p->_buffer[p->_pos]);
    bool retval = false;

    switch(p->_buffer[p->_pos++])
    {
    case PACKET_CLIENT_AUTHORIZE:
        if ( _status == STATUS_SERVER_INACTIVE )
        {
            retval = this->RecvClientLogin(p);
        }
        break;
    case PACKET_CLIENT_DISCONNECT:
        _serv->kill_client(_sockfd);
        retval = true;
        break;
    case PACKET_CLIENT_JOIN:
        if ( _status == STATUS_SERVER_AUTHORIZED )
        {
            _status = STATUS_SERVER_JOINING;
            retval = true;
        }
        break;
    case PACKET_CLIENT_JOIN_READY:
        if ( _status == STATUS_SERVER_JOINING )
        {
            _status = STATUS_SERVER_ACTIVE;
            retval = true;
        }
        break;
    case PACKET_CLIENT_MOVEMENT:
        retval = this->RecvClientMovement(p);
        break;
    case PACKET_CLIENT_CHAT:
        retval = this->RecvChatMsg(p);
        break;
    case PACKET_CLIENT_FILE:
        retval = this->RecvFile(p);
        break;    
    }

    return retval;
}

bool ServerSocketHandler::SendChatMsg(const char* msg)
{
    NetPacket* p = new NetPacket(PACKET_SERVER_CHAT);
    bool retval = false;

    if ( p->SendString(msg) )
    {
        retval = this->SendPacket(p);
    }

    // important
    delete p;

    return retval;
}

bool ServerSocketHandler::RecvChatMsg(NetPacket* p)
{
    // DEBUG: it is not a null pointer
    assert(p != (NetPacket *)0);

    bool retval = false;
    char* msg = new char[p->_size+1];

    // check if client is actually authorized to send this packet
    if (0 /*_status != STATUS_SERVER_ACTIVE */) { }
    else
    {
        if (p->RecvString(msg))
        {
            // got processed msg here
            string tmp(msg);
            _serv->broadcast( tmp.c_str(), _sockfd);
            _serv->log( tmp.insert( 0, "Client " ) );
            retval = true;
        } else
        {
            // printf("RecvString returned false\n");
        }
    }
    delete [] msg;
    return retval;
}

bool ServerSocketHandler::RecvAck(NetPacket* p)
{
    // DEBUG: it is not a null pointer
    assert(p != (NetPacket *)0);

    bool retval = false;

    return retval;
}

bool ServerSocketHandler::RecvClientLogin(NetPacket* p)
{
    // DEBUG: it is not a null pointer
    assert(p != (NetPacket *)0);

    bool retval = false;   
    char msg[p->_size];

    if (_status != STATUS_SERVER_INACTIVE ) { }
    else
    {
        p->RecvString(msg);

        // TODO: login system

        _status = STATUS_SERVER_AUTHORIZED;
        retval = true;
    }

    return retval;   
}

bool ServerSocketHandler::RecvClientMovement(NetPacket* p)
{
    // DEBUG: it is not a null pointer
    assert(p != (NetPacket *)0);

    bool retval = false;

    // TODO: proper handling

    unsigned int mvm = p->RecvUint();
    if(_game->MovePlayer(_sockfd, mvm))
    {
        //printf("Client %d moved in direction %d\n", _sockfd, mvm);
        SendAck(PACKET_SERVER_MOVE_RESPONSE, true);
        _serv->broadcast_movement(_sockfd);
        retval = true;
    } else
    {
        SendAck(PACKET_SERVER_MOVE_RESPONSE, false);
    }

    return retval;
}

bool ServerSocketHandler::SendMapData(int sockfd, Coords& coord)
{
    NetPacket* p = new NetPacket(PACKET_SERVER_MAPDATA);
    bool retval = false;

    if ( p->SendUint(sockfd) )
        if ( p->SendUint(coord.x) )
            if ( p->SendUint(coord.y) )
                retval = this->SendPacket(p);

    // important
    delete p;

    return retval;
}


// Implementation of ServerSocketArray class

ServerSocketArray::ServerSocketArray(GameServer* serv, GameInstance* game)
{
    _length = 0;
    _serv = serv;
    _game = game;
}

ServerSocketArray::~ServerSocketArray()
{
    int i;
    for(i = 0; i < _length && _client_sock[i]; i++)
        delete _client_sock[i];
}

bool ServerSocketArray::AddClient(int socket)
{
    // DEBUG: passed wrong socket #
    assert(socket >= MIN_CLIENT_SOCKFD);

    bool retval = false;

    if (socket < MIN_CLIENT_SOCKFD) { }
    else if ( _length < MAX_CLIENTS )
    {
        _client_sock[_length] = new ServerSocketHandler(socket, _serv, _game);
        _client_sock[_length]->SendAck(PACKET_SERVER_WELCOME);
        _length++;
        retval = true;
    } else 
    {
        ServerSocketHandler tmp(socket, _serv, _game);
        tmp.SendAck(PACKET_SERVER_FULL);
    }

    return retval;
}

bool ServerSocketArray::RemoveClient(int socket)
{
    // DEBUG: passed wrong socket #
    assert(socket >= MIN_CLIENT_SOCKFD);

    bool retval = false;
    int i;

    for(i = 0; i < _length && _client_sock[i]; i++)
    {
        if (_client_sock[i]->_sockfd == socket)
        {
            delete _client_sock[i];
            for(i += 1; i < _length && _client_sock[i]; i++)
                _client_sock[i-1] = _client_sock[i];

            _length--;
            retval = true;
            break;
        }
    }

    return retval;
}

void ServerSocketArray::ClearList()
{
    int i;
    for(i = 0; i < _length && _client_sock[i]; i++)
    {
        delete _client_sock[i];

        // so we cant delete memory again
        _client_sock[i] = (ServerSocketHandler*) 0;
    }

    _length = 0;
}

int ServerSocketArray::Length()
{
    return _length;
}

ServerSocketArray::operator int()
{
    return _length;
}

ServerSocketHandler* ServerSocketArray::GetClient (const int sockfd)
{
    ServerSocketHandler* retval = (ServerSocketHandler *)0;
    int i;

    for(i = 0; i < _length && _client_sock[i]; i++)
    {
        if (_client_sock[i]->_sockfd == sockfd)
            retval = (ServerSocketHandler *) _client_sock[i];
    }
    return retval;
}

ServerSocketHandler* ServerSocketArray::operator [] (const int sockfd)
{
    return GetClient(sockfd);
}

