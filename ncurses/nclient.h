
#ifndef PLA_NCLIENT_H
#define PLA_NCLIENT_H

#include <vector>
#include "../core/defines.h"

#define PLA_TEMP_NCURSES_OUTPUT
#define CHATSIZE 15

class ClientSocketHandler;
struct Character;
struct Coords;
class ChatWindow;

class GameClient
{
   std::string _message, _username;   
   fd_set _readfds, _testfds;
   int _port, _server_sock;
   
   int _last_move;
   
   ChatWindow* _cw;
   Character* _c;
   std::vector<Character> _players;
   ClientSocketHandler* _serv_sh;
   
   public:
   GameClient(int port = SERV_PORT);
   virtual ~GameClient();
   bool net_connect();
   bool run_select();
   void init_curses();
   void ncurses_temp_out(char* str);
   void addNewPlayer( int, int, int, char* );
   void drawChararacters();
   void drawScreen();
   Coords getPlayerPosition();
   void set_char(int x, int y);
   void move_char();
};

#endif
