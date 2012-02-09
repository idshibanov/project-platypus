// Project Platypus
// socket.cpp - implements ClientSocketArray and ClientSocketHandler classes

#include <sys/socket.h>
#include <sys/types.h>

#include "defines.h"
#include "datatypes.h"
#include "socket.h"

#include <stdio.h>


// Implementation of ClientSocketHandler class

ClientSocketHandler::ClientSocketHandler(int socket)
{
   // constructor is private, so we assume that socket data is ok
   
   _sockfd = socket;
   _pfirst = (NetPacket *)0;
   
   // temp assignment, must be replaced with a proper auth. system
   _status = STATUS_ACTIVE;
}

ClientSocketHandler::~ClientSocketHandler() { }

int ClientSocketHandler::GetSockNo()
{
   return _sockfd;
}

ClientSocketHandler::operator int()
{
   return _sockfd;
}

bool ClientSocketHandler::AddPacket(NetPacket* p)
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

bool ClientSocketHandler::SendPacket(NetPacket* p)
{
   bool retval = false;
   uint snd;
   
   if (p != (NetPacket *)0)
   {
      p->PrepareToSend();

      snd = send(_sockfd, (const void*) &p->_buffer[0], p->_size, 0);
      if ( snd == p->_size)
      {
         retval = true;
         printf("SendPacket passed, size: %d\n", p->_size);
      }
   }
   
   return retval;
}

bool ClientSocketHandler::SendPackets()
{
   // implement it later
   return false;
}

bool ClientSocketHandler::RecvPacket()
{
   bool retval = false;
   uint psize = sizeof(PacketSize);
   NetPacket* p = new NetPacket(this);
   
   if (recv(_sockfd, (void*) &p->_buffer[0], psize, 0) == psize )
   {
      p->ReadSize();
      printf("size read\n");
      if (recv(_sockfd, (void*) &p->_buffer[psize], p->_size - psize, 0) == p->_size - psize)
      {
         printf("RecvPacket passed packet to HandlePacket\n");
         p->_pos = psize;
         retval = this->HandlePacket(p);
      }
   }
   
   // important
   delete p;
   
   return retval;
}

bool ClientSocketHandler::HandlePacket(NetPacket* p)
{
   // replace & implement later
   
   // reads type char
   p->_pos++;
   return this->RecvChatMsg(p);
}

bool ClientSocketHandler::SendChatMsg(const char* msg)
{
   NetPacket* p = new NetPacket(PACKET_CLIENT_CHAT);
   bool retval = false;
   
   if ( p->send_string(msg) )
   {
      printf("send_string passed\n");
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
   char msg[p->_size+1];
   
   // check if client is actually authorized to send this packet
   if (_status != STATUS_ACTIVE) { }
   else
   {
      if (p->recv_string(msg, p->_size - p->_sizeof_sizetype))
      {
         printf("recv_string passed\n");
      } else
      {
         printf("recv_string returned false\n");
      }
      printf("RecvChatMsg passed\n");
      
      // got processed msg here
      printf(" - says: %s", msg);
      retval = true;
   }
   return retval;
}



// Implementation of ClientSocketArray class

ClientSocketArray::ClientSocketArray()
{
   _lenght = 0;
}

ClientSocketArray::~ClientSocketArray()
{
   int i;
   for(i = 0; i < _lenght && _client_sock[i]; i++)
      delete _client_sock[i];
}

bool ClientSocketArray::AddClient(int socket)
{
   // DEBUG: passed wrong socket #
   assert(socket >= MIN_CLIENT_SOCKFD);
   
   bool retval = false;
   
   if (socket < MIN_CLIENT_SOCKFD) { }
   else if ( _lenght < MAX_CLIENTS )
   {
      _client_sock[_lenght] = new ClientSocketHandler(socket);
      _lenght++;
      retval = true;
   }
   
   return retval;
}

bool ClientSocketArray::RemoveClient(int socket)
{
   // DEBUG: passed wrong socket #
   assert(socket >= MIN_CLIENT_SOCKFD);
   
   bool retval = false;
   int i;
   
   for(i = 0; i < _lenght && _client_sock[i]; i++)
   {
      if (_client_sock[i]->_sockfd == socket)
      {
         delete _client_sock[i];
         for(i += 1; i < _lenght && _client_sock[i]; i++)
            _client_sock[i-1] = _client_sock[i];
            
         _lenght--;
         retval = true;
         break;
      }
   }
   
   return retval;
}

ClientSocketHandler* ClientSocketArray::operator [] (const int sockfd)
{
   ClientSocketHandler* retval = (ClientSocketHandler *)0;
   int i;
   
   for(i = 0; i < _lenght && _client_sock[i]; i++)
   {
      if (_client_sock[i]->_sockfd == sockfd)
         retval = (ClientSocketHandler *) _client_sock[i];
   }
   return retval;
}
    
    

