
#include "Menu.h"
#include "MainMenu.h"
#include "LoginMenu.h"
#include "RegisterMenu.h"
#include "GameMenu.h"
#include "../game/GameScreen.h"

LoginMenu::LoginMenu( GameClient* gc ): Menu( gc, 2 )
{};

void LoginMenu:: draw()
{
    clear();
    box( stdscr, '|', '-' );
    drawTitle();

    mvaddstr( LINES - 10, COLS/2 - 1 , "L O G I N" );
    mvaddstr( LINES - 9, COLS/2 - 1 , "*********" );

    if ( _selected == 0 )
        mvaddstr( LINES - 8, COLS/2 - 2, "> SUBMIT <" );
    else
        mvaddstr( LINES - 8, COLS/2, "SUBMIT" );

    if ( _selected == 1 )
        mvaddstr( LINES - 7, COLS/2 - 2, "> REGISTER <" );
    else
        mvaddstr( LINES - 7, COLS/2, "REGISTER" );

    if ( _selected == 2 )
        mvaddstr( LINES - 6, COLS/2 - 2, "> BACK <" );
    else
        mvaddstr( LINES - 6, COLS/2, "BACK" );
    refresh();
};

Menu* LoginMenu::execute()
{
    
    if ( _selected == 0 ) {
        //TODO: IMPLEMENT LOGIN HERE / login method called here.
        return new GameMenu( _gc );
    } else if ( _selected == 1 ) {
        // might implement previous menu argument, so back can return the previous menu visited.
        // i.e.
        // return new RegisterMenu( _gc, this )
        return new RegisterMenu( _gc );
    } else if ( _selected == 2 ) {
        return new MainMenu( _gc );
    }
};

