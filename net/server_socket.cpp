// Project Platypus
// server_socket.cpp - implements ServerSocketArray and ServerSocketHandler classes

#include "../core/defines.h"
#include "../server/server.h"
#include "server_socket.h"
#include <stdio.h>

ServerSocketHandler::ServerSocketHandler (int socket, GameServer* serv)
                                          : SocketHandler(socket)
{
   _status = STATUS_SERVER_INACTIVE;
   _serv = serv;
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

bool ServerSocketHandler::RecvClientMovement(NetPacket* p)
{
   // DEBUG: it is not a null pointer
   assert(p != (NetPacket *)0);

   bool retval = false;
   
   // TODO: proper handling


   unsigned int mvm = p->RecvUint();

   //server->broadcast_movement(client, newpos);
   SendAck(PACKET_SERVER_MOVE_RESPONSE, true);

   retval = true;

   return retval;
}


// Implementation of ServerSocketArray class

ServerSocketArray::ServerSocketArray(GameServer* serv)
{
   _lenght = 0;
   _serv = serv;
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
      _client_sock[_lenght] = new ServerSocketHandler(socket, _serv);
      _client_sock[_lenght]->SendAck(PACKET_SERVER_WELCOME);
      _lenght++;
      retval = true;
   } else {
      ServerSocketHandler tmp(socket, _serv);
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

void ServerSocketArray::ClearList()
{
   int i;
   for(i = 0; i < _lenght && _client_sock[i]; i++)
   {
      delete _client_sock[i];
      
      // so we cant delete memory again
      _client_sock[i] = (ServerSocketHandler*) 0;
   }
   
   _lenght = 0;
}

int ServerSocketArray::Lenght()
{
   return _lenght;
}

ServerSocketArray::operator int()
{
   return _lenght;
}

ServerSocketHandler* ServerSocketArray::GetClient (const int sockfd)
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

ServerSocketHandler* ServerSocketArray::operator [] (const int sockfd)
{
   return GetClient(sockfd);
}

