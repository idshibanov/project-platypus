
#ifndef PLA_NCLIENT_CORE_H
#define PLA_NCLIENT_CORE_H

struct Character {
	int x, y;
	char *name;
};

class Chatmsg {
  public:
    string msg;
    Chatmsg *next;
    Chatmsg *prev;
    Chatmsg( string );
};

Chatmsg::Chatmsg( string message ):
  msg( message ),
  next( NULL ),
  prev( NULL )
{};

class ChatWindow {
  private:
    int _ypos;
    int _maxlines;
    int _messages;
    Chatmsg* _lastmsg;

    void trim();
  public:
    ChatWindow( int, int);
    ~ChatWindow();
    bool addMessage( string );
    void draw();
};

ChatWindow::ChatWindow( int ypos, int vl ): 
  _ypos( ypos ),
  _maxlines( vl ),
  _lastmsg( NULL ),
  _messages( 0 ) {};

ChatWindow::~ChatWindow() {
  Chatmsg *temp;
  while ( _lastmsg ) {
    temp = _lastmsg->next;
    delete temp;
  }
};

void ChatWindow::trim(){
  int num = 0;
  Chatmsg *temp = _lastmsg,
          *del;
  
  while( temp->next ) {
    temp = temp->next;
  }
  del = temp;
  temp->prev->next = NULL;
  delete del;	
}

bool ChatWindow::addMessage( string message ) {
  if( message.empty() )
    return false;

  Chatmsg *temp = new Chatmsg( message );

  if ( _lastmsg ) {
    temp->next = _lastmsg;
    _lastmsg->prev = temp;
  } else {
    temp->next = NULL;
  }
  temp->prev = NULL;
  _lastmsg = temp;
  _messages++;
  if( _messages > _maxlines )
  	trim();
  return true;
};

void ChatWindow::draw() {
  int pos = LINES - 4;    
  move( _ypos, 1);
  hline( '-', COLS-2 );
 	move( LINES - 3, 1);
  hline( '-', COLS-2 );
  Chatmsg *temp = _lastmsg;
  while( temp ) {
    mvaddstr( pos--, 2, temp->msg.c_str() );
    temp = temp->next;
  }
};

#endif
