
#include "Menu.h"
#include "MainMenu.h"


MainMenu::MainMenu( GameClient* gc ): Menu( gc, 2 )
{};
void MainMenu::draw()
{
    clear();
    box( stdscr, '|', '-' );
    drawTitle();

    mvaddstr( LINES - 10, COLS/2 - 1 , "M A I N" );
    mvaddstr( LINES - 9, COLS/2 - 1 , "*******" );

    if ( _selected == 0 )
        mvaddstr( LINES - 8, COLS/2 - 2, "> LOGIN <" );
    else
        mvaddstr( LINES - 8, COLS/2, "LOGIN" );

    if ( _selected == 1 )
        mvaddstr( LINES - 7, COLS/2 - 2, "> REGISTER <" );
    else
        mvaddstr( LINES - 7, COLS/2, "REGISTER" );

    if ( _selected == 2 )
        mvaddstr( LINES - 6, COLS/2 - 2, "> EXIT <" );
    else
        mvaddstr( LINES - 6, COLS/2, "EXIT" );
    refresh();
};

Menu* MainMenu::execute()
{

    if ( _selected == 0 ) {
        return new LoginMenu( _gc );
    } else if ( _selected == 1 ) {
        return new RegisterMenu( _gc );
    } else if ( _selected == 2 ) {
        _gc->signal_exit();
        return (Menu*)0;
    }
};
