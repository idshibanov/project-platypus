
#ifndef PLA_NCLIENT_H
#define PLA_NCLIENT_H

#include "../core/defines.h"

#define PLA_TEMP_NCURSES_OUTPUT
#define CHATSIZE 15

class ClientSocketHandler;
struct Character;
class ChatWindow;

class GameClient
{
   std::string _message, _username;   
   fd_set _readfds, _testfds;
   int _port, _server_sock;
   
   int _last_move;
   
   ChatWindow* _cw;
   Character* _c;
   ClientSocketHandler* _serv_sh;
   
   public:
   GameClient(int port = SERV_PORT);
   virtual ~GameClient();
   bool net_connect();
   bool run_select();
   void init_curses();
   void ncurses_temp_out(char* str);
   void drawScreen( int, int );
   void set_char(int x, int y);
   void move_char();
};

#endif
