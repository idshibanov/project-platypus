
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
        mvaddstr( LINES - 11, COLS / 2 - 18, "Username: " );
        std::string str = p.getMessage( COLS / 2 - 5, LINES - 11, 14 );        
        GameScreen gs( str );
        gs.net_connect();
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

