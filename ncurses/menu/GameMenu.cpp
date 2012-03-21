
#include "Menu.h"
#include "MainMenu.h"
#include "GameMenu.h"
#include "RegisterMenu.h"
#include "SettingsMenu.h"
#include "../tools/Prompt.h"
#include "../game/GameScreen.h"

GameMenu::GameMenu( GameClient* gc ): Menu( gc, 3 )
{};

void GameMenu:: draw()
{
    clear();
    box( stdscr, '|', '-' );
    drawTitle();

    mvaddstr( LINES - 10, COLS/2 - 1 , "G A M E" );
    mvaddstr( LINES - 9, COLS/2 - 1 , "*******" );

    if ( _selected == 0 )
        mvaddstr( LINES - 8, COLS/2 - 2, "> START GAME <" );
    else
        mvaddstr( LINES - 8, COLS/2, "START GAME" );

    if ( _selected == 1 )
        mvaddstr( LINES - 7, COLS/2 - 2, "> HIGH SCORES <" );
    else
        mvaddstr( LINES - 7, COLS/2, "HIGH SCORES" );

    if ( _selected == 2 )
        mvaddstr( LINES - 6, COLS/2 - 2, "> SETTINGS <" );
    else
        mvaddstr( LINES - 6, COLS/2, "SETTINGS" );

    if ( _selected == 3 )
        mvaddstr( LINES - 5, COLS/2 - 2, "> BACK <" );
    else
        mvaddstr( LINES - 5, COLS/2, "BACK" );
    refresh();
};

Menu* GameMenu::execute()
{

    if ( _selected == 0 ) {        
        // start a game session. 
        // Temporary... just for testing purposes
        Prompt p( ALPHANUMERIC );
        mvaddstr( LINES - 13, COLS / 2 - 14, "Username: " );
        std::string str = p.getMessage( COLS / 2 - 4, LINES - 13, 14 );
        Prompt p2( ALPHANUMERIC );
        mvaddstr( LINES - 12, COLS / 2 - 14, "Password: " );
        std::string str2 = p2.getMessage( COLS / 2 - 4, LINES - 12, 14 ); 
        GameScreen gs( str );
        gs.net_connect( str2 );
        return new GameMenu( _gc );
    } else if ( _selected == 1 ) {
        return new ScoresMenu( _gc );
    } else if ( _selected == 2 ) {
        return new SettingsMenu( _gc );
    } else if ( _selected == 3 ) {
        // TODO: Log out here
        return new MainMenu( _gc );
    }
};

