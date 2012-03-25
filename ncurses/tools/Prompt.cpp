
#include "Prompt.h"

Prompt::Prompt( PROMPTCHARSET pcs ):
    _colX(0),
    _lineY(0),
    _startPos(0),
    _maxLen( COLS ),
    _inputBuff( "" ),
    _charset( pcs )
{};

string Prompt::getMessage( int x, int y, int ml ) {
    if ( x < 0 || x > COLS || y < 0 || y > LINES )
       return string("");

    bool more = true;
    int c;
    
    _colX = _startPos =  x;
    _lineY = y;
    _maxLen = ml;

    curs_set(2);

    // for some dumb reason, it moves down then over
    // move ( y, x ) 
    move( _lineY, _colX );

    while( more ) {
        c = getch();

        // Is there room?
        if ( _inputBuff.length() < _maxLen ) {

            // Only letters and numbers?
            if (  _charset == ALPHANUMERIC || _charset == PASSWORD  ) {
                if ( ( c > 64 && c < 123 ) || ( c > 47 && c < 58 ) || c == 32 ) {
                    addChar( (char)c );
                    advanceCursor();
                }
            } 

            // Symobols too?            
            else if ( _charset == ALPHANUMERIC_SYMBOLS ) {
                if ( c > 31 && c < 127 ) {
                    addChar( (char)c );
                    advanceCursor();
                }
            }
        }

        // Handle non-characters   
        switch( c ) {
            // Enter
            case 10:
            case 13:
            	curs_set(0);
                return _inputBuff;
            // Backspace
            case KEY_BACKSPACE:
                if ( _inputBuff.length() > 0 ) {
                    deleteCharAt( _colX - _startPos );
                    retreatCursor();
                }
                break;
            default:
                break; 
        }
    }   
}; // end getMessage

// move cursor forwards
void Prompt::advanceCursor() {
    // can assume a check has been performed already
    _colX++;
    move( _lineY, _colX );
};

// move cursor back one space
void Prompt::retreatCursor() {
    // can assume a check has been performed already
    _colX--;
    move( _lineY, _colX );
};

void Prompt::deleteCharAt( int pos ) {
    string::iterator it = _inputBuff.begin() + ( pos - 1 );
    _inputBuff.erase( it );
    mvaddch( _lineY, _colX - 1, ' ' );
};

void Prompt::addChar( char c ) {
    if ( _charset == PASSWORD )
        addch( '*' );
    else    
        addch( c );
    _inputBuff += c;
};


