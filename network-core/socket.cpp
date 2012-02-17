// Project Platypus
// socket.cpp - implements SocketHandler base class, used by both server and client

#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "defines.h"
#include "datatypes.h"
#include "socket.h"

#include <stdio.h>
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
   if (_pfirst)
   {
      p->_next = _pfirst;
      // check & replace that
      while(p->_next->_next != (NetPacket *)0) p->_next = p->_next->_next;
   }
   else
   {
      _pfirst = p;
      p->_next = (NetPacket *)0;
   }
}

bool SocketHandler::SendPacket(NetPacket* p)
{
   bool retval = false;
   uint snd;

   if (p != (NetPacket *)0)
   {
      p->PrepareToSend();

      snd = send(_sockfd, (const void*) &p->_buffer[0], p->_size, 0);
      if ( snd == p->_size)
      {
         // EVENT: Packet sent succesfully
         retval = true;
         
         // printf("SendPacket passed, size: %d\n", p->_size);
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

   if (recv(_sockfd, (void*) &p->_buffer[0], psize, 0) == psize )
   {
      p->ReadSize();
      if (recv(_sockfd, (void*) &p->_buffer[psize], p->_size - psize, 0) == p->_size - psize)
      {
         // printf("RecvPacket passed packet to HandlePacket\n");
         p->_pos = psize;
         retval = this->HandlePacket(p);
      }
   }

   // important
   delete p;

   return retval;
}

bool SocketHandler::SendAck(NetPacketType ack, bool val)
{
   NetPacket* p = new NetPacket(ack);
   bool retval = false;

   if ( p->SendBool(val) )
   {
      retval = this->SendPacket(p);
   }

   // important
   delete p;

   return retval;
}

bool SocketHandler::SendFile(const char* msg)
{
   // not working properly yet

   NetPacket* p = new NetPacket(PACKET_CLIENT_FILE);
   bool retval = false;
   int line;

   p->SendString(msg);
   // loaded something
   retval = this->SendPacket(p);

   // important
   delete p;

   return retval;
}

bool SocketHandler::RecvFile(NetPacket* p)
{
   // not working properly yet

   // DEBUG: it is not a null pointer
   assert(p != (NetPacket *)0);

   bool retval = false;
   char msg[p->_size+1];

   // check if client is actually authorized to send this packet
   if (0 /*_status != STATUS_ACTIVE */) { }
   else
   {
      printf("got packet, size: %d\n", p->_size);
      write(STDOUT_FILENO, &p->_buffer[p->_pos], p->_size);

   /* while (p->RecvString(msg, strlen((const char *) p->_buffer[p->_pos])))
      {
         p->_pos += strlen((const char *) p->_buffer[p->_pos]);
         printf("%s", msg);
      } */

      retval = true;
   }
   return retval;
}



