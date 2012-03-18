
#include "Menu.h"
#include "MainMenu.h"
#include "LoginMenu.h"
#include "RegisterMenu.h"
#include "SettingsMenu.h"
#include "../game/GameScreen.h"

LoginMenu::LoginMenu( GameClient* gc ): Menu( gc, 0 )
{
    GameScreen gs( "Chris" );
    gs.net_connect();
};

void LoginMenu:: draw()
{
    clear();
    box( stdscr, '|', '-' );
    drawTitle();

    if ( _selected == 0 )
        mvaddstr( LINES - 8, COLS/2 - 2, "> Back <" );
    else
        mvaddstr( LINES - 8, COLS/2, "Back" );

    /*if ( _selected == 1 )
        mvaddstr( LINES - 7, COLS/2 - 2, "> dsfg <" );
    else
        mvaddstr( LINES - 7, COLS/2, "sdfg" );

    if ( _selected == 2 )
        mvaddstr( LINES - 6, COLS/2 - 2, "> sdfg <" );
    else
        mvaddstr( LINES - 6, COLS/2, "sdfg" );

    if ( _selected == 3 )
        mvaddstr( LINES - 5, COLS/2 - 2, "> EXIT <" );
    else
        mvaddstr( LINES - 5, COLS/2, "EXIT" );*/
    refresh();
};

Menu* LoginMenu::execute()
{

    if ( _selected == 0 )
        return new MainMenu( _gc );
    return (Menu*)0;
};

