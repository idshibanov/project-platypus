#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <curses.h>
#include <signal.h>
#include <sys/stat.h>
#include <fcntl.h>

using namespace std;

#include "nclient_core.h"
#include "nclient.h"
#include "./core/defines.h"
#include "./net/client_socket.h"

GameClient::GameClient(int port)
{
   _port = port;
   _cw = new ChatWindow( LINES - (CHATSIZE+2), CHATSIZE - 2);
   _c = new Character();
   _serv_sh = (ClientSocketHandler* )0;
   
   net_connect();
}

GameClient::~GameClient()
{
   delete _c;
   delete _cw;
   if (_serv_sh)
      delete _serv_sh;
}

bool GameClient::net_connect()
{
   struct addrinfo* ailist;
   getaddrinfo(SERV_IP, NULL, NULL, &ailist);
   struct sockaddr_in* server;
   server = (struct sockaddr_in* )ailist->ai_addr;
   server->sin_port = htons(_port);
  
   if( ( _server_sock = socket( AF_INET, SOCK_STREAM, 0 ) ) == -1 ) {
      perror( "socket call failed" );
      exit( 1 );
   }
   
   if( connect( _server_sock, ( sockaddr* ) server, sizeof(sockaddr_in) ) == -1 ) {
      perror( "connect call failed" );
      exit( 1 );
   }
   
   _serv_sh = new ClientSocketHandler(_server_sock, this);
   
   FD_ZERO(&_readfds);
   FD_SET(_server_sock, &_readfds);
   FD_SET(STDIN_FILENO, &_readfds);
   
   init_curses();
   run_select();
   
   return true;
}

bool GameClient::run_select()
{
  int fd, nread, ch;
  char buffer[1024];

  while (1) {
    _testfds = _readfds;
    select(FD_SETSIZE, &_testfds, NULL, NULL, NULL);

    for (fd = 0; fd < 4; fd++) {
      if (FD_ISSET(fd, &_testfds)) {
        if (fd == 0)  {
          ch = getch();
          switch ( ch ) {
            case KEY_DOWN:
              //write(server_sock, "KEY_DOWN PRESSED", 17);
              _serv_sh->SendMovement(2);
              if ( _c->y < ( LINES - CHATSIZE ) - 3 ) {
                _c->y++;
              }
              break;
            case KEY_UP:
              //write(server_sock, "KEY_UP PRESSED", 15);
              _serv_sh->SendMovement(8);
              if ( _c->y > 1 ) {
                _c->y--;
              }
              break;
            case KEY_LEFT:
              //write(server_sock, "KEY_LEFT PRESSED", 17);
              _serv_sh->SendMovement(4);
              if ( _c->x > 2 ) {
                _c->x--;
              }
              break;
            case KEY_RIGHT:
              //write(server_sock, "KEY_RIGHT PRESSED", 18);
              _serv_sh->SendMovement(6);
              if ( _c->x < COLS-2) {
                _c->x++;
              }
              break;
            case 0x63:
            case 0x43:
              mvprintw( LINES - 2, 2, _username.c_str() );
              mvprintw( LINES - 2, _username.size() + 2, ": " );
              fflush( stdin );
              curs_set(1);
              echo();
              getstr( buffer );
              curs_set(0);
              noecho();
              if( buffer[0] != '\0' ) {
                _message = string( buffer );
                _message.insert( 0, ": " );
                _message.insert( 0, _username);
                //write( _server_sock, _message.c_str(), _message.size() );
                _serv_sh->SendChatMsg(_message.c_str());
                _cw->addMessage( _message );
              }
              break;
          }
          clear();
          _cw->draw();
          drawScreen(_c->x, _c->y);
          refresh();
        } else {
          ioctl( fd, FIONREAD, &nread );

          if( nread == 0 ) {
            close( fd );
            mvcur(0,COLS-1,LINES-1,0);
            endwin();         
            exit(0);
          }

          //nread = read( fd, buffer, nread );
          //buffer[nread] = '\0';

          //cw->addMessage( string( buffer ) );
          //clear();
          //cw->draw();
          
          _serv_sh->RecvPacket();
          
          drawScreen(_c->x, _c->y);					
          refresh();
        }
      }
    }
  }
  
  return true;
}

void GameClient::init_curses()
{
   char buffer[1024];

   int die();
   initscr();
   clear();
   keypad(stdscr,1);
   signal(SIGINT,(__sighandler_t)die);
   crmode();
   noecho();
   nonl();
   curs_set(0);
   
   // initially from main()
   _c->x = _c->y = 2;
   drawScreen(_c->x, _c->y);
   _cw->draw();
   mvprintw( LINES - 2, 1, "Enter User Name: " );
   curs_set(1);
   echo();
   getstr( buffer );
   curs_set(0);
   noecho();
   _username = string( buffer );
   clear();
   drawScreen(_c->x, _c->y);
   _cw->draw();  
   refresh();
}

void GameClient::drawScreen( int x, int y ) {
  if( !x || !y )
    return;
  move( y, x );
  addch( 'X' );

  box( stdscr, '|', '-' );

  move( LINES - CHATSIZE - 2, 0 );
  addch( ACS_LTEE );

  move( LINES - CHATSIZE - 2, COLS -1 );
  addch( ACS_RTEE );
}

void GameClient::ncurses_temp_out(char* str)
{
   //string tmp("Server: ");
   //tmp.append(str);
   _cw->addMessage( string(str) );
   clear();
   _cw->draw();
}


int main() {
   GameClient gc;

   return 0;
}

int die() {
  signal(SIGINT,SIG_IGN);
  mvcur(0,COLS-1,LINES-1,0);
  endwin();
  exit(0);
}


