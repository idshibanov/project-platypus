
#ifndef PLA_NCLIENT_H
#define PLA_NCLIENT_H

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

#include <vector>
#include "../core/defines.h"

enum MENU_TYPE {
    MAIN_MENU,
    LOGIN_MENU,
    REGISTER_MENU,
    SETTINGS_MENU,
    SCORES_MENU,
    JOIN_GAME
};

class GameScreen;
class Menu;

class GameClient {
    private:

        Menu* _menu;

        GameScreen* _gs;
        
        MENU_TYPE _current_menu;

        bool _exit_flag;

    public:
        // Constructor, De-Constructor
        GameClient();
        virtual ~GameClient();

        // Initialize ncurses
        void init_curses();

        // Menu related members
        void enter_menus();

        void signal_exit()
        {
            _exit_flag = true; 
        };


};

#endif
