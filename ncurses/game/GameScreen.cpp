#include <curses.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <curses.h>
#include <signal.h>
#include <sys/stat.h>
#include <fcntl.h>

using namespace std;

#include "GameScreen.h"
#include "../nclient_core.h"
#include "../../core/defines.h"
#include "../../net/client_socket.h"

GameScreen::GameScreen( string playerName, int port ):
    _port( port ),
    _c( new Character( playerName ) ),
    _serv_sh( (ClientSocketHandler* )0 ),    
   _last_move(0 )
{};

GameScreen::~GameScreen()
{
    delete _c;
    delete _cw;
    if (_serv_sh)
        delete _serv_sh;
}

void GameScreen::run_select()
{

    int fd, nread, ch;
    char buffer[1024];

    while (1) {
        _testfds = _readfds;
        select(FD_SETSIZE, &_testfds, NULL, NULL, NULL);

        for (fd = 0; fd < 4; fd++) {
            if (FD_ISSET(fd, &_testfds)) {
                if ( fd == _server_sock ) {
                    ioctl( fd, FIONREAD, &nread );

                    if( nread == 0 ) {
                        close( fd );
                        mvcur(0,COLS-1,LINES-1,0);
                        endwin();         
                        exit(0);
                    }
                    
                    _serv_sh->RecvPacket();
                    clear();
                    drawGameScreen();				
                    refresh();
                } else if (fd == 0)  {
                    ch = getch();
                    switch ( ch ) {
                        case KEY_DOWN:
                            _serv_sh->SendMovement(2);
                            _last_move = 2;
                            break;
                        case KEY_UP:
                            _serv_sh->SendMovement(8);
                            _last_move = 8;
                            break;
                        case KEY_LEFT:
                            _serv_sh->SendMovement(4);
                            _last_move = 4;
                            break;
                        case KEY_RIGHT:
                            _serv_sh->SendMovement(6);
                            _last_move = 6;
                            break;
                        case 0x63:
                        case 0x43:
                            mvprintw( LINES - 2, 2, _c->name.c_str() );
                            mvprintw( LINES - 2, _c->name.size() + 2, ": " );
                            fflush( stdin );
                            curs_set(1);
                            echo();
                            getstr( buffer );
                            curs_set(0);
                            noecho();
                            if( buffer[0] != '\0' ) {
                              _message = string( buffer );
                              _message.insert( 0, ": " );
                              _message.insert( 0, _c->name );
                              _serv_sh->SendChatMsg(_message.c_str());
                              _cw->addMessage( _message );
                            }
                            break;
                    }
                    clear();          
                    drawGameScreen();
                    refresh();
                 
                }
            }
        }
    }
    
    return;
}

void GameScreen::init_game()
{
    _cw = new ChatWindow( LINES - (CHATSIZE+2), CHATSIZE - 2);

    _c->x = _c->y = 2;
    drawGameScreen();
    _cw->draw();
    refresh();
    
}

bool GameScreen::net_connect()
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

    clear();

    init_game();
    run_select();

    return true;
}

void GameScreen::drawGameScreen()
{
    _cw->draw(); 

    drawCharacters();

    box( stdscr, '|', '-' );

    move( LINES - CHATSIZE - 2, 0 );
    addch( ACS_LTEE );

    move( LINES - CHATSIZE - 2, COLS -1 );
    addch( ACS_RTEE ); 
}

void GameScreen::drawCharacters()
{

    //player
    move( _c->y, _c->x );
    addch( 'X' );

    if ( _players.size() > 0 ) {
        //other players
        vector<Character>::iterator vi;
        for ( vi = _players.begin(); vi < _players.end(); vi++ ) {
            move( vi->y, vi->x );
            addch( 'X' );
        }
    }
  
}

void GameScreen::ncurses_temp_out(char* str)
{
    _cw->addMessage( string(str) );
}

void GameScreen::addNewPlayer( int id, int x, int y, char* name)
{
    Character *nc = new Character( string ( name ) );
    nc->x = x;
    nc->y = y;
    nc->id = id;

    _players.push_back( *nc );
}

Coords GameScreen::getPlayerPosition()
{
    Coords pc;
    pc.x = _c->x;
    pc.y = _c->y;
    return pc;
}

void GameScreen::set_char(int id, unsigned int x, unsigned int y)
{
    bool found = false;
    vector<Character>::iterator vi;
    for ( vi = _players.begin(); vi < _players.end(); vi++ ) {
        if( vi->id == id) {
            vi->x = x;
            vi->y = y;
            found = true;
            break;
        }
    }

    if(!found)
        addNewPlayer(id, x, y, (char*) "");
}

void GameScreen::move_char()
{
    if (!_last_move)
        return;

    switch(_last_move) {
        case 2:
            _c->y++;
            break;
        case 4:
            _c->x--;
            break;
        case 6:
            _c->x++;
            break;
        case 8:
            _c->y--;
            break;
    }
    _last_move = 0;
}
