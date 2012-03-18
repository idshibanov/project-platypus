
#include "Menu.h"
#include "MainMenu.h"


MainMenu::MainMenu( GameClient* gc ): Menu( gc, 4 )
{};
void MainMenu::draw()
{
    clear();
    box( stdscr, '|', '-' );
    drawTitle();

    if ( _selected == 0 )
        mvaddstr( LINES - 8, COLS/2 - 2, "> LOGIN <" );
    else
        mvaddstr( LINES - 8, COLS/2, "LOGIN" );

    if ( _selected == 1 )
        mvaddstr( LINES - 7, COLS/2 - 2, "> REGISTER <" );
    else
        mvaddstr( LINES - 7, COLS/2, "REGISTER" );

    if ( _selected == 2 )
        mvaddstr( LINES - 6, COLS/2 - 2, "> HIGH SCORES <" );
    else
        mvaddstr( LINES - 6, COLS/2, "HIGH SCORES" );

    if ( _selected == 3 )
        mvaddstr( LINES - 5, COLS/2 - 2, "> SETTINGS <" );
    else
        mvaddstr( LINES - 5, COLS/2, "SETTINGS" );

    if ( _selected == 4 )
        mvaddstr( LINES - 4, COLS/2 - 2, "> EXIT <" );
    else
        mvaddstr( LINES - 4, COLS/2, "EXIT" );
    refresh();
};

Menu* MainMenu::execute()
{

    if ( _selected == 0 ) {
        return new LoginMenu( _gc );
    } else if ( _selected == 1 ) {
        return new RegisterMenu( _gc );
    } else if ( _selected == 2 ) {
        return new ScoresMenu( _gc );
    } else if ( _selected == 3 ) {
        return new SettingsMenu( _gc );
    } else if ( _selected == 4 ) {
        _gc->signal_exit();
        return (Menu*)0;
    }
};
