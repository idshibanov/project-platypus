
#ifndef PLA_GAMESCREEN_H
#define PLA_GAMESCREEN_H

#include <vector>
#include <string.h>
#include <string>

#include "../../core/defines.h"
#include "../tools/Prompt.h"

class Character;
struct Coords;
class ChatWindow;
class ClientSocketHandler;

#define PLA_TEMP_NCURSES_OUTPUT
#define CHATSIZE 15

class GameScreen {
    private:
        
        int _port, _server_sock;
        int _last_move;

        ChatWindow* _cw;
        Character* _c;
        std::vector<Character> _players;
        std::string _message;
        fd_set _readfds, _testfds;
        ClientSocketHandler* _serv_sh;
       	Prompt* _pr;
        
    public:
        GameScreen( std::string n, int port = SERV_PORT );
        ~GameScreen();
        void run_select();
        // Network related memebers
        bool net_connect(std::string pwd);

        // Game related members
        void init_game();
        void addMessage(char* str);
        void addNewPlayer( int, int, int, char* );
        void drawCharacters();
        void drawGameScreen();
        Coords getPlayerPosition();
        void set_char(int id, unsigned int x, unsigned int y);
        void move_char(unsigned int x, unsigned int y);
};

#endif
