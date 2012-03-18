
#include "nclient.h"
#include "../core/defines.h"
#include "../net/client_socket.h"
#include "game/GameScreen.h"
#include "menu/Menu.h"
#include "menu/MainMenu.h"
#include "menu/LoginMenu.h"
#include "menu/RegisterMenu.h"
#include "menu/SettingsMenu.h"

GameClient::GameClient():
    _current_menu( MAIN_MENU ),
    _menu( new MainMenu( this ) ),
    _exit_flag( false )
{
   enter_menus();
}

GameClient::~GameClient()
{
    if ( _menu )
        delete _menu;
    if ( _gs )
        delete _gs;
}

void GameClient::enter_menus()
{
    init_curses();
    int c;
    Menu* newMenu = NULL;
    while( !_exit_flag ) {
        _menu->draw();
        c = wgetch(stdscr);
        switch( c ) {
            case KEY_DOWN:
                _menu->goDown();
                break;
            case KEY_UP:
                _menu->goUp();
                break;
            case 10:
                newMenu = _menu->execute();
                if ( newMenu ) {
                    delete _menu;
                    _menu = newMenu;
                }
                break;
            case 27:
                _exit_flag = false;
                break;
         }
    }
    mvcur(0,COLS-1,LINES-1,0);
    endwin();
} 

void GameClient::init_curses()
{
    char buffer[1024];

    int die();
    initscr();
    clear();
    keypad(stdscr,1);
    
    noecho();
    cbreak();
    //nonl();
    curs_set(0);
    signal(SIGINT,(__sighandler_t)die);
}



int main()
{
    GameClient gc;
    return 0;
}

int die()
{
    signal(SIGINT,SIG_IGN);
    mvcur(0,COLS-1,LINES-1,0);
    endwin();
    exit(0);
}


