
#include "ScoresMenu.h"

ScoresMenu::ScoresMenu( GameClient* gc ): Menu( gc, 0 )
{
};

void ScoresMenu:: draw()
{
    clear();
    box( stdscr, '|', '-' );
    drawTitle();

    mvaddstr( LINES - 10, COLS/2 - 1 , "S C O R E S" );
    mvaddstr( LINES - 9, COLS/2 - 1 , "***********" );

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

Menu* ScoresMenu::execute()
{

    if ( _selected == 0 )
        return new MainMenu( _gc );
    return (Menu*)0;
};

