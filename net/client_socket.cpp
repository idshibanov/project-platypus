// Project Platypus
// client_socket.cpp - implements ClientSocketHandler class

#include <stdio.h>
#include <string>
#include <sys/select.h>

#include "../ncurses/nclient.h"
#include "client_socket.h"

ClientSocketHandler::ClientSocketHandler(int socket, GameClient* gc) : SocketHandler(socket)
{
   _status = STATUS_CLIENT_INACTIVE;
   _gc = gc;
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

   switch(p->_buffer[p->_pos++])
   {
      case PACKET_SERVER_FULL:
         if (_status == STATUS_CLIENT_INACTIVE )
         {
            // printf("Sorry, server is full\n");            
            // TODO: disconnect      
            retval = true;
         }
         break;
      case PACKET_SERVER_WELCOME:
         if (_status == STATUS_CLIENT_INACTIVE )
         {
            // temp replace
            //_status = STATUS_CLIENT_NOT_AUTH;
            
            _status = STATUS_CLIENT_ACTIVE;
            
            // TODO: ask user to send his username
            retval = true;
         }
         break;
      case PACKET_SERVER_SHUTDOWN:
         // TODO: disconnect
         retval = true;
         break;
      case PACKET_SERVER_AUTH_RESPONSE:
         if (_status == STATUS_CLIENT_NOT_AUTH )
         {
            if (p->RecvBool())
            {
               _status = STATUS_CLIENT_AUTH;
            }
            retval = true;
         }      
         break;
      case PACKET_SERVER_JOIN_MAP:
         if (_status == STATUS_CLIENT_MAP )
         {
            //retval = this->RecvMapStruct();
         }
         break;
      case PACKET_SERVER_MAPDATA:
         if (_status == STATUS_CLIENT_ACTIVE )
         {
            // EVENT: something happend on the map
            retval = this->RecvMapData(p);
         }
         break;
      case PACKET_SERVER_MOVE_RESPONSE:
         if (_status == STATUS_CLIENT_ACTIVE )
         {
            if (p->RecvBool())
            {
               // EVENT: recieved true on request
               // TODO: move character
               _gc->move_char();
               _gc->ncurses_temp_out((char*)"Can move");
            }
            retval = true;
         }
         break;
      case PACKET_SERVER_CHAT:
         if (_status < STATUS_CLIENT_END ) // should be = STATUS_CLIENT_ACTIVE
         {
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

   if ( p->SendString(msg) )
   {
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
   else
   {
      if (p->RecvString(msg))
      {
         // printf("RecvString passed\n");
         
         // got processed msg here
         #ifdef PLA_TEMP_NCURSES_OUTPUT
         _gc->ncurses_temp_out(msg);
         #endif
         #ifndef PLA_TEMP_NCURSES_OUTPUT
         printf("Server says: %s", msg);
         #endif
         
      } else
      {
         // printf("RecvString returned false\n");
      }


      retval = true;
   }
   delete [] msg;
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
   NetPacket* p = new NetPacket(PACKET_CLIENT_MOVEMENT);
   bool retval = false;

   if ( p->SendUint(side) )
   {
      retval = this->SendPacket(p);
   }

   // important
   delete p;

   return retval;
}

bool ClientSocketHandler::RecvMapData(NetPacket* p)
{
   // DEBUG: it is not a null pointer
   assert(p != (NetPacket *)0);

   bool retval = false;
   
   int x = p->RecvUint();
   int y = p->RecvUint();
   
   _gc->set_char(x, y);

   return retval;
}


