// Project Platypus
// socket.cpp - implements SocketHandler base class, used by both server and client

#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "../core/defines.h"
#include "../core/datatypes.h"
#include "socket.h"

#include <stdio.h>
#include <iostream>
#include <string.h>

// Implementation of SocketHandler class

SocketHandler::SocketHandler(int socket)
{
    // constructor is private, so we assume that socket data is ok

    _sockfd = socket;
    _pfirst = (NetPacket *)0;
}

SocketHandler::~SocketHandler() { }

int SocketHandler::GetSockNo()
{
    return _sockfd;
}

SocketHandler::operator int()
{
    return _sockfd;
}

bool SocketHandler::AddPacket(NetPacket* p)
{
    if (_pfirst) {
        p->_next = _pfirst;
        // check & replace that
        while(p->_next->_next != (NetPacket *)0) p->_next = p->_next->_next;
    } else {
        _pfirst = p;
        p->_next = (NetPacket *)0;
    }
}

bool SocketHandler::SendPacket(NetPacket* p)
{
    bool retval = false;
    uint snd;

    if (p != (NetPacket *)0) {
        p->PrepareToSend();

        snd = send(_sockfd, (const void*) p->_buffer.str().data(), p->_size, 0);
        if ( snd == p->_size) {
            // EVENT: Packet sent succesfully
            retval = true;
        }
    }

    return retval;
}

bool SocketHandler::SendPackets()
{
    // TODO: implement it later
    return false;
}

bool SocketHandler::RecvPacket()
{
    bool retval = false;
    uint psize = sizeof(PacketSize);
    NetPacket* p = new NetPacket(this);

    int readsize;
    char newbuf[BUFSIZ+1];
    memset (newbuf,0,sizeof(newbuf));
    readsize = read(_sockfd, newbuf, BUFSIZ);
    p->_data.assign(newbuf);
    //std::cout << "GOT: " << newbuf << std::endl;
    if ( p->ReadRaw() ) {
        //std::cout << p->_data << std::endl;
        //std::cout << p->_pos << " pos, size: " << p->_size << std::endl;
        retval = this->HandlePacket(p);
    }

    // important
    delete p;

    return retval;
}

bool SocketHandler::SendAck(NetPacketType ack, bool val)
{
    NetPacket* p = new NetPacket(ack);
    bool retval = false;

    if ( p->SendBool(val) ) {
        retval = this->SendPacket(p);
    }

    // important
    delete p;

    return retval;
}


