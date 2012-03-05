// Project Platypus
// packet.cpp - implements NetPacket class

#include "../core/defines.h"
#include "../core/bitmath.h"
#include "packet.h"

#include <string.h>
#include <stdio.h>

NetPacket::NetPacket(SocketHandler* sock)
{
   // DEBUG: crash if NULL is passed (false evaluated)
   assert(sock != (SocketHandler *)0);

   _read_sock = sock;
   _next = (NetPacket *)0;
   _size = 0;
   _pos = 0;
   _buffer = new uchar[MAX_PACKET_SIZE];
}

NetPacket::NetPacket(NetPacketType type)
{
   _read_sock = (SocketHandler *)0;
   _next = (NetPacket *)0;
   _size = sizeof(PacketSize);
   _pos = 0;
   _buffer = new uchar[MAX_PACKET_SIZE];
   _buffer[_size++] = type;
}

NetPacket::~NetPacket()
{
   delete [] _buffer;
}

void NetPacket::PrepareToSend()
{
   printf("sent %d bytes\n", _size);
   _pos = 0;

   // implement standart realloc function
   int i;
   uchar* tmp = new uchar[_size];

   for(i = 0; i < _size; i++)
      tmp[i] = _buffer[i];
   delete [] _buffer;
   _buffer = tmp;

   // test both methods:

   // 1 - to retrieve size by bytes
   // uchar* getsize = (uchar *)&_size;

   // _buffer[0] = getsize[0];
   // _buffer[1] = getsize[1];

   // 2 - using bitmath

   _buffer[0] = GB(_size, 0);
   _buffer[1] = GB(_size, 8);
}

void NetPacket::ReadSize()
{
	_size  = (PacketSize)_buffer[0];
	_size += (PacketSize)_buffer[1] << 8;
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
   if (_size + sizeof(data) > MAX_PACKET_SIZE) { }
   else
   {
      _buffer[_size++] = data;
      retval = true;
   }
   
   return retval;
}

uchar NetPacket::RecvUchar()
{
   // DEBUG: check if there is data left
   assert(_pos < _size);
   
   return (_pos < _size) ? _buffer[_pos++] : 0;
}

bool NetPacket::SendString(const char* data)
{
   // DEBUG: check if there is data
   assert(data != (const char*)0);

   bool retval = false;
   
   // return false if string is longer than space left in a packet
   if (_size + strlen(data) + 1 > MAX_PACKET_SIZE) { }
   else
   {
      while ((_buffer[_size++] = *data++) != '\0');
      _size--;
      retval = true;
   }

   return retval;
}

bool NetPacket::RecvString(char* buf, PacketSize size)
{
   // DEBUG: passed string is not null
   assert(buf != (char *)0);

   // DEBUG: ensure that we are reading, not writing
   assert(_read_sock != (SocketHandler *)0);

   bool retval = false;
   uint i;

   // return false if it is asked to get more than it contains
   if (size > _size - _pos) { }
   else
   {
      // printf("size is: %d, pos is: %d, asking for: %d\n", _size, _pos, size);
      // while ( _pos < _size && (buf[i++] = _buffer[_pos++]) != '\0' );
      i = strlen((const char*)&_buffer[_pos]);
      strcpy(buf, (const char*)&_buffer[_pos]);
      _pos += i;
      retval = true;
   }

   return retval;
}

