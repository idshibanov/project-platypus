// Project Platypus
// packet.cpp - implements NetPacket class

#include "../core/defines.h"
#include "../core/bitmath.h"
#include "packet.h"

#include <stdio.h>
#include <stdlib.h>


NetPacket::NetPacket(SocketHandler* sock)
{
    // DEBUG: crash if NULL is passed (false evaluated)
    assert(sock != (SocketHandler *)0);

    _read_sock = sock;
    _next = (NetPacket *)0;
    _size = 0;
    _pos = 0;
}

NetPacket::NetPacket(NetPacketType type)
{
    _read_sock = (SocketHandler *)0;
    _next = (NetPacket *)0;
    _pos = 0;
    _buffer << type << ';';
    _size = _buffer.str().size();
}

NetPacket::~NetPacket()
{

}

void NetPacket::PrepareToSend()
{
    _pos = 0;

    std::stringstream ss (std::stringstream::in | std::stringstream::out);
    _size = _buffer.str().size();
    ss << 'S' << _size << ';';
    _size += ss.str().size();
    _buffer.str( ss.str() + _buffer.str() );
    
    //std::cout << _buffer.str() << std::endl;
}

bool NetPacket::ReadRaw()
{
    bool retval = false;

    if ( ReadSize() ) {
        if ( ReadType() ) {
            _data.assign(_data.substr(_pos));
            _size = _data.size();
            _pos = 0;
            retval = true;
        }
    }
    return retval;
}

bool NetPacket::ReadSize()
{
    bool retval = false;
    std::string tmpstr;
    
    _pos = _data.find(';');
    if (_pos == std::string::npos) {
        _size = 0;
    } else {
        tmpstr = _data.substr(0,_pos);
        if (*tmpstr.data() == 'S') {
            tmpstr.assign(tmpstr.substr(1, _pos));
            _size = atoi(tmpstr.data());
            retval = true;
        } else {
            _size = 0;
        }
    }
    
    return retval;
}

bool NetPacket::ReadType()
{
    bool retval = false;
    std::size_t found;
    std::string tmpstr;
    
    found = _data.find(';', _pos+1);
    if (found != std::string::npos) {
        _pos++; // to pass the ';' char
        tmpstr = _data.substr(_pos,found);
        _type = (NetPacketType) atoi(tmpstr.data());
        _pos = found;
        _pos++; // to pass the ';' char
        retval = true;
    }
    
    return retval;
}

bool NetPacket::SendBool(bool data)
{
    return SendUchar(data ? '1' : '0');
}

bool NetPacket::RecvBool()
{
    return RecvUchar() != '0';
}

bool NetPacket::SendUchar(uchar data)
{
    bool retval = false;

    // return false if we exceed max packet size
    if (_size + sizeof(data) > MAX_PACKET_SIZE) {
    } else {
        _buffer << data << ";";
        _size = _buffer.str().size();
        retval = true;
    }

    return retval;
}

uchar NetPacket::RecvUchar()
{
    uchar retval = 0;
    if (_pos < _size) {
        retval = *(_data.data() + _pos);
        _pos = _data.find(';', _pos + 1) + 1;
        //printf("Got: %c, pos: %d\n", retval, _pos);
    }

    return retval;
}

bool NetPacket::SendUint(uint data)
{
    bool retval = false;

    // return false if we exceed max packet size
    if (_size + sizeof(data) > MAX_PACKET_SIZE) {
    } else {
        _buffer << data << ";";
        _size = _buffer.str().size();
        retval = true;
    }

    return retval;
}

uint NetPacket::RecvUint()
{
    uint retval = 0;
    std::size_t found;
    
    if (_pos < _size) {
        found = _data.find(';', _pos + 1);
    
        retval = atoi(_data.substr(_pos,found).data());
        _pos = found+1;
        //printf("Got: %d, pos: %d\n", retval, _pos);
    }

    return retval;
}

bool NetPacket::SendString(const char* data)
{
    // DEBUG: check if there is data
    assert(data != (const char*)0);

    bool retval = false;


    if (_size + sizeof(data) > MAX_PACKET_SIZE) {
    } else {
        _buffer << data;
        //str.erase(str.find_last_not_of(" \n\r\t")+1);
        _buffer << ";";
        _size = _buffer.str().size();
        retval = true;
    }

    return retval;
}

bool NetPacket::RecvString(char* buf)
{
    // DEBUG: passed string is not null
    assert(buf != (char *)0);

    // DEBUG: ensure that we are reading, not writing
    assert(_read_sock != (SocketHandler *)0);

    bool retval = false;
    std::size_t found;
    
    if (_pos < _size) {
        found = _data.find(';', _pos + 1);
    
        //printf("Pos: %d, Found: %d\n", _pos, found);
        _data.copy(buf, found-_pos, _pos);
        buf[found-_pos]='\0';
        _pos = found+1;
        //printf("Got: %s, pos: %d\n", buf, _pos);
        
        retval = true;
    }

    return retval;
}

