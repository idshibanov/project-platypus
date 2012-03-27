// Project Platypus
// client_socket.cpp - implements ClientSocketHandler class

#include <stdio.h>
#include <string>
#include <sys/select.h>

#include "../ncurses/game/GameScreen.h"
#include "client_socket.h"

ClientSocketHandler::ClientSocketHandler(int socket, GameScreen* gs) : SocketHandler(socket)
{
   _status = STATUS_CLIENT_CONNECTED;
   _gs = gs;
}

ClientSocketHandler::~ClientSocketHandler()
{

}

bool ClientSocketHandler::HandlePacket(NetPacket* p)
{
    // DEBUG: it is not a null pointer
    assert(p != (NetPacket *)0);

    // printf("got %d bytes, packet %d\n", p->_size, p->_buffer[p->_pos]);

    bool retval = false;

    switch(p->_buffer[p->_pos++]) {
    
    case PACKET_SERVER_FULL:
        if (_status == STATUS_CLIENT_OFFLINE ) {
            // printf("Sorry, server is full\n");            
            // TODO: status bar in the menu, disconnect
            retval = true;
        }
        break;
    case PACKET_SERVER_WELCOME:
        if (_status == STATUS_CLIENT_OFFLINE ) {
            // temp replace
            //_status = STATUS_CLIENT_NOT_AUTH;

            _status = STATUS_CLIENT_CONNECTED;

            // TODO: ask user to send his username
            retval = true;
        }
        break;
    case PACKET_SERVER_SHUTDOWN:
        // TODO: disconnect
        retval = true;
        break;
    case PACKET_SERVER_AUTH_RESPONSE:
        if (_status == STATUS_CLIENT_CONNECTED ) {
            if (p->RecvBool()) {
                //_status = STATUS_CLIENT_AUTHORIZED;
                _status = STATUS_CLIENT_GAME_ACTIVE;
            } else {
                // TODO: relogin
            }
            retval = true;
        }      
        break;
    case PACKET_SERVER_REG_STATUS:
        if (_status == STATUS_CLIENT_OFFLINE ) {
            if (p->RecvBool()) {
                _status = STATUS_CLIENT_REGISTRATION;
                // TODO: let user register
            } else {
                // TODO: sorry message in status bar
            }
            retval = true;
        }      
        break;
    case PACKET_SERVER_REG_RESPONSE:
        if (_status == STATUS_CLIENT_REGISTRATION ) {
            if (p->RecvBool()) {
                _status = STATUS_CLIENT_OFFLINE;
                // TODO: success action, get login form?
            } else {
                // TODO: register again
            }
            retval = true;
        }      
        break;
    case PACKET_SERVER_JOIN_DATA:
        if (_status == STATUS_CLIENT_AUTHORIZED ) {
            // function will update server status itself
            //retval = this->RecvClientList(p);
        }
        break;
    case PACKET_SERVER_JOIN_STARTSIN:
        if (_status == STATUS_CLIENT_JOINED )
            _status = STATUS_CLIENT_GAME_STARTED;
            
        if (_status == STATUS_CLIENT_GAME_STARTED ) {
            // TODO: update "Game starts in ..." counter
            //retval = this->RecvStartCount(p);
        }
        break;
    case PACKET_SERVER_JOIN_MAPFIRST:
        if (_status == STATUS_CLIENT_GAME_STARTED ) {
            //retval = this->RecvMapStruct(p, PACKET_SERVER_JOIN_MAPFIRST);
            _status = STATUS_CLIENT_GAME_INIT;
        }
        break;
    case PACKET_SERVER_JOIN_MAPDATA:
        if (_status == STATUS_CLIENT_GAME_INIT ) {
            //retval = this->RecvMapStruct(p, PACKET_SERVER_JOIN_MAPDATA);
        }
        break;
    case PACKET_SERVER_JOIN_MAPLAST:
        if (_status == STATUS_CLIENT_GAME_INIT ) {
            // function will update server status itself
            //retval = this->RecvMapStruct(p, PACKET_SERVER_JOIN_MAPLAST);
        }
        break;
    case PACKET_SERVER_CHARDATA:
        if (_status == STATUS_CLIENT_GAME_ACTIVE ) {
            // EVENT: another character moved
            retval = this->RecvCharData(p);
         }
         break;
    case PACKET_SERVER_MAPDATA:
        if (_status == STATUS_CLIENT_GAME_ACTIVE ) {
            // EVENT: something happend on the map
            // retval = this->RecvMapData(p);
         }
         break;
    case PACKET_SERVER_MOVE_RESPONSE:
        if (_status == STATUS_CLIENT_GAME_ACTIVE )
        {
            if (p->RecvBool())
            {
               // EVENT: recieved true on request
               retval = RecvMovement(p);
            }
        }
        break;
    case PACKET_SERVER_CHAT:
        // should be = STATUS_CLIENT_GAME_ACTIVE or JOINED
        if (_status == STATUS_CLIENT_GAME_ACTIVE ) { 
            retval = this->RecvChatMsg(p);
        }         
        break;
    case PACKET_SERVER_FILE:
        retval = this->RecvFile(p);
        break;      
    }

    return retval;
}

bool ClientSocketHandler::SendChatMsg(const char* msg)
{
    NetPacket* p = new NetPacket(PACKET_CLIENT_CHAT);
    bool retval = false;

    if ( p->SendString(msg) ) {
        retval = this->SendPacket(p);
    }

    // important
    delete p;

    return retval;
}

bool ClientSocketHandler::RecvChatMsg(NetPacket* p)
{
    // DEBUG: it is not a null pointer
    assert(p != (NetPacket *)0);

    bool retval = false;
    char* msg = new char[p->_size+1];

    // check if client is actually authorized to send this packet
    if (0 /*_status != STATUS_CLIENT_ACTIVE */) { }
    else {
        if (p->RecvString(msg)) {
            // printf("RecvString passed\n");

            // got processed msg here
            _gs->addMessage(msg);

        } else {
            // printf("RecvString returned false\n");
        }


        retval = true;
    }
    delete [] msg;
    return retval;
}

bool ClientSocketHandler::SendAuthRequest(std::string usr, std::string pwd)
{
    NetPacket* p = new NetPacket(PACKET_CLIENT_AUTH_REQUEST);
    bool retval = false;

    if ( p->SendString(usr.c_str()) ) {
        if ( p->SendString(pwd.c_str()) ) {
            retval = this->SendPacket(p);
        }
    }

    // important
    delete p;

    return retval;
}

bool ClientSocketHandler::RecvAck(NetPacket* p)
{
    // DEBUG: it is not a null pointer
    assert(p != (NetPacket *)0);

    bool retval = p->RecvBool();

    return retval;
}


bool ClientSocketHandler::SendMovement(unsigned int side)
{
    NetPacket* p = new NetPacket(PACKET_CLIENT_MOVE_REQUEST);
    bool retval = false;

    if ( p->SendUint(side) ) {
        retval = this->SendPacket(p);
    }

    // important
    delete p;

    return retval;
}

bool ClientSocketHandler::RecvMovement(NetPacket* p)
{
    // DEBUG: it is not a null pointer
    assert(p != (NetPacket *)0);

    bool retval = false;
    
    unsigned int x = p->RecvUint();
    unsigned int y = p->RecvUint();    
    _gs->move_char(x, y);

    return retval;
}

bool ClientSocketHandler::RecvCharData(NetPacket* p)
{
    // DEBUG: it is not a null pointer
    assert(p != (NetPacket *)0);

    bool retval = false;

    unsigned int id = p->RecvUint();
    unsigned int x = p->RecvUint();
    unsigned int y = p->RecvUint();

    _gs->set_char(id, x, y);

    return retval;
}


