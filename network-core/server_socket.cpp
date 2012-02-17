// Project Platypus
// socket.cpp - implements ServerSocketArray and ServerSocketHandler classes

#include "defines.h"
#include "server_socket.h"
#include <stdio.h>

ServerSocketHandler::ServerSocketHandler (int socket, GameInstance* game)
                                          : SocketHandler(socket)
{
   _status = STATUS_SERVER_INACTIVE;
   _game = game;
}

ServerSocketHandler::~ServerSocketHandler()
{

}

bool ServerSocketHandler::HandlePacket(NetPacket* p)
{
   // DEBUG: it is not a null pointer
   assert(p != (NetPacket *)0);
   
   printf("got %d bytes, packet %d\n", p->_size, p->_buffer[p->_pos]);
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
         // kill client
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
         // retval = this->RecvMove(p);
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
   char msg[p->_size+1];

   // check if client is actually authorized to send this packet
   if (0 /*_status != STATUS_SERVER_ACTIVE */) { }
   else
   {
      if (p->RecvString(msg, p->_size - p->_sizeof_sizetype))
      {
         // printf("RecvString passed\n");
      } else
      {
         // printf("RecvString returned false\n");
      }

      // got processed msg here
      printf("Client says: %s", msg);
      retval = true;
   }
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
      p->RecvString(msg, p->_size - p->_sizeof_sizetype);
      
      // TODO: login system
      
      _status = STATUS_SERVER_AUTHORIZED;
      retval = true;
   }

   return retval;   
}


// Implementation of ServerSocketArray class

ServerSocketArray::ServerSocketArray(GameInstance* game)
{
   _lenght = 0;
   _game = game;
}

ServerSocketArray::~ServerSocketArray()
{
   int i;
   for(i = 0; i < _lenght && _client_sock[i]; i++)
      delete _client_sock[i];
}

bool ServerSocketArray::AddClient(int socket)
{
   // DEBUG: passed wrong socket #
   assert(socket >= MIN_CLIENT_SOCKFD);

   bool retval = false;

   if (socket < MIN_CLIENT_SOCKFD) { }
   else if ( _lenght < MAX_CLIENTS )
   {
      _client_sock[_lenght] = new ServerSocketHandler(socket, _game);
      _lenght++;
      retval = true;
   } else {
      ServerSocketHandler tmp(socket, _game);
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

int ServerSocketArray::Lenght()
{
   return _lenght;
}

ServerSocketArray::operator int()
{
   return _lenght;
}

ServerSocketHandler* ServerSocketArray::operator [] (const int sockfd)
{
   ServerSocketHandler* retval = (ServerSocketHandler *)0;
   int i;

   for(i = 0; i < _lenght && _client_sock[i]; i++)
   {
      if (_client_sock[i]->_sockfd == sockfd)
         retval = (ServerSocketHandler *) _client_sock[i];
   }
   return retval;
}

