// Project Platypus
// packet.cpp - implements NetPacket class

#include "../core/defines.h"
#include "../core/bitmath.h"
#include "packet.h"


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
    std::cout << "1Size: " << _size << std::endl;
    ss << _size << ';';
    _size += ss.str().size();
    std::cout << "2Size: " << _size << std::endl;
    ss.str("");
    ss << _size << ';';
    _buffer.str( ss.str() + _buffer.str() );
    
    std::cout << _buffer.str() << std::endl;
}

void NetPacket::ReadSize()
{
//    _size  = (PacketSize)_buffer[0];
//    _size += (PacketSize)_buffer[1] << 8;
}

bool NetPacket::SendBool(bool data)
{
    return SendUchar(data ? 1 : 0);
}

bool NetPacket::RecvBool()
{
    return RecvUchar() != 0;
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
    // DEBUG: check if there is data left
    assert(_pos < _size);

    return 0; //(_pos < _size) ? _buffer[_pos++] : 0;
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

    // DEBUG: check if there is data left
    //assert(_pos < _size);

    uint retval = 0;

    //retval = (uint)_buffer[_pos++];
    //retval += (uint)_buffer[_pos++] << 8;
    //retval += (uint)_buffer[_pos++] << 16;
    //retval += (uint)_buffer[_pos++] << 24;

    //retval = (uint)_buffer[_pos];
    //_pos += sizeof(uint);

    return retval;
}

bool NetPacket::SendString(const char* data)
{
    // DEBUG: check if there is data
    //assert(data != (const char*)0);

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
    //assert(buf != (char *)0);

    // DEBUG: ensure that we are reading, not writing
    //assert(_read_sock != (SocketHandler *)0);

    bool retval = false;
    //uint i;

    // strcpy works buggy, replaced with plain while loop

    //i = strlen((const char*)&_buffer[_pos]);
    //strcpy(buf, (const char*)&_buffer[_pos]);
    //_pos += i;

    //int k = 0;
    //while ((buf[k++] = _buffer[_pos++]) != '\0' && _pos < _size);
    //buf[k++] = '\0';

    //retval = true;

    return retval;
}

