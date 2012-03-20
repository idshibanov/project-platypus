
#include "Menu.h"
#include "MainMenu.h"

RegisterMenu::RegisterMenu( GameClient* gc ): Menu( gc, 1 )
{};

void RegisterMenu:: draw()
{
    clear();
    box( stdscr, '|', '-' );
    drawTitle();

    mvaddstr( LINES - 10, COLS/2 - 1 , "R E G I S T E R" );
    mvaddstr( LINES - 9, COLS/2 - 1 , "***************" );

    if ( _selected == 0 )
        mvaddstr( LINES - 8, COLS/2 - 2, "> SUBMIT <" );
    else
        mvaddstr( LINES - 8, COLS/2, "SUBMIT" );

    if ( _selected == 1 )
        mvaddstr( LINES - 7, COLS/2 - 2, "> BACK <" );
    else
        mvaddstr( LINES - 7, COLS/2, "BACK" );

    refresh();
};

Menu* RegisterMenu::execute()
{

    if ( _selected == 0 )
        // TODO: IMPLEMENT
        return new MainMenu( _gc );

    if ( _selected == 1 )
        return new MainMenu( _gc );
};

